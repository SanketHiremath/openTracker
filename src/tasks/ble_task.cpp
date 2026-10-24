/*
 * BLE task.
 *
 * Responsibility: own the Bluetooth stack lifecycle and advertise the device
 * as CONFIG_BT_DEVICE_NAME ("car"), plus track system status for local
 * diagnostics.
 *
 * SECURITY / PRIVACY -- deliberate design choice:
 * This task subscribes to GPS fixes but never places position, speed, or any
 * derived location data into the advertising payload. BLE advertising is
 * unauthenticated broadcast: anything put there is readable by anyone in
 * range. On an anti-theft tracker that would hand a thief (or anyone
 * wardriving) a way to locate and follow the vehicle -- inverting the
 * product's purpose. That holds unconditionally: the advertising data below
 * is the device name and flags, nothing else.
 *
 * Telemetry over GATT (CONFIG_CAR_TRACKER_BLE_TELEMETRY, off unless enabled)
 * -------------------------------------------------------------------------
 * A deliberate, gated relaxation so GPS speed and the accelerometer's
 * dominant vibration frequency can be read from a paired phone. It is not
 * open: both characteristics, and subscribing to their notifications, require
 * an *authenticated, encrypted* link -- LE passkey pairing with a fixed
 * passkey (CONFIG_CAR_TRACKER_BLE_PASSKEY) -- so a stranger who merely
 * connects gets "insufficient authentication". The protection is exactly as
 * strong as that passkey is secret, and the default is a placeholder.
 * Position (latitude/longitude) is never exposed this way; only speed.
 *
 * Service f6f6a800-588c-48d4-a10b-2a45966c0ecc, two read+notify
 * characteristics, each a little-endian uint32:
 *   f6f6a801-...  GPS speed in mm/s (divide by 277.78 for km/h);
 *                 0xFFFFFFFF = no GPS fix
 *   f6f6a802-...  dominant vibration frequency in millihertz (27000 = 27 Hz);
 *                 0xFFFFFFFF = too still to name a frequency / no data yet
 * Values update as new GPS fixes (~1 Hz) and vibration captures (~every 2.6 s)
 * arrive; a subscribed client is notified on each update.
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/zbus/zbus.h>

#if defined(CONFIG_CAR_TRACKER_BLE_TELEMETRY)
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#endif

#include "../app_config.hpp"
#include "../channels.hpp"
#include "../messages.hpp"
#include "ble_task.hpp"
#include "supervisor_task.hpp"

LOG_MODULE_REGISTER(ct_ble, CT_LOG_LEVEL);

/* External linkage required: channels.cpp references this observer. */
ZBUS_SUBSCRIBER_DEFINE(ble_sub, ct::config::kBleSubQueueDepth);

namespace {

constexpr const char *kDeviceName = CONFIG_BT_DEVICE_NAME;
constexpr std::size_t kDeviceNameLen = sizeof(CONFIG_BT_DEVICE_NAME) - 1;

/* Name lives in the primary advertising payload so the device is
 * identifiable in a scan list without a scan-request round trip.
 * Note the absence of any telemetry here -- see the header comment.
 */
const bt_data kAdvData[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, kDeviceName, kDeviceNameLen),
};

#if defined(CONFIG_CAR_TRACKER_BLE_TELEMETRY)

/* Sent when a value has nothing meaningful to say (no fix / too still / no
 * data yet), so a client can tell "unavailable" from a genuine zero.
 */
constexpr std::uint32_t kUnavailable = 0xFFFFFFFFU;

const bt_uuid_128 kSvcUuid =
	BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xf6f6a800, 0x588c, 0x48d4, 0xa10b, 0x2a45966c0ecc));
const bt_uuid_128 kSpeedUuid =
	BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xf6f6a801, 0x588c, 0x48d4, 0xa10b, 0x2a45966c0ecc));
const bt_uuid_128 kVibrationUuid =
	BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xf6f6a802, 0x588c, 0x48d4, 0xa10b, 0x2a45966c0ecc));

/* Current characteristic values, already in wire (little-endian) order.
 * Written only by the BLE thread; read by the Bluetooth host thread from the
 * read callback. A naturally aligned 32-bit store is a single instruction on
 * Cortex-M, so a read never sees a half-written value and no lock is needed.
 */
std::uint32_t speed_le = sys_cpu_to_le32(kUnavailable);
std::uint32_t vibration_le = sys_cpu_to_le32(kUnavailable);

ssize_t read_u32(bt_conn *conn, const bt_gatt_attr *attr, void *buf, uint16_t len,
		 uint16_t offset)
{
	const auto *value = static_cast<const std::uint32_t *>(attr->user_data);

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(*value));
}

