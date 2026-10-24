/*
 * GPS acquisition task.
 *
 * Responsibility: turn NEO-6M NMEA output into GpsFix messages on the bus,
 * and debounce speed into a moving/parked MotionState verdict (see
 * MotionDetector below) -- that debounce lives here, next to the data it
 * debounces, rather than in a separate consumer that would just have to
 * re-derive it from the same fixes. What to *do* with that verdict (gate a
 * telemetry publish, etc.) is still someone else's policy; this task only
 * reports it.
 *
 * Threading note: the GNSS driver invokes its data callback from the modem
 * chat workqueue. Publishing to zbus from there would block a shared
 * workqueue on our subscribers' queues, so the callback only does a
 * non-blocking k_msgq_put and this task does the actual publish. That keeps
 * the driver's context free and makes back-pressure explicit (a full queue
 * drops the oldest sample rather than stalling the driver).
 *
 * Liveness vs. fix: the driver only delivers a sample (the callback above)
 * once GGA and RMC both carry a parseable UTC, so a receiver that is
 * searching with no time yet delivers none. Link liveness is therefore
 * tracked separately, from linker-wrapped GGA/RMC match callbacks (see the
 * __wrap_ hooks and CMakeLists.txt): the boot self-test passes on any GGA/RMC
 * line, and the GPS watchdog is fed by a sample *or* a recent NMEA line, so
 * "no fix yet" never reboots the device -- only a silent UART does.
 */

#include <cstdint>
#include <cstdlib>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gnss.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/data/navigation.h>
#include <zephyr/logging/log.h>
#include <zephyr/modem/chat.h>
#include <zephyr/pm/device.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/zbus/zbus.h>

#include "../app_config.hpp"
#include "../channels.hpp"
#include "../messages.hpp"
#include "gps_task.hpp"

#include "../services/selftest.hpp"
#include "supervisor_task.hpp"

LOG_MODULE_REGISTER(ct_gps, CT_LOG_LEVEL);

