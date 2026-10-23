/*
 * zbus channel definitions.
 *
 * The observer lists here are the single authoritative description of which
 * task consumes what. Subscriber symbols themselves are defined by each task
 * (ZBUS_SUBSCRIBER_DEFINE); ZBUS_CHAN_DEFINE emits the extern declarations
 * for them, so no cross-task headers are needed.
 */

#include "channels.hpp"

#include "app_config.hpp"

ZBUS_CHAN_DEFINE(chan_gps_fix,          /* channel name */
		 ct::GpsFix,            /* message type */
		 nullptr,               /* validator */
		 nullptr,               /* user data */
		 ZBUS_OBSERVERS(ble_sub),
		 ZBUS_MSG_INIT(0));

#if defined(CONFIG_CAR_TRACKER_GPS_ONLY)
ZBUS_CHAN_DEFINE(chan_motion_state,
		 ct::MotionState,
		 nullptr,
		 nullptr,
		 ZBUS_OBSERVERS(ble_sub),
		 ZBUS_MSG_INIT(0));
#else
ZBUS_CHAN_DEFINE(chan_motion_state,
		 ct::MotionState,
		 nullptr,
		 nullptr,
		 ZBUS_OBSERVERS(ble_sub, cellular_sub),
		 ZBUS_MSG_INIT(0));

ZBUS_CHAN_DEFINE(chan_link_state,
		 ct::LinkState,
		 nullptr,
		 nullptr,
		 ZBUS_OBSERVERS(ble_sub),
		 ZBUS_MSG_INIT(0));

#endif /* !CONFIG_CAR_TRACKER_GPS_ONLY */

/* Orthogonal to the GPS-only split above: it exists whenever the sensor
 * subsystem (and therefore the IMU task) is built.
 */
#if defined(CONFIG_SENSOR)
ZBUS_CHAN_DEFINE(chan_vibration,
		 ct::VibrationState,
		 nullptr,
		 nullptr,
		 ZBUS_OBSERVERS(ble_sub),
		 ZBUS_MSG_INIT(0));

#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)
ZBUS_CHAN_DEFINE(chan_imu_motion,
		 ct::ImuMotion,
		 nullptr,
		 nullptr,
		 ZBUS_OBSERVERS(power_motion_lis),
		 ZBUS_MSG_INIT(0));
#endif
#endif /* CONFIG_SENSOR */