void ccc_changed(const bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);
	LOG_INF("telemetry notifications %s", value == BT_GATT_CCC_NOTIFY ? "on" : "off");
}

/* The GATT macros build these as C compound literals (&(struct ...){...}),
 * which C++ rejects, so the characteristic declarations and CCC state are
 * spelled out as ordinary objects and the attribute table is assembled with
 * the lower-level BT_GATT_ATTRIBUTE().
 */
bt_gatt_chrc speed_chrc = {
	.uuid = &kSpeedUuid.uuid,
	.value_handle = 0U,
	.properties = BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
};
bt_gatt_chrc vibration_chrc = {
	.uuid = &kVibrationUuid.uuid,
	.value_handle = 0U,
	.properties = BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
};
bt_gatt_ccc_managed_user_data speed_ccc =
	BT_GATT_CCC_MANAGED_USER_DATA_INIT(ccc_changed, nullptr, nullptr);
bt_gatt_ccc_managed_user_data vibration_ccc =
	BT_GATT_CCC_MANAGED_USER_DATA_INIT(ccc_changed, nullptr, nullptr);

/* AUTHEN, not just ENCRYPT: encryption alone is satisfied by "Just Works"
 * pairing, which any phone can complete silently with no secret. AUTHEN
 * demands the passkey exchange. The CCC is protected too, or a stranger could
 * subscribe to notifications without ever reading.
 */
constexpr uint16_t kValuePerm = BT_GATT_PERM_READ_AUTHEN;
constexpr uint16_t kCccPerm = BT_GATT_PERM_READ_AUTHEN | BT_GATT_PERM_WRITE_AUTHEN;

bt_gatt_attr telemetry_attrs[] = {
	BT_GATT_PRIMARY_SERVICE(&kSvcUuid),

	BT_GATT_ATTRIBUTE(BT_UUID_GATT_CHRC, BT_GATT_PERM_READ, bt_gatt_attr_read_chrc, nullptr,
			  &speed_chrc),
	BT_GATT_ATTRIBUTE(&kSpeedUuid.uuid, kValuePerm, read_u32, nullptr, &speed_le),
	BT_GATT_CCC_MANAGED(&speed_ccc, kCccPerm),

	BT_GATT_ATTRIBUTE(BT_UUID_GATT_CHRC, BT_GATT_PERM_READ, bt_gatt_attr_read_chrc, nullptr,
			  &vibration_chrc),
	BT_GATT_ATTRIBUTE(&kVibrationUuid.uuid, kValuePerm, read_u32, nullptr, &vibration_le),
	BT_GATT_CCC_MANAGED(&vibration_ccc, kCccPerm),
};

STRUCT_SECTION_ITERABLE(bt_gatt_service_static, telemetry_svc) = BT_GATT_SERVICE(telemetry_attrs);

/* Updates a characteristic and, if it changed, notifies any subscribed client.
 * bt_gatt_notify_uuid() with a null connection fans out to every subscriber
 * and simply returns an error when there is none -- not worth logging.
 */
void publish(std::uint32_t &stored_le, const bt_uuid_128 &uuid, std::uint32_t host_value)
{
	const std::uint32_t wire = sys_cpu_to_le32(host_value);

	if (wire == stored_le) {
		return;
	}

	stored_le = wire;
	(void)bt_gatt_notify_uuid(nullptr, &uuid.uuid, telemetry_attrs, &stored_le,
				  sizeof(stored_le));
}

void on_connected(bt_conn *conn, uint8_t err)
{
	ARG_UNUSED(conn);
	LOG_INF("BLE connected%s", err != 0 ? " (failed)" : "");
}

void on_disconnected(bt_conn *conn, uint8_t reason)
{
	ARG_UNUSED(conn);
	LOG_INF("BLE disconnected (reason 0x%02x)", reason);
}

/* A connection ending stops connectable advertising, and the controller does
 * not resume it on its own. Nothing noticed before because nothing used
 * connections; now a phone that connects once would otherwise never see the
 * device again until reset. `recycled` is the point at which the connection
 * object is free and advertising may be restarted.
 */
void restart_advertising();

void on_recycled()
{
	restart_advertising();
}

void on_security_changed(bt_conn *conn, bt_security_t level, bt_security_err err)
{
	ARG_UNUSED(conn);

	if (err == BT_SECURITY_ERR_SUCCESS) {
		LOG_INF("BLE security level %d", static_cast<int>(level));
	} else {
		LOG_WRN("BLE pairing/security failed (%d)", static_cast<int>(err));
	}
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = on_connected,
	.disconnected = on_disconnected,
	.recycled = on_recycled,
	.security_changed = on_security_changed,
};

