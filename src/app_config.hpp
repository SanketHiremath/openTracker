/*
 * Central compile-time configuration for the car tracker.
 *
 * Everything here is `constexpr` so tuning values are type-checked and cost
 * nothing at runtime. Thread stacks/priorities live here too, so the whole
 * concurrency layout can be reviewed in one place rather than being spread
 * across the task implementations.
 */

#pragma once

/* Log level for every ct_* module (LOG_MODULE_REGISTER's second argument).
 * Numeric because the logging macros need a preprocessor-time constant:
 * 3 == LOG_LEVEL_INF, 0 == LOG_LEVEL_NONE. Off via CONFIG_CAR_TRACKER_APP_LOGS=n.
 */
#if defined(CONFIG_CAR_TRACKER_APP_LOGS)
#define CT_LOG_LEVEL 3
#else
#define CT_LOG_LEVEL 0
#endif

#include <cstdint>

#include <zephyr/app_version.h>
#include <zephyr/kernel.h>

namespace ct::config {

/* ---------------------------------------------------------------------
 * Firmware version
 *
 * Sourced from the VERSION file at the project root (not this file) via
 * Zephyr's app_version.h generation -- bump VERSION there, not here.
 * Currently just logged at boot; the intent is for this to be published
 * in the telemetry payload and checked against the OTA server's advertised
 * version once OTA is implemented, so the device can report and identify
 * its own running image.
 * ------------------------------------------------------------------ */

inline constexpr const char *kFirmwareVersion = APP_VERSION_STRING;

/* ---------------------------------------------------------------------
 * Motion detection (consumed by gps_task.cpp's MotionDetector)
 * ------------------------------------------------------------------ */

/* Speed at or above which the car counts as moving, in mm/s (the unit of
 * navigation_data::speed). 1200 mm/s ~= 4.3 km/h: above standstill GPS
 * jitter, well below real driving speed.
 */
inline constexpr std::uint32_t kMovingSpeedThresholdMms = 1200;

/* Consecutive above-threshold fixes required before declaring motion.
 * Debounces a single noisy fix from waking the (expensive) modem.
 */
inline constexpr std::uint32_t kMovingDebounceCount = 3;

/* ---------------------------------------------------------------------
 * Timing
 * ------------------------------------------------------------------ */

/* Telemetry publish cadence (cellular_task.cpp's PublishScheduler).
 *
 * TEST MODE: 5 min, and kPublishRequireMoving (below) is false, so this
 * fires on a flat timer regardless of motion state -- for verifying the
 * MQTT pipeline end to end on the bench without needing to drive. The
 * temporary bench loop in cellular_thread_entry() also sleeps this long
 * after each publish attempt, so it sets the live cadence too.
 * Real anti-theft policy: gated on kPublishRequireMoving = true, so the
 * parked tracker stays silent (and the modem stays off) between trips.
 * Switch it when bench testing is done.
 */
inline constexpr k_timeout_t kMovingPublishInterval = K_MINUTES(5);

/* See kMovingPublishInterval's comment above -- false is the current test
 * setting; the real policy is true.
 */
inline constexpr bool kPublishRequireMoving = false;

/* How often the cellular task re-checks whether a publish is due. Only
 * needs to be fine-grained relative to kMovingPublishInterval (minutes),
 * not to anything watchdog-related -- the supervisor feeds the watchdog on
 * its own tick independently, so this thread getting delayed a few seconds
 * behind schedule is harmless.
 */
inline constexpr k_timeout_t kPublishSchedulerTick = K_SECONDS(5);

/* Supervisor tick: how often it wakes to feed the watchdog. Must stay
 * comfortably below kWatchdogTimeoutMs.
 */
inline constexpr k_timeout_t kSupervisorTick = K_SECONDS(1);

/* Minimum usable AT+CSQ signal level, in dBm, checked before attempting a
 * publish. -93 dBm corresponds to CSQ RSSI 10 (of 0-31): a commonly used
 * "fair or better" cutoff, comfortably above the noise floor where
 * registration/dial attempts tend to just time out anyway rather than fail
 * fast.
 */
inline constexpr int kMinSignalDbm = -93;

/* How long the cellular task waits before re-checking signal strength when
 * it's below kMinSignalDbm, or the modem couldn't be reached at all. Each
 * check is a full modem power cycle (see eg916q_get_signal_quality()'s doc
 * comment), so this isn't free -- it's a deliberate tradeoff of battery for
 * simplicity: no half-initialized modem state to reason about between
 * checks. Revisit if bench testing shows it needs to be cheaper.
 */
inline constexpr std::uint32_t kSignalRetryIntervalS = 30;

/* ---------------------------------------------------------------------
 * GPS power management
 * ------------------------------------------------------------------ */

/* Upper bound on a single ct::gps::standby() request (UBX-RXM-PMREQ
 * duration field). GPS ephemeris is only valid for ~4 hours after
 * broadcast; once it expires, the next fix degrades from a hot start to a
 * warm/cold one. This is kept far under that limit so the receiver's own
 * PMREQ timer wakes it on schedule even if nothing ever sends an explicit
 * wake -- ephemeris stays fresh, and the tracker never gets stuck asleep.
 */
inline constexpr std::uint32_t kGpsBackupMaxDurationMs = 30U * 60U * 1000U; /* 30 min */

/* ---------------------------------------------------------------------
 * Publish retry policy (cellular_task.cpp's PublishScheduler)
 * ------------------------------------------------------------------ */

inline constexpr std::uint32_t kPublishMaxAttempts = 3;
inline constexpr std::uint32_t kPublishRetryBaseDelayS = 5; /* 5s, 10s, 20s */

/* ---------------------------------------------------------------------
 * Watchdog
 * ------------------------------------------------------------------ */

/* Hardware watchdog timeout. This is the task watchdog's hardware fallback,
 * so it is set by CONFIG_TASK_WDT_MIN_TIMEOUT + CONFIG_TASK_WDT_HW_FALLBACK_DELAY
 * in the prj*.conf files; supervisor_task.cpp static_asserts the two agree.
 * The task watchdog feeds it from the kernel timer, so only a wedged kernel
 * (interrupts held off, timer ISR dead) lets it fire.
 */
inline constexpr std::uint32_t kWatchdogTimeoutMs = 10000;

/* ---------------------------------------------------------------------
 * IMU vibration analysis (consumed by imu_task.cpp and spectrum.cpp)
 * ------------------------------------------------------------------ */

/* Accelerometer sample rate for the FFT window. Must equal the sensor's output
 * data rate (CONFIG_LIS2DH_ODR_5 = 100 Hz in prj.conf; imu_task.cpp
 * static_asserts it): polling slower than the sensor drops samples, faster just
 * re-reads the same one. It also sets the ceiling -- nothing above half this
 * rate (50 Hz) is visible, and a faster vibration folds back down and is
 * reported at the wrong frequency. 100 Hz is chosen so an idling engine's
 * firing rate (~27 Hz for a 4-cylinder at 800 rpm) sits comfortably inside it;
 * the previous 50 Hz put it near the edge. A window is 256 samples = 2.56 s,
 * a frequency resolution of 0.39 Hz.
 */
inline constexpr std::uint32_t kImuVibrationSampleRateHz = 100;

/* Captures whose gravity-removed RMS (all axes) is below this are reported as
 * "no vibration" instead of being given a dominant frequency: the "peak" of
 * pure sensor noise is a random bin, and printing it as a frequency would be
 * misleading. 100 mm/s^2 is ~10 mg, a little over two 4 mg sensor LSBs, which
 * sits above a still board's jitter (successive readings differ by 1-2 LSB).
 * Lower it to see fainter vibration, at the price of sometimes calling noise a
 * tone; the peak-share figure in the log says how far to trust a weak result.
 * A starting value -- tune it against the real mounting.
 */
inline constexpr std::int32_t kImuVibrationMinRmsMms2 = 100;

/* ---------------------------------------------------------------------
 * Idle sleep (consumed by power_task.cpp)
 * ------------------------------------------------------------------ */

/* How long the accelerometer must stay quiet before the MCU goes to System
 * OFF. Measured from the last motion interrupt (or from boot). Shorter saves
 * battery sooner but makes a brief stop (traffic light, drive-through) cost a
 * full reboot, GPS re-acquisition and a modem cycle on the next movement.
 */
inline constexpr std::int64_t kIdleSleepMs = 5 * 60 * 1000;

/* How often to re-check when the idle timer has expired but sleeping has to
 * wait (self-test still running, firmware being staged). A modem transmission
 * is not polled: the power task waits on the cell_active mutex instead.
 */
inline constexpr std::int64_t kIdleSleepRetryMs = 1000;

/* Longest one transmission may hold cell_active (modem power-on, waiting for
 * signal, registration, MQTT publish). The driver gives up and powers the
 * modem off once this has passed, so a parked vehicle with no coverage
 * does not keep the device awake chasing a signal. Enforced between stages and
 * in the signal wait, so a single AT stage already in flight can run past it
 * (the slowest, AT+QIACT, has a 150 s timeout).
 */
inline constexpr std::int64_t kCellSessionMaxMs = 3 * 60 * 1000;

/* How long the power task waits for cell_active before giving up on this
 * sleep attempt: the session cap plus allowance for that one overrunning
 * stage. If it expires, the cellular task is stuck rather than slow.
 */
inline constexpr std::int64_t kCellLockWaitMs = kCellSessionMaxMs + 3 * 60 * 1000;

/* Accelerometer wake sensitivity while the MCU sleeps, as raw register values
 * (the INT1_THS and INT1_DUR registers of the LSM303DLHC/LIS2DH family).
 * Deliberately far less sensitive than the awake setting (about 33 mg for 1
 * sample at 100 Hz). Measured on this board: with the awake threshold, going
 * into System OFF made the sensor report a Z-axis (gravity axis) high event
 * within seconds, every time, and wake the device; with no wake pin armed the
 * sensor held that event latched afterwards, and with the MCU merely idling
 * instead of off it never fired. Raising the threshold stopped it. The cause
 * is NOT established: the first guess, the shared supply rail moving, does not
 * fit now that the IMU is known to have its own supply, so something else the
 * MCU powering down changes (ground return, the I2C/INT lines, the board's
 * regulator) is the likelier source. Treat this value as an empirical fix.
 *   threshold: 1 LSB = 16 mg at +/-2 g, so 6 = about 94 mg
 *   duration:  in samples at the sleeping 25 Hz rate, so 2 = 80 ms
 * Ignition cranking and driving are well above this; a gentle nudge is not.
 * Lower the threshold for more sensitivity until false wakes reappear.
 */
inline constexpr std::uint8_t kImuSleepIntThsReg = 6;
inline constexpr std::uint8_t kImuSleepIntDurReg = 2;

/* ---------------------------------------------------------------------
 * Task watchdog channels (see supervisor_task.hpp)
 *
 * Each monitored thread owns a channel and must feed it within its timeout or
 * the SoC is reset. Timeouts are sized to the longest legitimate stretch the
 * thread can go without reaching its feed point, with margin, so that only a
 * thread that is truly stuck (deadlocked, waiting on a dead peripheral) trips
 * one. Threads that block on something that can legitimately take long wake on
 * kTaskWdtPoll instead of waiting out the whole thing, purely to feed.
 * ------------------------------------------------------------------ */

inline constexpr std::uint32_t kSupervisorWdtMs = 5000; /* feeds every kSupervisorTick */

/* GPS: the receiver emits GGA/RMC every second, fix or no fix, so thirty
 * seconds without a single line means the receiver or its UART is dead. The
 * thread waits kGpsSampleWait per attempt and feeds when a sample arrived or,
 * with no sample (no time/fix yet), when a NMEA line was seen within that wait.
 */
inline constexpr std::uint32_t kGpsWdtMs = 30000;
inline constexpr k_timeout_t kGpsSampleWait = K_SECONDS(5);

/* BLE: event driven, and with no GPS fix there may be no events for a long
 * time, so this proves the thread is scheduled and not stuck in a call, not
 * that data is flowing. It wakes every kBleIdleWake to feed.
 */
inline constexpr std::uint32_t kBleWdtMs = 30000;
inline constexpr k_timeout_t kBleIdleWake = K_SECONDS(10);

/* Power: wakes at least once a minute (power_task.cpp). The slowest stretch
 * between feeds is the sleep sequence: modem power-down (~22 s), fed between
 * every step, with the cell_active wait fed every kTaskWdtPoll.
 */
inline constexpr std::uint32_t kPowerWdtMs = 120000;

/* Cellular: the longest legitimate stretch between feeds is one transmission,
 * kCellSessionMaxMs plus the one stage that may overrun it. That is exactly
 * what the power task allows for in kCellLockWaitMs, plus a minute of margin so
 * the power task gives up on a stuck session (and logs it) before the reset.
 * The wait for cell_active and the idle wait between publishes feed every
 * kTaskWdtPoll.
 */
inline constexpr std::uint32_t kCellularWdtMs = static_cast<std::uint32_t>(kCellLockWaitMs) + 60000U;

/* Longest a monitored thread blocks in one call when the wait can be long
 * (a mutex, a sleep between publishes) -- well inside every timeout above.
 */
inline constexpr std::int64_t kTaskWdtPollMs = 30 * 1000;

/* ---------------------------------------------------------------------
 * Threads
 *
 * Zephyr preemptive priorities: LOWER number == HIGHER priority. Assigned by
 * deadline-monotonic reasoning (short/urgent work high, long-blocking or
 * CPU-heavy work low), not by how "important" a task feels, so worst-case
 * response times can be reasoned about instead of leaning on round-robin
 * time slicing between same-priority tasks:
 *
 *   2  supervisor      must never be starved; the one thread that must
 *                      never be late (watchdog feed)
 *   3  LIS2DH trigger  (CONFIG_LIS2DH_THREAD_PRIORITY below) the motion
 *                      interrupt is the anti-theft system's primary event
 *                      and also restarts the sleep timer -- short, bounded
 *                      work (one I2C read + a zbus publish), so it belongs
 *                      near the top, not below every other app thread
 *   4  power           short work that must run promptly once the idle
 *                      timer fires
 *   5  gps             1 Hz input with a ~1 s deadline; a full queue drops
 *                      the oldest sample rather than stalling acquisition
 *   6  ble             event-driven, short, not time-critical
 *   9  cellular        long blocking network I/O; priority matters only
 *                      while it is runnable
 *  10  imu             CPU-heavy background FFT; bench probe, never
 *                      time-critical
 *  11  selftest LED    cosmetic
 * ------------------------------------------------------------------ */

inline constexpr int kSupervisorPriority = 2;
inline constexpr int kGpsPriority = 5;
inline constexpr int kBlePriority = 6;
inline constexpr int kCellularPriority = 9; /* lowest of the timing-sensitive tasks: long blocking network I/O */
inline constexpr int kImuPriority = 10; /* bench probe only: never time-critical */
inline constexpr int kPowerPriority = 4; /* idle-sleep bookkeeping: must react promptly when its timer fires */

inline constexpr std::size_t kSupervisorStackSize = 2048;
inline constexpr std::size_t kGpsStackSize = 2048;
inline constexpr std::size_t kBleStackSize = 2048;
/* imu_task.cpp: snprintk and sensor API frames plus the FFT call chain; the
 * large FFT buffers are static (spectrum.cpp), not on this stack. */
inline constexpr std::size_t kImuStackSize = 2048;
/* power_task.cpp: log formatting plus the shutdown sequence's call chain. */
inline constexpr std::size_t kPowerStackSize = 2048;
/* mbedTLS's X.509 parsing and RSA/ECDHE handshake math runs on this thread's
 * stack (only its large scratch buffers go to the mbedTLS heap via
 * CONFIG_MBEDTLS_ENABLE_HEAP) -- 4096 was fine for plaintext MQTT but is
 * tight to the point of likely overflow now that every publish does a full
 * TLS handshake. CONFIG_MPU_STACK_GUARD is on, so an overflow faults cleanly
 * (visible as a reset) rather than corrupting memory silently, but bumping
 * this is the actual fix, not just a mitigation.
 */
inline constexpr std::size_t kCellularStackSize = 8192; /* net stack + MQTT/TLS */

/* ---------------------------------------------------------------------
 * Queue depths
 * ------------------------------------------------------------------ */

/* Raw fixes buffered between the GNSS driver callback and the GPS task. */
inline constexpr std::uint32_t kGpsRawQueueDepth = 4;

/* zbus subscriber queue depths (in channel-pointer entries). Supervisor has
 * none: it doesn't subscribe to anything (see supervisor_task.cpp).
 */
inline constexpr std::uint32_t kCellularSubQueueDepth = 4;
inline constexpr std::uint32_t kBleSubQueueDepth = 4;

/* ---------------------------------------------------------------------
 * Self-test (boot-time GPS/cellular liveness check)
 * ------------------------------------------------------------------ */

/* How long to wait for the first NMEA GGA/RMC line from the GPS module before
 * declaring the GPS self-test check failed. Lines arrive ~1x/sec once the
 * receiver is powered, with or without time or a fix, so this is comfortably
 * clear of that. It checks the link only, not satellite reception.
 */
inline constexpr k_timeout_t kSelftestGpsTimeout = K_SECONDS(5);

/* Self-test success: a short blip, LED on for kSelftestHeartbeatOnTime then
 * off for kSelftestHeartbeatOffTime. Failure: a fast 50 % blink, one toggle
 * every kSelftestFailBlinkHalfPeriod (5 Hz).
 */
inline constexpr k_timeout_t kSelftestHeartbeatOnTime = K_MSEC(200);
inline constexpr k_timeout_t kSelftestHeartbeatOffTime = K_SECONDS(1);
inline constexpr k_timeout_t kSelftestFailBlinkHalfPeriod = K_MSEC(100);

inline constexpr int kSelftestLedPriority = 11; /* lowest of the app threads: cosmetic */
inline constexpr std::size_t kSelftestLedStackSize = 512;

} /* namespace ct::config */
