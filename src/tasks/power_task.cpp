/*
 * Power task.
 *
 * Responsibility: put the device into System OFF when it has been still for
 * kIdleSleepMs, and nothing else. Wake-up is not handled here at all -- the
 * accelerometer's INT1 line is armed as the nRF52840's only wake source, and
 * a wake is a full reboot through MCUboot, so the whole firmware simply
 * starts again (the GPS is nudged out of backup mode early in gps::start()).
 *
 * How the idle time is measured:
 *   idle_timer   a one-shot k_timer of kIdleSleepMs, driven by the RTC1
 *                compare interrupt (the kernel's system timer), started at
 *                boot. Every accelerometer motion interrupt restarts it from
 *                the full kIdleSleepMs, via the chan_imu_motion listener
 *                below, which runs straight in the IMU driver's callback.
 *   sleep_now    a semaphore the timer's expiry handler gives. The power
 *                thread blocks on it and does nothing else until it fires.
 *   cell_active  a mutex (modem_link.hpp) the cellular task holds for the
 *                whole of each modem session. Before shutting down, the power
 *                thread takes it -- waiting, if a transmission is in progress,
 *                for it to finish -- and holds it until the MCU is off, so no
 *                new session can start in between. Abandoning sleep releases it.
 *
 * Sleep sequence, ordered so everything that can still say "not now" comes
 * before anything that cannot be taken back:
 *   1. Take cell_active (waiting for a transmission to finish), then power
 *      the modem down (AT+QPOWD, ~12 s).
 *   2. imu::prepare_for_sleep()  arms the wake source and checks INT1 is
 *      quiet. Fails safe: no verified wake source, no sleep.
 *   After each of these, motion since the timer fired abandons the attempt
 *   (imu::cancel_sleep(), release cell_active); the listener has already
 *   restarted the timer.
 *   3. Point of no return: LED off, USB off, GPS to backup mode (or, if the
 *      optional 3.3 V / modem rails have switches, cut them and release the pins
 *      facing them -- see cut_switched_rails()), then
 *      imu::arm_wake_source() -- last, because each of those steps changes
 *      the supply current and can make the accelerometer see a phantom
 *      motion; it clears the latch, confirms INT1 is low and only then arms
 *      the wake pin -- then sys_poweroff(). Real motion in these milliseconds
 *      leaves INT1 high, and a high level at System OFF wakes the chip at
 *      once, so it is not lost.
 *
 * Deliberately not done: stopping BLE or suspending the UARTs. System OFF
 * powers the radio and every peripheral down, and the UART pins are best left
 * exactly as they are -- the GPS's RX line held high is what keeps the
 * receiver in backup mode (RX activity wakes it).
 *
 * The GPS goes to UBX backup mode in step 3.
 */

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/regulator.h>
#include <hal/nrf_gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/poweroff.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/zbus/zbus.h>

#include "../app_config.hpp"
#include "../channels.hpp"
#include "../messages.hpp"
#include "../services/fw_update.hpp"
#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
#include "../services/modem_link.hpp"
#endif
#include "../services/selftest.hpp"
#include "gps_task.hpp"
#include "imu_task.hpp"
#include "power_task.hpp"

LOG_MODULE_REGISTER(ct_power, CT_LOG_LEVEL);

namespace {

/* sleep_now: given by idle_timer's expiry, taken by the power thread. */
K_SEM_DEFINE(sleep_now, 0, 1);

/* Set by every motion interrupt, cleared when idle_timer fires: "the board
 * moved since the timer last expired", i.e. the sleep attempt is stale.
 */
atomic_t motion_since_expiry;

void idle_timer_expired(k_timer *)
{
	atomic_clear(&motion_since_expiry);
	k_sem_give(&sleep_now);
}

K_TIMER_DEFINE(idle_timer, idle_timer_expired, nullptr);

void restart_idle_timer(k_timeout_t timeout)
{
	k_timer_start(&idle_timer, timeout, K_NO_WAIT);
}

/* chan_imu_motion listener: runs synchronously in the publisher's context
 * (the LIS2DH driver's trigger thread), so it must not block -- k_timer_start
 * and an atomic are both safe there.
 */
void on_motion(const zbus_channel *)
{
	atomic_set(&motion_since_expiry, 1);
	restart_idle_timer(K_MSEC(ct::config::kIdleSleepMs));
}

} /* namespace */

