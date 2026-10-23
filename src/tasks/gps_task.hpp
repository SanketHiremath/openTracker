#pragma once

#include <zephyr/kernel.h>

namespace ct::gps {

/* Verifies the GNSS device and starts the acquisition thread.
 * Returns 0 on success, -ENODEV if the GNSS device is not ready.
 */
int start();

/* Blocks until the first NMEA sentence (GGA/RMC, fix or no-fix) has been
 * observed on uart0, or `timeout` elapses. Independent of start()/the
 * normal fix pipeline -- used by the boot self-test to verify the MCU can
 * talk to the GPS module before anything else touches it. Returns 0 if a
 * sentence arrived in time, -EAGAIN on timeout (see k_sem_take()).
 */
int wait_for_first_sample(k_timeout_t timeout);

/* Requests UBX backup mode (RXM-PMREQ, bounded duration -- see
 * kGpsBackupMaxDurationMs) over uart0. Cuts the NEO-6M's active current draw
 * while leaving VCC applied, so battery-backed RAM (ephemeris/almanac/last
 * fix) survives. The bounded duration keeps that ephemeris within its
 * validity window (~4 hours) even if wake() is never called -- the receiver
 * times out of backup mode on its own -- so the next fix is always a hot
 * start, never a cold one. No fixes will arrive on chan_gps_fix until the
 * receiver leaves backup mode (by wake() or its own timeout); callers
 * decide policy (e.g. calling this once the car has been parked for a
 * while) -- this function only knows how to talk to the hardware. Not
 * currently called by anything -- reserved for whichever task ends up
 * owning parked-power policy.
 */
void standby();

/* Like standby() but with no time limit (RXM-PMREQ duration 0): the receiver
 * stays in backup mode until wake() or until it sees UART RX activity. For
 * the power task's System OFF sleep, where nothing is running to call wake()
 * and a bounded request would have the receiver wake itself every
 * kGpsBackupMaxDurationMs and run at full current for as long as the MCU
 * sleeps. The cost is that the ephemeris eventually goes stale, so the first
 * fix after a long sleep is a warm start (tens of seconds), not a hot one.
 */
void standby_until_wake();

/* Wakes the receiver from standby() early by nudging UART RX with dummy
 * bytes -- see the implementation comment for why (no EXTINT/RESET line
 * wired) and its caveats. Fixes resume arriving on chan_gps_fix once the
 * receiver has reacquired, which is fast (hot start) as long as standby()'s
 * bounded duration hasn't elapsed.
 */
void wake();

} /* namespace ct::gps */
