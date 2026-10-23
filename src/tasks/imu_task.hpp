#pragma once

namespace ct::imu {

/* Starts the bench bring-up probe thread (see imu_task.cpp). Only compiled
 * and called when CONFIG_SENSOR is on. Never fails: a sensor that did not
 * come up is reported in the log and skipped, and the probe is deliberately
 * outside the boot self-test gate (see main.cpp) so a missing or miswired
 * sensor cannot park GPS and cellular.
 */
void start();

#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)

/* Prepares the motion sensors for System OFF and arms INT1 as the wake
 * source. Blocking (a few seconds): parks the probe thread, puts the
 * magnetometer (and gyro, if present) to sleep, drops the accelerometer to a
 * 25 Hz low-power rate, then waits for INT1 to be low and stable -- the wake
 * input is level sensitive, so a high line would wake the device immediately.
 *
 * Returns 0 only when the sensors are in their sleep configuration and INT1
 * has been seen quiet. The wake pin itself is NOT armed yet: see
 * arm_wake_source(). On any
 * failure (no accelerometer, I/O error, a line that will not settle because
 * the board is still moving) it restores everything and returns a negative
 * errno, and the caller must not sleep. After success, the caller either
 * goes to System OFF or calls cancel_sleep().
 */
int prepare_for_sleep();

/* Final step before sys_poweroff(): re-clears the accelerometer's latch,
 * waits for INT1 to be low again, and arms it as the System OFF wake input.
 * Call it after every other shutdown action (GPS to backup, USB detach, ...)
 * so their supply-current steps cannot leave a spurious interrupt behind.
 * No return value: if INT1 will not settle the board is moving, and waking
 * immediately is correct. Not undoable; follow it with sys_poweroff().
 */
void arm_wake_source();

/* Undoes prepare_for_sleep(): restores the sensor settings and resumes the
 * probe thread. Also called internally on failure.
 */
void cancel_sleep();

#endif /* CONFIG_CAR_TRACKER_IDLE_SLEEP */

} /* namespace ct::imu */
