/*
 * Cellular task.
 *
 * Responsibility: decide when a telemetry publish is due (cadence + retry/
 * backoff -- see PublishScheduler below, moved here from the supervisor)
 * and execute it. It owns the modem/link lifecycle and the MQTT session,
 * end to end, in this one thread -- no separate "decide" and "execute"
 * threads round-tripping a request/result over the bus, since the same
 * thread that decides a publish is due is the one blocking-I/O thread that
 * can actually run it.
 *
 * While the MCU is awake the modem stays powered on and registered between
 * publishes (a successful publish leaves it on; a failed one power-cycles it
 * to recover), so each publish skips the ~30-40 s power-on and attach. The
 * power task powers it down just before System OFF, holding cell_active.
 * cell_active is held only around a transmission and for at most
 * ct::config::kCellSessionMaxMs, so a parked vehicle with no coverage still
 * gets to sleep.
 * ct::modem::publish_telemetry() is a single self-contained call handling the
 * whole cycle -- registration, the module's own embedded TCP/IP+TLS+MQTT
 * stack -- so there is no separate connect()/disconnect() to sequence here (see
 * modem_link.cpp and the quectel_eg916q driver's header comment for why:
 * this module has its own AT-command MQTT/TLS client, used instead of PPP +
 * a host-side stack).
 *
 * Each publish runs through a small state machine (see execute() below):
 * check signal strength first, and only pay for the much more expensive
 * publish_telemetry() (registration + the module's TCP/IP/TLS/MQTT
 * handshake) once signal looks usable. On weak/no signal, wait and re-check
 * rather than attempting -- and failing -- a full publish.
 */

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "../app_config.hpp"
#include "../channels.hpp"
#include "../messages.hpp"
#include "../services/modem_link.hpp"
#include "../services/selftest.hpp"
#include "cellular_task.hpp"
#include "supervisor_task.hpp"

LOG_MODULE_REGISTER(ct_cellular, CT_LOG_LEVEL);

/* External linkage required: channels.cpp references this observer. */
ZBUS_SUBSCRIBER_DEFINE(cellular_sub, ct::config::kCellularSubQueueDepth);

namespace {

K_THREAD_STACK_DEFINE(cellular_stack, ct::config::kCellularStackSize);
k_thread cellular_thread_data;

#if 0
/* --- Normal publish-scheduling logic -- TEMPORARILY DISABLED ---
 *
 * Commented out (not deleted) while bench-testing signal strength in
 * isolation: cellular_thread_entry() below currently just powers the modem
 * on and logs AT+CSQ every 5s in a loop, ignoring GPS fixes/motion state
 * entirely. Restore this block and cellular_thread_entry()'s original body
 * once signal behavior is confirmed.
 */

void announce_link(bool connected)
{
	ct::LinkState state{};
	state.connected = connected;
	(void)zbus_chan_pub(&chan_link_state, &state, K_MSEC(50));
}

/* Renders a fix as compact JSON. Returns the populated span, or an empty
 * span if formatting failed/truncated.
 */
std::span<const char> format_payload(const ct::GpsFix &fix, std::span<char> buffer)
{
	const auto lat_deg = static_cast<std::int32_t>(fix.latitude_ndeg / 1000000000);
	const auto lat_frac = static_cast<std::int32_t>(std::llabs((fix.latitude_ndeg % 1000000000) / 100));
	const auto lon_deg = static_cast<std::int32_t>(fix.longitude_ndeg / 1000000000);
	const auto lon_frac = static_cast<std::int32_t>(std::llabs((fix.longitude_ndeg % 1000000000) / 100));
	/* mm/s -> km/h*10: mm/s * 3600 s/h / 1e6 mm/km * 10 = mm/s * 36 / 1000. */
	const auto speed_kmh_x10 = (fix.speed_mms * 36U) / 1000U;

	const int len = std::snprintf(
		buffer.data(), buffer.size(),
		R"({"lat":%d.%07d,"lon":%d.%07d,"spd_kmh":%u.%u,"sats":%u,"up_s":%u})", lat_deg,
		lat_frac, lon_deg, lon_frac, speed_kmh_x10 / 10U, speed_kmh_x10 % 10U,
		fix.satellites, fix.uptime_s);

	if (len < 0 || static_cast<std::size_t>(len) >= buffer.size()) {
		LOG_ERR("payload formatting failed/truncated");
		return {};
	}

	return {buffer.data(), static_cast<std::size_t>(len)};
}

/* ------------------------------------------------------------------
 * Publish scheduling (moved here from the supervisor -- see the file
 * header comment on why "decide" and "execute" are now the same thread)
 * ------------------------------------------------------------------ */

class PublishScheduler {
public:
	/* True when a new publish attempt should be issued now. See
	 * kPublishRequireMoving's comment in app_config.hpp for why this is
	 * currently a flat timer rather than gated on motion.
	 */
	[[nodiscard]] bool due(bool moving, std::int64_t now_ms) const
	{
		if (ct::config::kPublishRequireMoving && !moving) {
			return false;
		}
		return !in_flight_ && now_ms >= next_attempt_ms_;
	}

