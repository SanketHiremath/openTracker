/*
 * Boot-time peripheral self-test.
 *
 * Verifies the MCU can talk to the GPS and cellular modules before the
 * tasks that depend on them do any real work, and gives a single visible
 * pass/fail signal via the onboard LED. See docs/superpowers/specs/
 * 2026-09-24-boot-selftest-design.md for the full rationale.
 */

#include <cstdint>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../app_config.hpp"
#include "../tasks/gps_task.hpp"
#include "selftest.hpp"

#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
#include "../../drivers/modem/quectel_eg916q.h"
#endif

LOG_MODULE_REGISTER(ct_selftest, CT_LOG_LEVEL);

namespace {

const gpio_dt_spec kLed = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

K_SEM_DEFINE(gps_ready_sem, 0, 1);
K_SEM_DEFINE(cellular_ready_sem, 0, 1);

K_THREAD_STACK_DEFINE(led_stack, ct::config::kSelftestLedStackSize);
k_thread led_thread_data;
atomic_t led_started;
atomic_t run_finished;

void led_heartbeat_entry(void *ok_ptr, void *, void *)
{
	const bool ok = static_cast<bool>(reinterpret_cast<std::uintptr_t>(ok_ptr));
	if (ok) {
		for (;;) {
			gpio_pin_set_dt(&kLed, 1);
			k_sleep(ct::config::kSelftestHeartbeatOnTime);
			gpio_pin_set_dt(&kLed, 0);
			k_sleep(ct::config::kSelftestHeartbeatOffTime);
		}
	}

	for (;;) {
		gpio_pin_toggle_dt(&kLed);
		k_sleep(ct::config::kSelftestFailBlinkHalfPeriod);
	}
}

} /* namespace */

namespace ct::selftest {

bool run()
{
	int err = gpio_pin_configure_dt(&kLed, GPIO_OUTPUT_INACTIVE);

	if (err != 0) {
		LOG_ERR("failed to configure self-test LED gpio: %d", err);
	}

	LOG_INF("running boot self-test");

	const int gps_err = ct::gps::wait_for_first_sample(ct::config::kSelftestGpsTimeout);

	if (gps_err != 0) {
		LOG_ERR("self-test: no NMEA data from GPS (%d)", gps_err);
	} else {
		LOG_INF("self-test: GPS communication OK");
	}

#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
	const struct device *const modem_dev = DEVICE_DT_GET(DT_ALIAS(modem));
	const int cellular_err =
		device_is_ready(modem_dev) ? eg916q_selftest(modem_dev) : -ENODEV;

	if (cellular_err != 0) {
		LOG_ERR("self-test: cellular modem not responding (%d)", cellular_err);
	} else {
		LOG_INF("self-test: cellular modem communication OK");
	}
#else
	const int cellular_err = 0;
#endif

	const bool ok = (gps_err == 0) && (cellular_err == 0);

	if (ok) {
		LOG_INF("self-test passed; releasing GPS/cellular tasks");
		k_sem_give(&gps_ready_sem);
#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
		k_sem_give(&cellular_ready_sem);
#endif
	} else {
		LOG_ERR("self-test failed; affected task(s) will not run");
	}

	k_thread_create(&led_thread_data, led_stack, K_THREAD_STACK_SIZEOF(led_stack),
			led_heartbeat_entry, reinterpret_cast<void *>(static_cast<std::uintptr_t>(ok)),
			nullptr, nullptr, ct::config::kSelftestLedPriority, 0, K_NO_WAIT);
	k_thread_name_set(&led_thread_data, "selftest_led");
	atomic_set(&led_started, 1);
	atomic_set(&run_finished, 1);

	return ok;
}

bool finished()
{
	return atomic_get(&run_finished) != 0;
}

void led_off()
{
	if (atomic_get(&led_started) != 0) {
		k_thread_abort(&led_thread_data);
		atomic_set(&led_started, 0);
	}
	(void)gpio_pin_set_dt(&kLed, 0);
}

void wait_gps_ready()
{
	k_sem_take(&gps_ready_sem, K_FOREVER);
}

void wait_cellular_ready()
{
	k_sem_take(&cellular_ready_sem, K_FOREVER);
}

} /* namespace ct::selftest */