namespace {

const struct device *const kGnssDev = DEVICE_DT_GET(DT_ALIAS(gnss));

/* The gnss-nmea-generic driver owns uart0 for NMEA *reception* (RX pipe/
 * chat), but that doesn't stop us writing to the same physical UART's TX
 * line directly -- uart_poll_out() just pushes a byte into the peripheral's
 * TX path and does not touch the driver's RX-side state, so the two are
 * safe to interleave. This is how standby()/wake() below reach the NEO-6M
 * with raw UBX bytes despite the driver only speaking NMEA.
 */
const struct device *const kGnssUartDev = DEVICE_DT_GET(DT_BUS(DT_ALIAS(gnss)));

/* ------------------------------------------------------------------
 * UBX protocol -- just enough to send RXM-PMREQ (power request).
 *
 * Frame: B5 62 <class> <id> <len_lo> <len_hi> <payload...> <ck_a> <ck_b>
 * Checksum is the standard UBX 8-bit Fletcher, accumulated over
 * class/id/len/payload (not the B5 62 sync bytes).
 * ---------------------------------------------------------------- */

constexpr std::uint8_t kUbxSync1 = 0xB5;
constexpr std::uint8_t kUbxSync2 = 0x62;
constexpr std::uint8_t kUbxClassRxm = 0x02;
constexpr std::uint8_t kUbxIdPmreq = 0x41;

/* RXM-PMREQ, legacy 8-byte payload (protocol version 14 / u-blox 6):
 *   duration: U4, ms, 0 = indefinite (until woken)
 *   flags:    X4, bit1 (0x02) = backup mode
 * Newer chips (M8+) use a longer version-tagged payload; the NEO-6M on this
 * board takes the short form.
 *
 * duration is deliberately non-zero (see kGpsBackupMaxDurationMs): a bounded
 * request means the receiver's own timer brings it back out of backup mode
 * before its retained ephemeris can go stale, guaranteeing the next fix is a
 * hot start even if nothing ever calls wake() explicitly.
 */
void build_pmreq_backup_payload(std::uint8_t out[8], std::uint32_t duration_ms)
{
	constexpr std::uint32_t kFlagsBackup = 0x00000002;

	for (int i = 0; i < 4; ++i) {
		out[i] = static_cast<std::uint8_t>(duration_ms >> (8 * i));
	}
	for (int i = 0; i < 4; ++i) {
		out[4 + i] = static_cast<std::uint8_t>(kFlagsBackup >> (8 * i));
	}
}

void ubx_send(std::uint8_t cls, std::uint8_t id, const std::uint8_t *payload, std::uint16_t len)
{
	const std::uint8_t header[4] = {
		cls,
		id,
		static_cast<std::uint8_t>(len & 0xFF),
		static_cast<std::uint8_t>(len >> 8),
	};

	std::uint8_t ck_a = 0;
	std::uint8_t ck_b = 0;

	for (auto b : header) {
		ck_a = static_cast<std::uint8_t>(ck_a + b);
		ck_b = static_cast<std::uint8_t>(ck_b + ck_a);
	}
	for (std::uint16_t i = 0; i < len; ++i) {
		ck_a = static_cast<std::uint8_t>(ck_a + payload[i]);
		ck_b = static_cast<std::uint8_t>(ck_b + ck_a);
	}

	uart_poll_out(kGnssUartDev, kUbxSync1);
	uart_poll_out(kGnssUartDev, kUbxSync2);
	for (auto b : header) {
		uart_poll_out(kGnssUartDev, b);
	}
	for (std::uint16_t i = 0; i < len; ++i) {
		uart_poll_out(kGnssUartDev, payload[i]);
	}
	uart_poll_out(kGnssUartDev, ck_a);
	uart_poll_out(kGnssUartDev, ck_b);
}

/* Raw samples handed from driver callback context to the GPS thread. */
K_MSGQ_DEFINE(gps_raw_queue, sizeof(ct::GpsFix), ct::config::kGpsRawQueueDepth,
	      alignof(ct::GpsFix));

/* Given the first time a GGA or RMC line arrives, whether or not it carries
 * time or a fix -- proof the MCU<->GPS UART link is alive. Consumed by
 * ct::gps::wait_for_first_sample(), which the boot self-test calls. This is
 * independent of gps_raw_queue above (which feeds the normal fix pipeline).
 * Given from the __wrap_ GGA/RMC hooks below, NOT from gnss_data_cb: the
 * driver only calls gnss_data_cb once a GGA and an RMC with a parseable,
 * matching UTC have both arrived, and a receiver without time or fix sends
 * empty time fields, so a no-fix link would never reach the callback.
 */
K_SEM_DEFINE(gps_first_sample_sem, 0, 1);

/* k_uptime_get_32() of the most recent GGA/RMC line, written by the __wrap_
 * hooks below. Lets the GPS thread tell "receiver alive but no time/fix yet"
 * (no gnss_data_cb samples, but lines still arriving) from "receiver or UART
 * dead" (nothing arriving), so only the latter stops the watchdog feed.
 */
atomic_t gps_last_nmea_ms = ATOMIC_INIT(0);

/* Set once the GPS thread is actually draining gps_raw_queue. Until then the
 * driver callback throws samples away rather than queueing them -- see
 * gnss_data_cb(). Atomic because the writer is the GPS thread and the reader
 * is the driver's workqueue callback.
 */
atomic_t gps_consumer_ready = ATOMIC_INIT(0);

K_THREAD_STACK_DEFINE(gps_stack, ct::config::kGpsStackSize);
k_thread gps_thread_data;

/* ------------------------------------------------------------------
 * Motion debounce (moved here from the supervisor -- see the file header
 * comment on why it belongs with the data it debounces)
 * ------------------------------------------------------------------ */

class MotionDetector {
public:
	/* Feeds one fix; returns true if the moving/parked verdict changed. */
	bool update(const ct::GpsFix &fix)
	{
		if (!fix.valid) {
			/* Without a fix we cannot claim motion, but we also do
			 * not clear an established verdict on a single dropout.
			 */
			consecutive_ = 0;
			return false;
		}

		const bool above = fix.speed_mms >= ct::config::kMovingSpeedThresholdMms;

		if (above) {
			if (consecutive_ < ct::config::kMovingDebounceCount) {
				++consecutive_;
			}
		} else {
			consecutive_ = 0;
		}

		const bool now_moving = consecutive_ >= ct::config::kMovingDebounceCount;

		if (now_moving != moving_) {
			moving_ = now_moving;
			return true;
		}

		return false;
	}

