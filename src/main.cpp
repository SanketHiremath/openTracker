/*
 * Car GPS anti-theft tracker -- entry point.
 *
 * Architecture
 * ------------
 * Four cooperating tasks exchanging immutable messages over zbus channels.
 * No task references another, and no application state is shared between
 * threads, so there are no application-level locks:
 *
 *      [GNSS driver cb] -> msgq -> (GPS task) --chan_gps_fix----+--> (BLE)
 *                                             --chan_motion_state-+-> (BLE)
 *                                                                 +-> (Cellular)
 *
 *      (Cellular) --chan_link_state--> (BLE)
 *
 *      (IMU) --chan_vibration--> (BLE)        [only with CONFIG_SENSOR]
 *      (IMU) --chan_imu_motion--> (Power)     [only with CONFIG_CAR_TRACKER_IDLE_SLEEP]
 *      (Cellular) ==cell_active mutex==> (Power)  held per modem session; Power
 *                                                 waits for it before sleeping
 *
 * Cellular reads chan_gps_fix directly (zbus_chan_read(), not a
 * subscription) when its own publish scheduler decides a publish is due --
 * see cellular_task.cpp. Supervisor touches no channel at all.
 *
 * Separation of concerns:
 *   GPS        - acquisition, plus its own motion debounce (it already has
 *                every fix in hand as it arrives)
 *   Cellular   - decides when to publish (cadence + retry/backoff) *and*
 *                executes it; owns all long blocking I/O
 *   BLE        - identification/status; never broadcasts location (see ble_task.cpp)
 *   Supervisor - hardware watchdog only; must stay responsive
 *
 * Memory: everything is statically allocated (thread stacks, queues, channel
 * storage). No heap, no exceptions, no RTTI -- see prj.conf.
 *
 * CONFIG_CAR_TRACKER_GPS_ONLY compiles out the cellular task and its services
 * entirely for hardware bring-up with only the GPS attached.
 *
 * A boot-time self-test (src/services/selftest.cpp) verifies the MCU can
 * talk to the GPS (NMEA presence) and cellular modem (AT/OK) before their
 * tasks do real work, signaling pass/fail on the onboard LED. It can block
 * main() for tens of seconds (modem power-on alone is up to ~20s, plus AT
 * retries), so the supervisor -- and therefore the watchdog -- is started
 * *before* it, not after, so the watchdog is live for the entire self-test
 * window instead of only once it finishes.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/reboot.h>

#include "app_config.hpp"
#include "services/fw_update.hpp"
#include "services/selftest.hpp"
#include "tasks/ble_task.hpp"
#include "tasks/gps_task.hpp"
#if defined(CONFIG_SENSOR)
#include "tasks/imu_task.hpp"
#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)
#include "tasks/power_task.hpp"
#endif
#endif
#include "tasks/supervisor_task.hpp"
#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
#include "tasks/cellular_task.hpp"
#endif

LOG_MODULE_REGISTER(ct_main, CT_LOG_LEVEL);

int main()
{
	LOG_INF("car_tracker starting (%s build) fw=%s",
		IS_ENABLED(CONFIG_CAR_TRACKER_GPS_ONLY) ? "GPS-only" : "full",
		ct::config::kFirmwareVersion);

	/* BLE first and unconditionally: it is an independent identification
	 * aid, so it must not be gated behind GPS or cellular coming up.
	 */
	if (const int err = ct::ble::start(); err != 0) {
		LOG_WRN("BLE unavailable (%d); continuing", err);
	}

	/* Started before the (blocking, potentially tens-of-seconds) self-test
	 * below so the watchdog is live and feeding for the entire self-test
	 * window, not just after it. Supervisor has no dependency on GPS/
	 * cellular being started first -- it doesn't touch zbus at all, only
	 * its own watchdog.
	 */
	ct::supervisor::start();

	/* Started before the self-test, not after: start() is what resumes the
	 * GNSS device (see the PM_DEVICE note there), and until that happens
	 * uart0 RX is never opened and the self-test's NMEA check could only
	 * ever time out. Starting the task early is safe -- its loop parks on
	 * the self-test gate, which is the whole point of gating the loop
	 * rather than the thread creation.
	 */
	if (const int err = ct::gps::start(); err != 0) {
		LOG_ERR("GPS unavailable (%d); tracking degraded", err);
	}

#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
	/* Same deal: the loop parks on the gate until the self-test passes. */
	ct::cellular::start();
#endif

#if defined(CONFIG_SENSOR)
	/* Temporary bench probe for the I2C motion sensors. Deliberately outside
	 * the self-test gate: a sensor that is absent or miswired must not be
	 * able to park GPS and cellular. See imu_task.cpp.
	 */
	ct::imu::start();
#endif

#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)
	/* After the IMU: it is the only wake source, and the power task refuses
	 * to sleep without it. Not gated by the self-test either, but it does
	 * wait for the self-test to finish before sleeping. See power_task.cpp.
	 */
	ct::power::start();
#endif

	/* Blocks until the GPS/cellular liveness check finishes, then releases
	 * the task loops above (or not, on failure -- see selftest.cpp).
	 */
	const bool healthy = ct::selftest::run();

	/* A/B update gate: a freshly updated image is only a *trial* until it
	 * confirms itself here. If the self-test failed we deliberately do not
	 * confirm -- MCUboot reverts to the previous image on the next reset
	 * (the watchdog, or a crash before this point, takes the same path).
	 * An already-confirmed image (e.g. the one flashed over SWD) is a no-op.
	 */
	if (healthy) {
		ct::fw_update::confirm_if_trial();
	} else if (ct::fw_update::is_trial()) {
		LOG_ERR("self-test failed on an unconfirmed image; rebooting to revert");
		k_sleep(K_MSEC(200));
		sys_reboot(SYS_REBOOT_COLD);
	}

	/* All work happens in the tasks above. main() has no runtime role, so
	 * let its thread terminate rather than burn a stack spinning.
	 */
	return 0;
}
