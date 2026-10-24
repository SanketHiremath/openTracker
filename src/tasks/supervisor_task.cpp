/*
 * Supervisor task.
 *
 * Responsibility: own the watchdog. Nothing else. Motion debounce
 * moved to the GPS task (it already has every fix in hand as it arrives --
 * see gps_task.cpp), and publish scheduling/retry moved to the cellular
 * task (it already owns execution -- see cellular_task.cpp). This task no
 * longer touches zbus at all: the one thing it must never do is get wedged
 * waiting on anything, and the simplest way to guarantee that is to give it
 * nothing to wait on except its own tick.
 *
 * Watchdog rationale: an anti-theft tracker sits unattended in a parked car.
 * A silently wedged firmware is indistinguishable from a stolen-and-jammed
 * device, so a stuck task reboots the SoC rather than going quiet. The
 * supervisor itself is just one more monitored channel, fed from its tick;
 * the other tasks register theirs with watch() and feed from their own loops,
 * which is what catches a task that hangs while everything else carries on.
 */

#include <zephyr/device.h>
#include <zephyr/task_wdt/task_wdt.h>
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
 * Optional (CONFIG_TASK_WDT). The Zephyr task watchdog multiplexes the one
 * hardware watchdog into a channel per task: every monitored task registers
 * with watch() and feeds its channel from its own loop, so a task that stops
 * making progress -- deadlocked, or waiting on a peripheral that died -- is
 * noticed even though the supervisor thread itself keeps running. An expired
 * channel resets the SoC (from the kernel timer, so it works whatever the
 * stuck thread is doing). The hardware watchdog behind it is fed by that same
 * timer and only fires if the kernel itself wedges.
 *
 * Worth knowing on nRF52: the WDT cannot be stopped once started and keeps
 * running across a soft reset -- including the SYSRESETREQ that `west flash`
 * issues -- so a stale watchdog can survive reflashes and only a real power
 * cycle (POR) clears it. Debug and bring-up builds therefore leave it off.
 * ---------------------------------------------------------------- */

#if defined(CONFIG_TASK_WDT)

static_assert(ct::config::kWatchdogTimeoutMs ==
		      CONFIG_TASK_WDT_MIN_TIMEOUT + CONFIG_TASK_WDT_HW_FALLBACK_DELAY,
	      "kWatchdogTimeoutMs must match the task watchdog's hardware fallback timeout");

const struct device *const kWdtDev = DEVICE_DT_GET(DT_ALIAS(watchdog0));

/* Set once by start(), before any other task exists to read it. */
atomic_t wdt_ready;
int supervisor_channel = -1;

void watchdog_init()
{
	if (!device_is_ready(kWdtDev)) {
		LOG_ERR("watchdog device not ready; running unprotected");
		return;
	}

	if (const int err = task_wdt_init(kWdtDev); err != 0) {
		LOG_ERR("task_wdt_init failed: %d", err);
		return;
	}

	atomic_set(&wdt_ready, 1);

	/* The first channel also starts the hardware watchdog (paused while a
	 * debugger halts the core, or every breakpoint would reboot the board).
	 */
	supervisor_channel = ct::supervisor::watch(ct::config::kSupervisorWdtMs);
	if (supervisor_channel < 0) {
		LOG_ERR("supervisor watchdog channel failed: %d", supervisor_channel);
		return;
	}

	LOG_INF("watchdog armed (hardware %u ms)", ct::config::kWatchdogTimeoutMs);
}

#else /* !CONFIG_TASK_WDT */

int supervisor_channel = -1;

void watchdog_init()
{
	LOG_WRN("watchdog disabled in this build; running unprotected");
}

#endif /* CONFIG_TASK_WDT */

void supervisor_thread_entry(void *, void *, void *)
{
	LOG_INF("supervisor task started");

	for (;;) {
		k_sleep(ct::config::kSupervisorTick);
		ct::supervisor::feed(supervisor_channel);
	}
}

} /* namespace */

namespace ct::supervisor {

void start()
{
	/* Here, not in the thread: tasks started right after this register their
	 * channels as soon as they run, so the watchdog has to exist by then.
	 */
	watchdog_init();

	k_thread_create(&supervisor_thread_data, supervisor_stack,
			K_THREAD_STACK_SIZEOF(supervisor_stack), supervisor_thread_entry, nullptr,
			nullptr, nullptr, ct::config::kSupervisorPriority, 0, K_NO_WAIT);
	k_thread_name_set(&supervisor_thread_data, "supervisor");
}

int watch(std::uint32_t timeout_ms)
{
#if defined(CONFIG_TASK_WDT)
	if (atomic_get(&wdt_ready) == 0) {
		return -EAGAIN;
	}

	/* No callback: an expired channel reboots the SoC. */
	return task_wdt_add(timeout_ms, nullptr, nullptr);
#else
	ARG_UNUSED(timeout_ms);
	return -ENOTSUP;
#endif
}

void feed(int channel)
{
#if defined(CONFIG_TASK_WDT)
	if (channel >= 0) {
		(void)task_wdt_feed(channel);
	}
#else
	ARG_UNUSED(channel);
#endif
}

} /* namespace ct::supervisor */