	[[nodiscard]] bool moving() const { return moving_; }

private:
	std::uint32_t consecutive_{0};
	bool moving_{false};
};

void publish_motion(bool moving)
{
	ct::MotionState state{};
	state.moving = moving;
	(void)zbus_chan_pub(&chan_motion_state, &state, K_MSEC(50));
}

} /* namespace */

/* Link-liveness hooks. CMakeLists.txt links with --wrap on the driver's GGA
 * and RMC match callbacks, so the driver's unsolicited-match table calls
 * these instead. They run on every matched line *before* the driver parses
 * it, which makes the boot self-test's "NMEA is arriving" check independent
 * of whether the receiver has time or a fix yet. Runs on the modem chat
 * workqueue: must not block (k_sem_give is safe; extra gives beyond the
 * semaphore's limit of 1 are a documented no-op).
 */
extern "C" {
void __real_gnss_nmea0183_match_gga_callback(struct modem_chat *chat, char **argv,
					     uint16_t argc, void *user_data);
void __real_gnss_nmea0183_match_rmc_callback(struct modem_chat *chat, char **argv,
					     uint16_t argc, void *user_data);

void __wrap_gnss_nmea0183_match_gga_callback(struct modem_chat *chat, char **argv,
					     uint16_t argc, void *user_data)
{
	atomic_set(&gps_last_nmea_ms, static_cast<atomic_val_t>(k_uptime_get_32()));
	k_sem_give(&gps_first_sample_sem);
	__real_gnss_nmea0183_match_gga_callback(chat, argv, argc, user_data);
}

void __wrap_gnss_nmea0183_match_rmc_callback(struct modem_chat *chat, char **argv,
					     uint16_t argc, void *user_data)
{
	atomic_set(&gps_last_nmea_ms, static_cast<atomic_val_t>(k_uptime_get_32()));
	k_sem_give(&gps_first_sample_sem);
	__real_gnss_nmea0183_match_rmc_callback(chat, argv, argc, user_data);
}
}

