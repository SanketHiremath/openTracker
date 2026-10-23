/*
 * Cellular link management: modem power + native AT-command TCP/IP/MQTT.
 *
 * Thin wrapper over the out-of-tree quectel_eg916q driver's self-contained
 * power-on-to-power-off entry points. Kept free of policy: it knows how to
 * check signal and publish, not when to -- that's cellular_task.cpp/
 * supervisor_task.cpp's job.
 */

#include <cstring>

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../../drivers/modem/quectel_eg916q.h"
#include "../app_config.hpp"
#include "thingspeak_ca_cert.hpp"
#include "modem_link.hpp"

LOG_MODULE_REGISTER(ct_modem, CT_LOG_LEVEL);

K_MUTEX_DEFINE(cell_active);

namespace {

const struct device *const kModemDev = DEVICE_DT_GET(DT_ALIAS(modem));

} /* namespace */

namespace ct::modem {

int network_selftest()
{
	if (!device_is_ready(kModemDev)) {
		LOG_ERR("modem device not ready");
		return -ENODEV;
	}

	return eg916q_network_selftest(kModemDev, ct::config::kMinSignalDbm,
					ct::config::kSignalRetryIntervalS);
}

int check_signal(int *rssi_dbm)
{
	if (!device_is_ready(kModemDev)) {
		LOG_ERR("modem device not ready");
		return -ENODEV;
	}

	return eg916q_get_signal_quality(kModemDev, rssi_dbm);
}

int ping()
{
	if (!device_is_ready(kModemDev)) {
		LOG_ERR("modem device not ready");
		return -ENODEV;
	}

	return eg916q_ping(kModemDev, ct::config::kMinSignalDbm, ct::config::kSignalRetryIntervalS);
}

int http_get()
{
	if (!device_is_ready(kModemDev)) {
		LOG_ERR("modem device not ready");
		return -ENODEV;
	}

	return eg916q_http_get(kModemDev, ct::config::kMinSignalDbm,
				ct::config::kSignalRetryIntervalS);
}

int publish_telemetry(std::span<const char> payload, std::int64_t deadline_ms)
{
	if (!device_is_ready(kModemDev)) {
		LOG_ERR("modem device not ready");
		return -ENODEV;
	}

	const eg916q_mqtt_publish_params params = {
		.broker_host = CONFIG_CAR_TRACKER_MQTT_BROKER_HOSTNAME,
		.broker_port = CONFIG_CAR_TRACKER_MQTT_BROKER_PORT,
		.client_id = CONFIG_CAR_TRACKER_MQTT_CLIENT_ID,
		.username = CONFIG_CAR_TRACKER_MQTT_USERNAME,
		.password = CONFIG_CAR_TRACKER_MQTT_PASSWORD,
		.topic = CONFIG_CAR_TRACKER_MQTT_TOPIC,
		.ca_cert_pem = ct::mqtt::kThingspeakCaCertificate,
		.payload = payload.data(),
		.payload_len = static_cast<std::uint16_t>(payload.size()),
		.min_rssi_dbm = ct::config::kMinSignalDbm,
		.signal_retry_interval_s = ct::config::kSignalRetryIntervalS,
		.deadline_ms = deadline_ms,
	};

	return eg916q_mqtt_publish(kModemDev, &params);
}

int power_down()
{
	if (!device_is_ready(kModemDev)) {
		return -ENODEV;
	}

	return eg916q_power_down(kModemDev);
}

} /* namespace ct::modem */