	[[nodiscard]] std::uint32_t begin(std::int64_t now_ms)
	{
		in_flight_ = true;
		last_issued_ms_ = now_ms;
		return ++sequence_;
	}

	void on_result(std::uint32_t sequence, std::int32_t status, std::int64_t now_ms)
	{
		if (sequence != sequence_) {
			LOG_WRN("ignoring stale result #%u (current #%u)", sequence, sequence_);
			return;
		}

		in_flight_ = false;

		if (status == 0) {
			attempt_ = 0;
			next_attempt_ms_ = now_ms + kIntervalMs;
			return;
		}

		++attempt_;

		if (attempt_ >= ct::config::kPublishMaxAttempts) {
			LOG_ERR("giving up after %u attempts; next window in %lld s", attempt_,
				kIntervalMs / 1000);
			attempt_ = 0;
			next_attempt_ms_ = now_ms + kIntervalMs;
			return;
		}

		/* Exponential backoff: 5s, 10s, 20s, ... */
		const std::int64_t delay_s = static_cast<std::int64_t>(
			ct::config::kPublishRetryBaseDelayS << (attempt_ - 1));

		LOG_WRN("retry %u/%u in %lld s", attempt_ + 1, ct::config::kPublishMaxAttempts,
			delay_s);
		next_attempt_ms_ = now_ms + (delay_s * 1000);
	}

	/* Called when the car stops: abandon the current retry cycle so the next
	 * trip starts with an immediate publish rather than a stale backoff.
	 */
	void reset()
	{
		attempt_ = 0;
		next_attempt_ms_ = 0;
	}

private:
	static constexpr std::int64_t kIntervalMs =
		k_ticks_to_ms_floor64(ct::config::kMovingPublishInterval.ticks);