/* External linkage required: channels.cpp references this observer. */
ZBUS_LISTENER_DEFINE(power_motion_lis, on_motion);

/* The board's USB context is a static iterable (cdc_acm_serial), reachable only
 * by walking them. Detaching cleanly lets the host see a disconnect rather than
 * a device that just stops answering. Global scope, not the anonymous
 * namespace below: the iterable's start/end symbols are declared where the
 * macro expands, and must be the global ones the linker defines.
 */
static void usb_detach()
{
	STRUCT_SECTION_FOREACH(usbd_context, usb_ctx) {
		(void)usbd_disable(usb_ctx);
	}
}

namespace {

/* Optional switched rails (rail_3v3 / modem_vbat in app.overlay). Present only
 * once the nodes are enabled; without them everything below compiles away and
 * the loads stay powered, as before.
 */
#if DT_NODE_HAS_STATUS(DT_ALIAS(rail_3v3), okay)
#define CT_3V3_RAIL 1
const device *const k3v3Rail = DEVICE_DT_GET(DT_ALIAS(rail_3v3));
#endif
#if DT_NODE_HAS_STATUS(DT_ALIAS(modem_power), okay)
#define CT_MODEM_RAIL 1
const device *const kModemRail = DEVICE_DT_GET(DT_ALIAS(modem_power));
#endif

/* Disconnects a pin (input, buffer off, no pull). A pin left driving a load
 * whose supply has been cut back-powers it through the load's input
 * protection diode, which leaks current and can hold the load half-alive.
 */
#define CT_RELEASE_PIN(port, pin) nrf_gpio_cfg_default(NRF_GPIO_PIN_MAP(port, pin))

#define CT_GPIO_PORT(alias, prop) DT_PROP(DT_GPIO_CTLR(DT_ALIAS(alias), prop), port)
#define CT_GPIO_PIN(alias, prop)  DT_GPIO_PIN(DT_ALIAS(alias), prop)

K_THREAD_STACK_DEFINE(power_stack, ct::config::kPowerStackSize);
k_thread power_thread_data;

bool moved_since_expiry()
{
	return atomic_get(&motion_since_expiry) != 0;
}

bool may_sleep_now()
{
	return ct::selftest::finished() && !ct::fw_update::is_staging();
}

/* Cuts whichever rails exist, and lets go of the MCU pins that face them.
 * Safe to call with no rails configured (does nothing). Called before
 * imu::arm_wake_source(): switching a load changes the supply current, which
 * is exactly what the sensor must be given time to settle from.
 */
void cut_switched_rails()
{
#if defined(CT_3V3_RAIL)
	(void)regulator_disable(k3v3Rail);
	/* The GPS is the only known MCU-facing load on this rail: uart0, board
	 * pinctrl, P0.09 TX / P0.10 RX (see app.overlay).
	 */
	CT_RELEASE_PIN(0, 9);
	CT_RELEASE_PIN(0, 10);
#endif
#if defined(CT_MODEM_RAIL)
	(void)regulator_disable(kModemRail);
	/* uart1 TX/RX, then the modem's PWRKEY and RESET_N lines (app.overlay). */
	CT_RELEASE_PIN(0, 6);
	CT_RELEASE_PIN(0, 8);
	CT_RELEASE_PIN(CT_GPIO_PORT(modem, mdm_power_gpios), CT_GPIO_PIN(modem, mdm_power_gpios));
	CT_RELEASE_PIN(CT_GPIO_PORT(modem, mdm_reset_gpios), CT_GPIO_PIN(modem, mdm_reset_gpios));
#endif
}

/* Lets the cellular task start sessions again (it re-powers the modem itself). */
void release_cell()
{
#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
	(void)k_mutex_unlock(&cell_active);
#endif
}

/* Returns only if sleeping was abandoned. */
void try_sleep()
{
	LOG_INF("no motion for %d s; preparing System OFF",
		static_cast<int>(ct::config::kIdleSleepMs / 1000));

#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
	/* A transmission is capped at kCellSessionMaxMs, so this normally returns
	 * well inside kCellLockWaitMs. If it does not, the cellular task is stuck
	 * rather than slow: give up on this attempt (the caller starts a fresh idle
	 * period) instead of waiting for it forever.
	 */
	if (k_mutex_lock(&cell_active, K_MSEC(ct::config::kCellLockWaitMs)) != 0) {
		LOG_WRN("not sleeping: cellular task held cell_active for over %d s",
			static_cast<int>(ct::config::kCellLockWaitMs / 1000));
		return;
	}

	if (moved_since_expiry()) {
		LOG_INF("not sleeping: motion while waiting for the modem");
		release_cell();
		return;
	}

	/* Modem off first: it takes ~12 s, and it must not be left running through
	 * System OFF (a successful publish deliberately leaves it on).
	 */
	if (const int err = ct::modem::power_down(); err != 0) {
		LOG_WRN("modem power-down failed: %d", err);
	}
#endif

	if (const int err = ct::imu::prepare_for_sleep(); err != 0) {
		LOG_WRN("not sleeping: wake source unavailable (%d)", err);
		release_cell();
		return;
	}

	if (moved_since_expiry() || !may_sleep_now()) {
		LOG_INF("not sleeping: device became busy while preparing");
		ct::imu::cancel_sleep();
		release_cell();
		return;
	}

	LOG_INF("entering System OFF; wake on accelerometer INT1");

	/* Let the log line out before the console goes away. */
	k_sleep(K_MSEC(50));

	ct::selftest::led_off();
	usb_detach();
#if !defined(CT_3V3_RAIL)
	/* With the 3.3 V line cut the GPS's backup mode is moot (and cutting it
	 * loses the backup RAM anyway), so only ask for backup when it stays on.
	 */
	ct::gps::standby_until_wake();
#endif
	cut_switched_rails();
	k_sleep(K_MSEC(150)); /* UBX frame goes out byte by byte; let it and the rails settle */

	/* Last, so no later load change can leave a spurious interrupt behind. */
	ct::imu::arm_wake_source();

	sys_poweroff();
}

void power_thread_entry(void *, void *, void *)
{
	LOG_INF("power task started (idle sleep after %d s)",
		static_cast<int>(ct::config::kIdleSleepMs / 1000));

	restart_idle_timer(K_MSEC(ct::config::kIdleSleepMs));

	for (;;) {
		/* Wake once a minute just to say why not asleep yet (diagnostic). */
		if (k_sem_take(&sleep_now, K_SECONDS(60)) != 0) {
			LOG_INF("awake: idle timer %u s left, selftest=%d staging=%d cell_active=%s",
				static_cast<unsigned>(k_ticks_to_ms_floor64(
					k_timer_remaining_ticks(&idle_timer)) / 1000),
				ct::selftest::finished(), ct::fw_update::is_staging(),
#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
				cell_active.owner != nullptr ? "held" : "free"
#else
				"n/a"
#endif
				);
			continue;
		}

		if (!may_sleep_now()) {
			/* Self-test still running or an image being staged: look again
			 * shortly, unless motion restarts the full idle period first.
			 */
			restart_idle_timer(K_MSEC(ct::config::kIdleSleepRetryMs));
			continue;
		}

		try_sleep();

		/* Abandoned. After motion the listener has already restarted the
		 * timer; otherwise (no wake source, say) start a fresh idle period
		 * rather than retrying in a tight loop.
		 */
		if (!moved_since_expiry()) {
			restart_idle_timer(K_MSEC(ct::config::kIdleSleepMs));
		}
	}
}

} /* namespace */

namespace ct::power {

void start()
{
	k_thread_create(&power_thread_data, power_stack, K_THREAD_STACK_SIZEOF(power_stack),
			power_thread_entry, nullptr, nullptr, nullptr, ct::config::kPowerPriority,
			0, K_NO_WAIT);
	k_thread_name_set(&power_thread_data, "power");
}

} /* namespace ct::power */
