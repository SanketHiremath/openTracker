/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Public entry points for the out-of-tree EG916Q-GL modem driver that fall
 * outside the standard Zephyr device/PM-device API (device_is_ready(),
 * pm_device_action_run()) already used by src/services/modem_link.cpp.
 */

#ifndef CAR_TRACKER_DRIVERS_MODEM_QUECTEL_EG916Q_H_
#define CAR_TRACKER_DRIVERS_MODEM_QUECTEL_EG916Q_H_

#include <stdint.h>

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * One-shot power-on + "AT"/"OK" liveness check, independent of the
 * PM_DEVICE_ACTION_RESUME path (which additionally attaches chat, polls
 * network registration for up to CONFIG_MODEM_QUECTEL_EG916Q_REGISTER_TIMEOUT_S,
 * and dials PPP) -- so a modem with no SIM or no signal isn't misreported as
 * unreachable. Never touches the PM_DEVICE action framework, so the
 * device's cached PM state stays SUSPENDED throughout: a later normal
 * pm_device_action_run(PM_DEVICE_ACTION_RESUME) behaves exactly as if this
 * had never run.
 *
 * On success, deliberately leaves the module powered on and its chat
 * session attached rather than powering back off: the next real cellular
 * operation (eg916q_network_selftest()/eg916q_mqtt_publish()) picks up this
 * same session via eg916q_ensure_powered_on() instead of power-cycling
 * again from scratch, which otherwise added a full ~20s PWRKEY pulse/boot
 * cycle to every single call and restarted network registration each time.
 * On failure, the module is powered back off (nothing useful to keep
 * running if it never even answered a bare "AT").
 *
 * Returns 0 if the modem replied OK to AT, a negative errno otherwise
 * (e.g. -ETIMEDOUT if nothing replied in time).
 */
int eg916q_selftest(const struct device *dev);

/*
 * One-shot AT+CSQ signal check for bench debugging -- see the .c file's doc
 * comment for the full rationale. No retry loop and no minimum-signal gate;
 * the caller controls the polling cadence. Deliberately never powers off.
 *
 * Returns 0 and writes the RSSI in dBm to *rssi_dbm on success, a negative
 * errno on failure, or -EAGAIN if RSSI is still unknown (e.g. immediately
 * after boot, before the radio has scanned).
 */
int eg916q_get_signal_quality(const struct device *dev, int *rssi_dbm);

/*
 * One-shot bench ping to www.google.com for debugging -- see the .c file's
 * doc comment. Brings the module to full network readiness (registration +
 * PDP context activation) then issues a single AT+QPING; no HTTP GET.
 * Deliberately leaves the module powered on and the PDP context active on
 * success, so a caller pinging on a cadence reuses the same session.
 *
 * min_rssi_dbm/retry_interval_s: same meaning as
 * eg916q_network_selftest()'s parameters of the same name.
 *
 * Returns 0 if the ping succeeded, -ENOTCONN if the module responded but
 * the ping itself failed, another negative errno for an earlier setup
 * failure.
 */
int eg916q_ping(const struct device *dev, int min_rssi_dbm, uint32_t retry_interval_s);

/*
 * One-shot bench HTTP GET of www.google.com for debugging -- see the .c
 * file's doc comment. Same registration/QICSGP/QIACT setup as
 * eg916q_ping() above, then a plain (non-TLS) HTTP GET over a raw TCP
 * socket. Deliberately leaves the module powered on on success, same as
 * eg916q_ping().
 *
 * min_rssi_dbm/retry_interval_s: same meaning as
 * eg916q_network_selftest()'s parameters of the same name.
 *
 * Returns 0 if the GET got a non-empty response back, -ENOTCONN if a later
 * stage (open/send/read) failed, another negative errno for an earlier
 * setup failure.
 */
int eg916q_http_get(const struct device *dev, int min_rssi_dbm, uint32_t retry_interval_s);