#endif /* CONFIG_CAR_TRACKER_BLE_TELEMETRY */

K_THREAD_STACK_DEFINE(ble_stack, ct::config::kBleStackSize);
k_thread ble_thread_data;

/* Non-sensitive local status, touched only by this thread. */
struct Status {
	bool has_fix;
	bool moving;
	bool link_up;
};

Status status{};

void ble_thread_entry(void *, void *, void *)
{
	LOG_INF("BLE task started");

	const int wdt = ct::supervisor::watch(ct::config::kBleWdtMs);

	for (;;) {
		const zbus_channel *chan = nullptr;

		/* Events can be rare (no fix, nothing moving), so wake periodically
		 * just to feed: this proves the thread is not stuck in a call below,
		 * not that data is flowing.
		 */
		const int wait_err = zbus_sub_wait(&ble_sub, &chan, ct::config::kBleIdleWake);

		ct::supervisor::feed(wdt);

		if (wait_err != 0) {
			continue;
		}

		if (chan == &chan_gps_fix) {
			ct::GpsFix fix{};

			if (zbus_chan_read(&chan_gps_fix, &fix, K_MSEC(50)) == 0) {
				if (status.has_fix != fix.valid) {
					status.has_fix = fix.valid;
					LOG_INF("status: fix=%s", fix.valid ? "yes" : "no");
				}
#if defined(CONFIG_CAR_TRACKER_BLE_TELEMETRY)
				publish(speed_le, kSpeedUuid, fix.valid ? fix.speed_mms : kUnavailable);
#endif
			}
		} else if (chan == &chan_motion_state) {
			ct::MotionState motion{};

			if (zbus_chan_read(&chan_motion_state, &motion, K_MSEC(50)) == 0) {
				status.moving = motion.moving;
				LOG_INF("status: %s", motion.moving ? "MOVING" : "parked");
			}
		}
#if defined(CONFIG_CAR_TRACKER_BLE_TELEMETRY) && defined(CONFIG_SENSOR)
		else if (chan == &chan_vibration) {
			ct::VibrationState vib{};

			if (zbus_chan_read(&chan_vibration, &vib, K_MSEC(50)) == 0) {
				publish(vibration_le, kVibrationUuid,
					vib.significant ? vib.freq_mhz : kUnavailable);
			}
		}
#endif
#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
		else if (chan == &chan_link_state) {
			ct::LinkState link{};

			if (zbus_chan_read(&chan_link_state, &link, K_MSEC(50)) == 0) {
				status.link_up = link.connected;
				LOG_INF("status: link=%s", link.connected ? "up" : "down");
			}
		}
#endif
	}
}

} /* namespace */

namespace {

#if defined(CONFIG_CAR_TRACKER_BLE_TELEMETRY)
void restart_advertising()
{
	const int err =
		bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, kAdvData, ARRAY_SIZE(kAdvData), nullptr, 0);

	if (err != 0) {
		LOG_ERR("advertising failed to restart (%d)", err);
	}
}
#endif

} /* namespace */

namespace ct::ble {

int start()
{
	int err = bt_enable(nullptr);

	if (err != 0) {
		LOG_ERR("bluetooth init failed (%d)", err);
		return err;
	}

#if defined(CONFIG_CAR_TRACKER_BLE_TELEMETRY)
	/* With a fixed passkey set, Zephyr reports "display only" I/O capability
	 * even with no auth callbacks registered, which forces passkey-entry
	 * pairing: the phone prompts the user to type this code. The value is
	 * deliberately never logged.
	 */
	err = bt_passkey_set(CONFIG_CAR_TRACKER_BLE_PASSKEY);
	if (err != 0) {
		LOG_ERR("failed to set pairing passkey (%d)", err);
		return err;
	}
#endif

	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, kAdvData, ARRAY_SIZE(kAdvData), nullptr, 0);
	if (err != 0) {
		LOG_ERR("advertising failed to start (%d)", err);
		return err;
	}

	LOG_INF("advertising as \"%s\"", kDeviceName);

	k_thread_create(&ble_thread_data, ble_stack, K_THREAD_STACK_SIZEOF(ble_stack),
			ble_thread_entry, nullptr, nullptr, nullptr,
			ct::config::kBlePriority, 0, K_NO_WAIT);
	k_thread_name_set(&ble_thread_data, "ble");

	return 0;
}

} /* namespace ct::ble */