namespace {

/* Runs in GNSS driver (workqueue) context -- must not block. */
void gnss_data_cb(const struct device *dev, const struct gnss_data *data)
{
	ARG_UNUSED(dev);

	/* The GPS thread parks on the self-test gate before entering its loop,
	 * so until it starts draining there is no consumer. Queueing anyway
	 * would fill gps_raw_queue within seconds, log a drop warning on every
	 * subsequent sample, and leave a backlog of stale fixes to publish in
	 * one burst the moment the gate opens -- stale enough to skew the
	 * supervisor's motion debounce. Discard at the source instead.
	 */
	if (atomic_get(&gps_consumer_ready) == 0) {
		return;
	}

	ct::GpsFix fix{};
	fix.valid = (data->info.fix_status != GNSS_FIX_STATUS_NO_FIX);
	fix.satellites = data->info.satellites_cnt;
	fix.uptime_s = static_cast<std::uint32_t>(k_uptime_get() / 1000);

	if (fix.valid) {
		fix.latitude_ndeg = data->nav_data.latitude;
		fix.longitude_ndeg = data->nav_data.longitude;
		fix.altitude_mm = data->nav_data.altitude;
		fix.speed_mms = data->nav_data.speed;
		fix.bearing_mdeg = data->nav_data.bearing;
	}

	/* Never block the driver: on overflow drop the oldest queued sample.
	 * Losing a stale fix is strictly better than stalling acquisition.
	 */
	if (k_msgq_put(&gps_raw_queue, &fix, K_NO_WAIT) != 0) {
		ct::GpsFix discarded{};
		(void)k_msgq_get(&gps_raw_queue, &discarded, K_NO_WAIT);
		(void)k_msgq_put(&gps_raw_queue, &fix, K_NO_WAIT);
		LOG_WRN("GPS queue full; dropped oldest sample");
	}
}

void gps_thread_entry(void *, void *, void *)
{
	LOG_INF("GPS task started");

	ct::selftest::wait_gps_ready();

	/* Only from here on is there anything draining the queue. */
	atomic_set(&gps_consumer_ready, 1);

	MotionDetector motion;
	publish_motion(false);

	/* From here the receiver must keep talking: it emits GGA/RMC every second,
	 * fix or no fix. A sample, or a bare NMEA line while the receiver has no
	 * time/fix yet, feeds the watchdog; silence (dead receiver or UART)
	 * does not, so the board resets instead of this thread waiting forever.
	 */
	const int wdt = ct::supervisor::watch(ct::config::kGpsWdtMs);

	for (;;) {
		ct::GpsFix fix{};

		if (k_msgq_get(&gps_raw_queue, &fix, ct::config::kGpsSampleWait) != 0) {
			/* No sample, but samples only exist once the receiver has time
			 * or a fix. If NMEA lines are still arriving the hardware is
			 * fine and the receiver is just searching (cold start, no sky
			 * view): keep feeding, or the device would reboot-loop until
			 * it got a fix.
			 */
			const std::uint32_t nmea_age_ms =
				k_uptime_get_32() - static_cast<std::uint32_t>(atomic_get(&gps_last_nmea_ms));

			if (nmea_age_ms < k_ticks_to_ms_floor32(ct::config::kGpsSampleWait.ticks)) {
				ct::supervisor::feed(wdt);
				LOG_INF("GPS searching: NMEA alive, no time/fix yet");
				continue;
			}

			LOG_WRN("no GPS sample for %d s", static_cast<int>(
				k_ticks_to_ms_floor64(ct::config::kGpsSampleWait.ticks) / 1000));
			continue;
		}

		ct::supervisor::feed(wdt);

		if (motion.update(fix)) {
			LOG_INF(">>> %s <<<", motion.moving() ? "MOVING" : "parked");
			publish_motion(motion.moving());
		}

		if (fix.valid) {
			/* nanodegrees -> whole degrees + 7 decimals, integer only. */
			const auto lat_deg = static_cast<std::int32_t>(fix.latitude_ndeg / 1000000000);
			const auto lon_deg = static_cast<std::int32_t>(fix.longitude_ndeg / 1000000000);
			/* mm/s -> km/h*10: mm/s * 3600 s/h / 1e6 mm/km * 10 = mm/s * 36 / 1000. */
			const auto speed_kmh_x10 = (fix.speed_mms * 36U) / 1000U;

			LOG_INF("fix: %d.%07lld, %d.%07lld  %u.%u km/h  sats=%u", lat_deg,
				std::llabs((fix.latitude_ndeg % 1000000000) / 100), lon_deg,
				std::llabs((fix.longitude_ndeg % 1000000000) / 100),
				speed_kmh_x10 / 10U, speed_kmh_x10 % 10U, fix.satellites);
		} else {
			LOG_INF("no fix (sats=%u)", fix.satellites);
		}

		const int err = zbus_chan_pub(&chan_gps_fix, &fix, K_MSEC(50));

		if (err != 0) {
			LOG_WRN("failed to publish fix: %d", err);
		}
	}
}

} /* namespace */