/*
 * One-shot power-on + basic connectivity check: waits for usable signal
 * (see the min_rssi_dbm/retry_interval_s doc below), brings up the module's
 * embedded TCP/IP context, pings www.google.com (AT+QPING), then does a
 * plain (non-TLS) HTTP GET of it over a raw TCP socket (AT+QIOPEN/
 * AT+QISEND/AT+QIRD/AT+QICLOSE) -- a bench sanity check for "does the
 * cellular link work at all", deliberately independent of the MQTT/TLS/
 * AT+QFUPL path in eg916q_mqtt_publish(), which is considerably more
 * complex and harder to isolate failures in. Reuses an already-running
 * session left by a prior eg916q_selftest() if there is one (see its doc
 * comment), otherwise powers on fresh. Always powers the modem back off
 * before returning, success or failure -- this call is meant to complete
 * the work, unlike eg916q_selftest()'s "leave it running for the next
 * call" handoff.
 *
 * min_rssi_dbm: minimum AT+CSQ signal level, in dBm, required before
 * attempting registration/QIACT at all (e.g. -93, the app's
 * CONFIG_CAR_TRACKER equivalent -- kMinSignalDbm -- lives in app_config.hpp,
 * not this driver, so the caller passes it in).
 * retry_interval_s: how long to sleep between AT+CSQ re-checks while signal
 * is below min_rssi_dbm. The module stays powered on throughout this wait
 * (see eg916q_wait_for_signal()'s doc comment in the .c file for why a
 * separate power-cycle-per-check design proved unreliable on real
 * hardware); there is no bound on the total wait.
 *
 * Returns 0 if both the ping and the HTTP GET succeeded, -ENOTCONN if
 * either did not (see the log for which), another negative errno on a
 * lower-level communication failure (e.g. registration timeout).
 */
int eg916q_network_selftest(const struct device *dev, int min_rssi_dbm,
			    uint32_t retry_interval_s);

struct eg916q_mqtt_publish_params {
	const char *broker_host;
	uint16_t broker_port;
	const char *client_id;
	/* Either may be NULL/empty if the broker doesn't require them. */
	const char *username;
	const char *password;
	const char *topic;
	/* NUL-terminated PEM text of the broker's CA certificate, uploaded to
	 * the module's own UFS filesystem fresh on every call (see
	 * eg916q_mqtt_publish()'s doc comment for why this isn't cached).
	 */
	const char *ca_cert_pem;
	const char *payload;
	uint16_t payload_len;
	/* See eg916q_network_selftest()'s doc comment above -- same meaning. */
	int min_rssi_dbm;
	uint32_t signal_retry_interval_s;
	/* Absolute k_uptime_get() time after which the call gives up with
	 * -ETIMEDOUT (and powers the module off), or 0 for no limit. Checked in
	 * the signal wait and between stages; a stage already running finishes.
	 */
	int64_t deadline_ms;
};

/*
 * One-shot power-on + full MQTT-over-TLS publish, using the module's own
 * embedded TCP/IP and MQTT AT-command stack (AT+QIACT/AT+QMTOPEN/
 * AT+QMTCONN/AT+QMTPUBEX/AT+QSSLCFG/AT+QFUPL) rather than PPP + a host-side
 * TLS/MQTT stack -- see this driver's header comment on why. Reuses an
 * already-running session left by a prior eg916q_selftest() if there is
 * one (see its doc comment), otherwise powers on fresh. On success the
 * module is left powered on and registered for the next publish (power it
 * off with eg916q_power_down()); on failure it is powered back off. Never
 * touches the PM_DEVICE action framework.
 *
 * The CA cert is re-uploaded to the module's UFS on every call rather than
 * checked-and-skipped-if-present: simpler and more robust (no cached
 * existence check to get out of sync with reality) at the cost of a small,
 * fixed AT+QFUPL transfer every publish -- negligible next to the
 * registration/QIACT/QMTOPEN network round trips this already pays for.
 *
 * Several of the AT command specifics here (AT+QSSLCFG's exact parameter
 * semantics, AT+QFUPL's upload protocol) are inferred from general Quectel
 * module conventions, not confirmed against this module's own SSL/FILE
 * application notes (not available at the time this was written) -- verify
 * against real hardware behavior, and check those docs if this needs
 * adjusting.
 *
 * Returns 0 once the broker acknowledges the publish (QMTPUBEX's async
 * result), a negative errno otherwise (e.g. -ENOTCONN on TCP/TLS open
 * failure, -ECONNREFUSED on MQTT CONNACK rejection, -ETIMEDOUT if any stage
 * doesn't respond in time).
 */
int eg916q_mqtt_publish(const struct device *dev, const struct eg916q_mqtt_publish_params *params);

/* Powers the module fully off (AT+QPOWD, blocking ~12 s for the shutdown to
 * finish) if this driver has it powered on; a no-op returning 0 otherwise.
 * Needed because several entry points deliberately leave the module running,
 * and the MCU must not enter System OFF with it still drawing current.
 * Must not race another entry point: the caller serialises (see
 * ct::modem::cell_active).
 */
int eg916q_power_down(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif /* CAR_TRACKER_DRIVERS_MODEM_QUECTEL_EG916Q_H_ */
