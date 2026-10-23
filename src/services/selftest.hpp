#pragma once

namespace ct::selftest {

/* Runs the boot-time self-test: verifies the MCU can talk to both the GPS
 * (NMEA presence on uart0 -- see ct::gps::wait_for_first_sample()) and the
 * cellular modem (AT/OK on uart1 -- see eg916q_selftest() in the modem
 * driver). Blocking; call once from main() before starting the GPS/
 * cellular tasks.
 *
 * On success: starts a 1 Hz LED heartbeat (forever) and releases both
 * wait_gps_ready() and wait_cellular_ready() below.
 *
 * On failure: logs which check(s) failed, starts a fast 5 Hz LED blink
 * (forever) instead, and releases neither -- so the corresponding task(s)
 * stay parked before doing any peripheral I/O. BLE and the supervisor
 * (watchdog) are unaffected either way; only run() by itself, before this
 * function returns.
 *
 * Returns true if every check passed. main() uses this to decide whether a
 * freshly updated firmware image may be confirmed (see fw_update.hpp).
 */
bool run();

/* True once run() has finished (pass or fail). The power task will not put
 * the device to sleep before this, so a slow self-test is never cut short.
 */
bool finished();

/* Stops the heartbeat/fail blink thread and leaves the LED off. For the run-up
 * to System OFF: a latched-on LED would dominate the sleep current. Safe to
 * call whether or not run() got as far as starting the blink thread.
 */
void led_off();

/* Blocks until run() has confirmed GPS communication is OK. Call once, at
 * the top of the GPS task's thread entry, before its main loop.
 */
void wait_gps_ready();

/* Blocks until run() has confirmed cellular modem communication is OK.
 * Call once, at the top of the cellular task's thread entry, before its
 * main loop.
 */
void wait_cellular_ready();

} /* namespace ct::selftest */
