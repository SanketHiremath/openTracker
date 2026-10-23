/*
 * zbus channel declarations.
 *
 * Tasks communicate exclusively through these channels -- no task holds a
 * reference to another task, and there is no shared mutable state between
 * threads, so the design needs no application-level mutexes. Adding a new
 * consumer means subscribing to a channel, not editing the producer.
 */

#pragma once

#include <zephyr/zbus/zbus.h>

#include "messages.hpp"

/* Channels are C symbols emitted by the zbus macros; declare them outside
 * any namespace so the names match their definitions in channels.cpp.
 */

/* Every accepted GNSS sample (including "no fix" reports).
 * Published by: GPS task. Observed by: BLE. (Cellular reads it directly,
 * via zbus_chan_read(), when its own publish scheduler decides a publish is
 * due -- observing it would just wake the cellular thread on every fix for
 * no reason, since it only ever wants the *latest* value at its own pace.)
 */
ZBUS_CHAN_DECLARE(chan_gps_fix);

/* Debounced moving/parked verdict.
 * Published by: GPS task. Observed by: BLE, and (outside
 * CONFIG_CAR_TRACKER_GPS_ONLY) cellular -- it resets its retry/backoff state
 * when the car stops, and gates publishing on it if kPublishRequireMoving is
 * enabled (see app_config.hpp).
 */
ZBUS_CHAN_DECLARE(chan_motion_state);

#if defined(CONFIG_SENSOR)

/* Dominant vibration frequency of the latest accelerometer capture.
 * Published by: IMU task. Observed by: BLE.
 * Only exists when the sensor subsystem is on (CONFIG_SENSOR), i.e. not in
 * the GPS-only build.
 */
ZBUS_CHAN_DECLARE(chan_vibration);

#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)
/* Every accelerometer any-motion interrupt.
 * Published by: IMU task. Observed by: power task.
 */
ZBUS_CHAN_DECLARE(chan_imu_motion);
#endif

#endif /* CONFIG_SENSOR */

#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)

/* WAN link up/down.
 * Published by: cellular. Observed by: BLE, and the power task when
 * CONFIG_CAR_TRACKER_IDLE_SLEEP is on (it must not sleep mid-modem-session).
 */
ZBUS_CHAN_DECLARE(chan_link_state);

#endif /* !CONFIG_CAR_TRACKER_GPS_ONLY */