	std::uint32_t sequence_{0};
	std::uint32_t attempt_{0};
	std::int64_t next_attempt_ms_{0};
	std::int64_t last_issued_ms_{0};
	bool in_flight_{false};
};

enum class ConnectState {
	kCheckSignal,
	kPublish,
};

#if 0
/* --- Real telemetry publish logic -- TEMPORARILY DISABLED ---
 *
 * Commented out (not deleted) while chasing why no data was reaching
 * HiveMQ: the MQTT/TLS/AT+QFUPL path below is the most complex part of the
 * native-AT rewrite and the hardest to isolate failures in on its own, so
 * execute() below currently calls ct::modem::network_selftest() instead --
 * a much simpler ping + plain HTTP GET bench check -- to first confirm the
 * cellular link works at all, independent of MQTT entirely. Restore this
 * block (and execute()'s original body) once that's confirmed.
 *
 * One publish, run through a small state machine:
 *
 *   kCheckSignal -- weak/no signal? --> sleep kSignalRetryIntervalS,
 *                                       loop back to kCheckSignal
 *        |
 *        v (signal >= kMinSignalDbm)
 *   kPublish -- ct::modem::publish_telemetry(): registration, the module's
 *               own TCP/IP+TLS+MQTT handshake, publish, and power-down, all
 *               in one call
 *
 * The signal check is a full standalone modem power cycle (~tens of
 * seconds -- see eg916q_get_signal_quality()'s doc comment), so it isn't
 * free, but it's far cheaper than attempting -- and failing -- the much
 * longer publish_telemetry() below on a link that has no chance of coming
 * up. No bound on the retry loop by design: there's nothing else useful for
 * this thread to do while out of coverage, and the gap is expected to clear
 * (garage, tunnel, dead zone) rather than be permanent.
 */
std::int32_t execute(const ct::GpsFix &fix)
{
	std::array<char, 160> buffer{};
	const auto payload = format_payload(fix, buffer);

	if (payload.empty()) {
		return -EMSGSIZE;
	}

	ConnectState state = ConnectState::kCheckSignal;

	for (;;) {
		if (state == ConnectState::kCheckSignal) {
			int rssi_dbm = 0;
			const int err = ct::modem::get_signal_quality(&rssi_dbm);

			if (err == 0 && rssi_dbm >= ct::config::kMinSignalDbm) {
				LOG_INF("signal OK (%d dBm); publishing", rssi_dbm);
				state = ConnectState::kPublish;
				continue;
			}

			if (err == 0) {
				LOG_WRN("signal too weak (%d dBm)", rssi_dbm);
			} else {
				LOG_WRN("signal check failed: %d", err);
			}
			LOG_WRN("retrying signal check in %u s", ct::config::kSignalRetryIntervalS);
			k_sleep(K_SECONDS(ct::config::kSignalRetryIntervalS));
			continue;
		}

		/* state == ConnectState::kPublish */
		LOG_INF("publishing: %.*s", static_cast<int>(payload.size()), payload.data());
		announce_link(true);
		const std::int32_t status = ct::modem::publish_telemetry(payload);
		announce_link(false);

		return status;
	}
}
#endif

/* --- Bench connectivity check (TEMPORARY, see the #if 0 block above) ---
 *
 * Ignores the fix's payload entirely: this only asks "does the cellular
 * link work at all" (ping + plain HTTP GET over the module's own TCP/IP AT
 * commands), not "did this fix get published."
 *
 * No separate signal-check state here: ct::modem::network_selftest() waits
 * for usable signal internally, in the same powered session as everything
 * after it (registration, ping, HTTP GET) -- see its doc comment for why an
 * earlier design that checked signal via its own separate power-on/off
 * cycle, retried from this thread, proved unreliable on real hardware
 * (AT+CSQ kept timing out because a fresh ~20s PWRKEY cycle every retry
 * never gave the module enough settled time).
 */
std::int32_t execute(const ct::GpsFix &fix)
{
	ARG_UNUSED(fix);

	announce_link(true);
	const std::int32_t status = ct::modem::network_selftest();
	announce_link(false);

	if (status == 0) {
		LOG_INF("network self-test: ping + HTTP GET both succeeded");
	} else {
		LOG_WRN("network self-test failed: %d", status);
	}

	return status;
}
#endif /* normal publish-scheduling logic */

/* Tells observers (the power task) whether this task is inside a modem
 * session. Live counterpart of announce_link() in the disabled block above,
 * kept separate so re-enabling that block does not collide with it.
 */
void announce_modem_session(bool busy)
{
	ct::LinkState state{};
	state.connected = busy;
	(void)zbus_chan_pub(&chan_link_state, &state, K_MSEC(50));
}

/* Renders a fix as ThingSpeak's expected "field1=...&field2=..." form
 * (see Kconfig's CAR_TRACKER_MQTT_TOPIC help): field1/field2 = lat/lon so
 * they land in the channel's numeric fields for charting, field3 = speed,
 * field4 = battery (always 0 -- no battery/voltage sensor exists on this
 * hardware yet; this is a placeholder for when one is added), and the
 * trailing lat=/long= pair additionally populates ThingSpeak's own
 * location metadata for the map view, which the numbered fields alone
 * don't do. Returns the populated span, or an empty span if formatting
 * failed/truncated.
 */
std::span<const char> format_thingspeak_payload(const ct::GpsFix &fix, std::span<char> buffer)
{
	const auto lat_deg = static_cast<std::int32_t>(fix.latitude_ndeg / 1000000000);
	const auto lat_frac = static_cast<std::int32_t>(std::llabs((fix.latitude_ndeg % 1000000000) / 100));
	const auto lon_deg = static_cast<std::int32_t>(fix.longitude_ndeg / 1000000000);
	const auto lon_frac = static_cast<std::int32_t>(std::llabs((fix.longitude_ndeg % 1000000000) / 100));
	/* mm/s -> km/h*10: mm/s * 3600 s/h / 1e6 mm/km * 10 = mm/s * 36 / 1000. */
	const auto speed_kmh_x10 = (fix.speed_mms * 36U) / 1000U;

	const int len = std::snprintf(
		buffer.data(), buffer.size(),
		"field1=%d.%07d&field2=%d.%07d&field3=%u.%u&field4=0&lat=%d.%07d&long=%d.%07d",
		lat_deg, lat_frac, lon_deg, lon_frac, speed_kmh_x10 / 10U, speed_kmh_x10 % 10U, lat_deg,
		lat_frac, lon_deg, lon_frac);

	if (len < 0 || static_cast<std::size_t>(len) >= buffer.size()) {
		LOG_ERR("payload formatting failed/truncated");
		return {};
	}

	return {buffer.data(), static_cast<std::size_t>(len)};
}

/* Takes cell_active, feeding the watchdog while it waits: the power task holds
 * it through the whole shutdown sequence, which can outlast a feed interval.
 */
void lock_cell_active(int wdt)
{
	do {
		ct::supervisor::feed(wdt);
	} while (k_mutex_lock(&cell_active, K_MSEC(ct::config::kTaskWdtPollMs)) != 0);
}

/* k_sleep() for the idle gap between publishes, in slices that feed the
 * watchdog -- the gap is longer than the task's timeout.
 */
void sleep_feeding(int wdt, k_timeout_t total)
{
	const std::int64_t end_ms = k_uptime_get() + k_ticks_to_ms_floor64(total.ticks);

	for (std::int64_t left_ms = end_ms - k_uptime_get(); left_ms > 0;
	     left_ms = end_ms - k_uptime_get()) {
		ct::supervisor::feed(wdt);
		k_sleep(K_MSEC(MIN(left_ms, ct::config::kTaskWdtPollMs)));
	}
}

/* --- Signal/ping/HTTP-GET bench debug loop (TEMPORARY, see the #if 0 block
 * above) --- just powers the modem on and logs AT+CSQ + a ping + an HTTP GET
 * to www.google.com every kMovingPublishInterval, ignoring motion state/publish scheduling
 * entirely (format_thingspeak_payload() above and the latest GPS fix are
 * used for the MQTT publish test below). Restore the #if 0 block and this
 * function's original body once confirmed. Ping/HTTP GET are commented out
 * (not deleted) below while bench-testing the MQTT publish path next --
 * signal/ping/HTTP GET already confirmed working, see modem_link.cpp's
 * ping()/http_get().
 */
void cellular_thread_entry(void *, void *, void *)
{
	LOG_INF("cellular task started");

	ct::selftest::wait_cellular_ready();

	/* Registered only now: before the gate opens this thread is parked on
	 * purpose, which is not a hang. From here every pass round the loop must
	 * come back to a feed within kCellularWdtMs -- one transmission, bounded by
	 * kCellSessionMaxMs plus an overrunning stage, is the longest stretch.
	 */
	const int wdt = ct::supervisor::watch(ct::config::kCellularWdtMs);

	for (;;) {
		ct::supervisor::feed(wdt);

#if 0
		const int ping_err = ct::modem::ping();

		if (ping_err == 0) {
			LOG_INF("ping: www.google.com OK");
		} else {
			LOG_WRN("ping failed: %d", ping_err);
		}

		const int http_err = ct::modem::http_get();

		if (http_err == 0) {
			LOG_INF("http get: www.google.com OK");
		} else {
			LOG_WRN("http get failed: %d", http_err);
		}
#endif /* ping/HTTP GET bench checks -- confirmed working, see above */

		ct::GpsFix fix{};

		if (zbus_chan_read(&chan_gps_fix, &fix, K_MSEC(50)) != 0 || !fix.valid) {
			LOG_WRN("mqtt publish: no GPS fix yet, skipping this round");
			k_sleep(K_SECONDS(10));
			continue;
		}

		std::array<char, 160> buffer{};
		const auto payload = format_thingspeak_payload(fix, buffer);

		if (payload.empty()) {
			k_sleep(K_SECONDS(5));
			continue;
		}

		LOG_INF("mqtt publish: %.*s", static_cast<int>(payload.size()), payload.data());

		/* cell_active covers only the transmission -- modem power-on, waiting
		 * for signal, registration and the publish itself -- not the fix read
		 * or formatting above, nor the sleep below. The deadline caps how long
		 * it can be held: with no coverage the driver gives up, powers the
		 * modem off and returns -ETIMEDOUT, so the power task can take the lock
		 * and sleep instead of the device chasing a signal indefinitely.
		 * Blocks here while the power task is shutting down for sleep.
		 */
		lock_cell_active(wdt);
		announce_modem_session(true);

		/* Full timeout for the transmission, however long the lock wait took. */
		ct::supervisor::feed(wdt);

		const int mqtt_err = ct::modem::publish_telemetry(
			payload, k_uptime_get() + ct::config::kCellSessionMaxMs);

		announce_modem_session(false);
		(void)k_mutex_unlock(&cell_active);
		ct::supervisor::feed(wdt);

		if (mqtt_err == 0) {
			LOG_INF("mqtt publish: OK");
		} else if (mqtt_err == -ETIMEDOUT) {
			LOG_WRN("mqtt publish: gave up after %d s (no signal or slow network)",
				static_cast<int>(ct::config::kCellSessionMaxMs / 1000));
		} else {
			LOG_WRN("mqtt publish failed: %d", mqtt_err);
		}

		/* One attempt per interval, success or failure. The no-fix/format
		 * paths above keep their short 5 s retry so the first publish goes
		 * out as soon as a fix arrives.
		 */
		sleep_feeding(wdt, ct::config::kMovingPublishInterval);
	}
}

} /* namespace */

namespace ct::cellular {

void start()
{
	k_thread_create(&cellular_thread_data, cellular_stack,
			K_THREAD_STACK_SIZEOF(cellular_stack), cellular_thread_entry, nullptr,
			nullptr, nullptr, ct::config::kCellularPriority, 0, K_NO_WAIT);
	k_thread_name_set(&cellular_thread_data, "cellular");
}

} /* namespace ct::cellular */
