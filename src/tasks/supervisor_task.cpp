/*
 * Supervisor task.
 *
 * Responsibility: feed the hardware watchdog. Nothing else. Motion debounce
 * moved to the GPS task (it already has every fix in hand as it arrives --
 * see gps_task.cpp), and publish scheduling/retry moved to the cellular
 * task (it already owns execution -- see cellular_task.cpp). This task no
 * longer touches zbus at all: the one thing it must never do is get wedged
 * waiting on anything, and the simplest way to guarantee that is to give it
 * nothing to wait on except its own tick.
 *
 * Watchdog rationale: an anti-theft tracker sits unattended in a parked car.
 * A silently wedged firmware is indistinguishable from a stolen-and-jammed
 * device, so a stuck supervisor reboots the SoC rather than going quiet.
 */

#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../app_config.hpp"
#include "supervisor_task.hpp"

LOG_MODULE_REGISTER(ct_supervisor, CT_LOG_LEVEL);

namespace {

K_THREAD_STACK_DEFINE(supervisor_stack, ct::config::kSupervisorStackSize);
k_thread supervisor_thread_data;

/* ------------------------------------------------------------------
 * Watchdog
 *
 * Optional (CONFIG_WATCHDOG). Worth knowing on nRF52: the WDT cannot be
 * stopped once started and keeps running across a soft reset -- including
 * the SYSRESETREQ that `west flash` issues -- so a stale watchdog can
 * survive reflashes and only a real power cycle (POR) clears it. Debug and
 * bring-up builds therefore leave it off.
 * ---------------------------------------------------------------- */

#if defined(CONFIG_WATCHDOG)

const struct device *const kWdtDev = DEVICE_DT_GET(DT_ALIAS(watchdog0));
int wdt_channel = -1;

void watchdog_init()
{
	if (!device_is_ready(kWdtDev)) {
		LOG_ERR("watchdog device not ready; running unprotected");
		return;
	}

	wdt_timeout_cfg cfg{};
	cfg.window.min = 0U;
	cfg.window.max = ct::config::kWatchdogTimeoutMs;
	cfg.callback = nullptr;
	cfg.flags = WDT_FLAG_RESET_SOC;

	wdt_channel = wdt_install_timeout(kWdtDev, &cfg);
	if (wdt_channel < 0) {
		LOG_ERR("wdt_install_timeout failed: %d", wdt_channel);
		return;
	}

	/* PAUSE_HALTED_BY_DBG keeps a debugger halt from tripping a reset --
	 * without it, every breakpoint reboots the board.
	 */
	const int err = wdt_setup(kWdtDev, WDT_OPT_PAUSE_HALTED_BY_DBG);

	if (err != 0) {
		LOG_ERR("wdt_setup failed: %d", err);
		wdt_channel = -1;
		return;
	}

	LOG_INF("watchdog armed (%u ms)", ct::config::kWatchdogTimeoutMs);
}

inline void watchdog_feed()
{
	if (wdt_channel >= 0) {
		(void)wdt_feed(kWdtDev, wdt_channel);
	}
}

#else /* !CONFIG_WATCHDOG */

void watchdog_init()
{
	LOG_WRN("watchdog disabled in this build; running unprotected");
}

inline void watchdog_feed() {}

#endif /* CONFIG_WATCHDOG */

void supervisor_thread_entry(void *, void *, void *)
{
	LOG_INF("supervisor task started");

	watchdog_init();

	for (;;) {
		k_sleep(ct::config::kSupervisorTick);
		watchdog_feed();
	}
}

} /* namespace */

namespace ct::supervisor {

void start()
{
	k_thread_create(&supervisor_thread_data, supervisor_stack,
			K_THREAD_STACK_SIZEOF(supervisor_stack), supervisor_thread_entry, nullptr,
			nullptr, nullptr, ct::config::kSupervisorPriority, 0, K_NO_WAIT);
	k_thread_name_set(&supervisor_thread_data, "supervisor");
}

} /* namespace ct::supervisor */
