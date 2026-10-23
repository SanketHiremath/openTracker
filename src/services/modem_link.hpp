#pragma once

#include <cstdint>
#include <span>

#include <zephyr/kernel.h>

/* "Cell active": held by the cellular task only around a transmission
 * (publish_telemetry(), which includes modem power-on and waiting for signal)
 * and released when it is done or has hit its time limit
 * (ct::config::kCellSessionMaxMs). The power task takes it before powering
 * the modem down for System OFF, so it waits for a transmission to finish
 * instead of cutting it off -- for at most about kCellLockWaitMs -- and then
 * keeps holding it until the MCU is off (or releases it if sleep is
 * abandoned), so no new transmission can start in between.
 */
extern struct k_mutex cell_active;

namespace ct::modem {

/* Bench connectivity check: waits for usable signal (see
 * kMinSignalDbm/kSignalRetryIntervalS in app_config.hpp -- passed to the
 * driver, which stays powered on and re-polls AT+CSQ in place while
 * waiting, rather than power-cycling per attempt; see
 * eg916q_wait_for_signal()'s doc comment in the driver for why a
 * power-cycle-per-check design proved unreliable on real hardware), then
 * pings www.google.com and does a plain HTTP GET of it over a raw TCP
 * socket, all via the module's own embedded TCP/IP AT commands -- see
 * eg916q_network_selftest()'s doc comment for why this exists separately
 * from publish_telemetry() below (isolates "does the cellular link work at
 * all" from the much more complex MQTT/TLS path). One power-on/off cycle
 * per call. Returns 0 if both checks passed, -ENODEV if the modem device is
 * not ready, -ENOTCONN if either check failed (see the log for which one).
 */
int network_selftest();

/* One-shot AT+CSQ signal check for bench debugging -- see
 * eg916q_get_signal_quality()'s doc comment in the driver. No retry loop;
 * caller controls polling cadence. Returns 0 and writes the RSSI in dBm to
 * *rssi_dbm on success, -ENODEV if the modem device is not ready, another
 * negative errno otherwise (see the driver for specifics, e.g. -EAGAIN if
 * RSSI is still unknown).
 */
int check_signal(int *rssi_dbm);

/* One-shot bench ping to www.google.com for debugging -- see
 * eg916q_ping()'s doc comment in the driver. Uses kMinSignalDbm/
 * kSignalRetryIntervalS from app_config.hpp internally, same as
 * network_selftest(). Returns 0 on success, -ENODEV if the modem device is
 * not ready, another negative errno otherwise (see the driver for
 * specifics).
 */
int ping();

/* One-shot bench HTTP GET of www.google.com for debugging -- see
 * eg916q_http_get()'s doc comment in the driver. Uses kMinSignalDbm/
 * kSignalRetryIntervalS from app_config.hpp internally, same as ping().
 * Returns 0 on success, -ENODEV if the modem device is not ready, another
 * negative errno otherwise (see the driver for specifics).
 */
int http_get();

/* Powers the modem on, waits for usable signal (same as network_selftest()
 * above), brings up the module's own embedded TCP/IP context, uploads the
 * broker's CA cert, and publishes `payload` to the configured MQTT-over-TLS
 * broker (host/port/credentials/topic all from CONFIG_CAR_TRACKER_MQTT_*)
 * -- then leaves the modem on after a success (a failure power-cycles it off),
 * so later calls skip power-on and attach; power_down() powers it off
 * before System OFF. See eg916q_mqtt_publish()'s doc comment for why (the module's
 * own AT-command MQTT/TLS stack is used instead of a host-side one -- no
 * PPP, no on-MCU mbedTLS).
 *
 * deadline_ms is an absolute k_uptime_get() time (0 = no limit) after which the
 * call gives up with -ETIMEDOUT and powers the modem off; the caller derives
 * it from ct::config::kCellSessionMaxMs.
 *
 * Returns 0 once the broker acknowledges the publish, -ENODEV if the modem
 * device is not ready, -ETIMEDOUT if deadline_ms passed, another negative
 * errno on failure (see eg916q_mqtt_publish()'s doc comment for the specific
 * codes).
 */
int publish_telemetry(std::span<const char> payload, std::int64_t deadline_ms);

/* Powers the modem fully off (AT+QPOWD), ahead of System OFF. Blocks ~12 s
 * when the modem was on; does nothing when it was already off. The caller
 * must hold cell_active. Returns 0, or -ENODEV if the modem device is not
 * ready.
 */
int power_down();

} /* namespace ct::modem */