/* Registered at build time; fires once the driver is up. */
GNSS_DATA_CALLBACK_DEFINE(kGnssDev, gnss_data_cb);

namespace ct::gps {

int start()
{
	if (!device_is_ready(kGnssDev)) {
		LOG_ERR("GNSS device not ready");
		return -ENODEV;
	}

#if defined(CONFIG_PM_DEVICE)
	/* Mandatory, not an optimisation: with CONFIG_PM_DEVICE the in-tree
	 * gnss-nmea-generic driver ends its init with pm_device_init_suspended()
	 * and only opens its UART pipe in the RESUME action -- so until
	 * something resumes it, uart0 RX is never started and not a single
	 * NMEA sentence arrives. Builds without PM_DEVICE resume inside the
	 * driver's own init instead, so this block simply does not exist there.
	 */
	if (const int err = pm_device_action_run(kGnssDev, PM_DEVICE_ACTION_RESUME);
	    err != 0 && err != -EALREADY) {
		LOG_ERR("failed to resume GNSS device: %d", err);
		return err;
	}
#endif

	/* The device may have come out of System OFF with the receiver still in
	 * backup mode (see standby_until_wake()). Nudge it now, before the boot
	 * self-test starts its 5 s wait for NMEA, or that wait would fail. On a
	 * normal power-on the receiver is already running and just ignores the
	 * stray bytes.
	 */
	wake();

	k_thread_create(&gps_thread_data, gps_stack, K_THREAD_STACK_SIZEOF(gps_stack),
			gps_thread_entry, nullptr, nullptr, nullptr,
			ct::config::kGpsPriority, 0, K_NO_WAIT);
	k_thread_name_set(&gps_thread_data, "gps");

	return 0;
}

int wait_for_first_sample(k_timeout_t timeout)
{
	return k_sem_take(&gps_first_sample_sem, timeout);
}

void standby()
{
	std::uint8_t payload[8];
	build_pmreq_backup_payload(payload, ct::config::kGpsBackupMaxDurationMs);

	LOG_INF("requesting UBX backup mode for up to %u s",
		ct::config::kGpsBackupMaxDurationMs / 1000);
	ubx_send(kUbxClassRxm, kUbxIdPmreq, payload, sizeof(payload));

	/* VCC stays applied, so the receiver's battery-backed RAM (ephemeris,
	 * almanac, last position/time) is retained across this -- unlike
	 * cutting power at the module's VCC pin, which on typical NEO-6M
	 * breakouts also drops V_BCKP (it's tied to the same rail) and would
	 * force a cold start on every wake. Combined with the bounded
	 * duration above (ephemeris stays within its ~4 hour validity
	 * window), the next fix after wake is a hot start.
	 */
}

void standby_until_wake()
{
	std::uint8_t payload[8];
	build_pmreq_backup_payload(payload, 0);

	LOG_INF("requesting UBX backup mode until woken");
	ubx_send(kUbxClassRxm, kUbxIdPmreq, payload, sizeof(payload));

	/* Same battery-backed-RAM reasoning as standby() above. */
}

void wake()
{
	LOG_INF("nudging GPS out of backup mode");

	/* The NEO-6M has no EXTINT or RESET line wired on this board (see
	 * app.overlay's uart0 comment), so a hardware wake signal isn't an
	 * option. UART RX activity waking the receiver from backup mode is
	 * the commonly-used trick for these modules, but it is not a
	 * datasheet-guaranteed wake source the way EXTINT is -- if a given
	 * unit/firmware doesn't come back reliably, wiring EXTINT or
	 * RESET_N to a spare GPIO (mirroring the cellular modem's
	 * mdm-reset-gpios in app.overlay) is the fallback.
	 */
	for (int i = 0; i < 16; ++i) {
		uart_poll_out(kGnssUartDev, 0xFF);
	}
}

} /* namespace ct::gps */
