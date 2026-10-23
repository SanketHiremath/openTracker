/*
 * IMU task (bench bring-up probe).
 *
 * Responsibility: prove, on the bench, that the motion sensors on i2c0 bind,
 * answer, and return plausible numbers; that the accelerometer's any-motion
 * interrupt (INT1 on P1.06) fires; and report the dominant vibration frequency
 * of a 2.56 s capture (FFT, see spectrum.cpp). The only thing it publishes is
 * each capture's result on chan_vibration (observed by BLE); it makes no
 * decisions and there is deliberately no ignition logic here.
 *
 * Deliberately outside the boot self-test gate (see main.cpp): the self-test
 * is all-or-nothing, so a missing or miswired sensor must not be able to park
 * GPS and cellular.
 *
 * What to look for in the log:
 *   accel  m/s^2  at rest, flat: magnitude ~9.8 split across the axes
 *   magn   gauss  Earth's field, ~0.25-0.65 total, changes as you rotate it
 *   gyro   rad/s  ~0 at rest; skipped while disabled in app.overlay
 *   "accel vibration: dominant N Hz" once per 2.56 s window; "none" when the
 *   board is too still to call.
 *
 * A device reported "not ready" means its driver's init failed -- on I2C that
 * is almost always wiring, a wrong address, or missing pull-ups.
 */

#include <cstdint>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/pm/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)
#include <hal/nrf_gpio.h>
#endif

#include "../app_config.hpp"
#include "../channels.hpp"
#include "../messages.hpp"
#include "../services/spectrum.hpp"
#include "imu_task.hpp"

LOG_MODULE_REGISTER(ct_imu, CT_LOG_LEVEL);

namespace {

K_THREAD_STACK_DEFINE(probe_stack, ct::config::kImuStackSize);
k_thread probe_thread_data;

/* Hand-off with prepare_for_sleep(): the thread parks at the top of its loop
 * when asked, so the sleep preparation owns the I2C bus alone.
 */
atomic_t sleep_requested;
K_SEM_DEFINE(parked_sem, 0, 1);
K_SEM_DEFINE(resume_sem, 0, 1);

struct Sensor {
	const char *name;
	const char *unit;
	const device *dev;
	sensor_channel chan;
	int last_rc; /* log an error only when it changes, not every second */
};

Sensor sensors[] = {
	{"accel", "m/s^2", DEVICE_DT_GET_OR_NULL(DT_ALIAS(accel0)), SENSOR_CHAN_ACCEL_XYZ, 0},
	{"magn", "gauss", DEVICE_DT_GET_OR_NULL(DT_ALIAS(magn0)), SENSOR_CHAN_MAGN_XYZ, 0},
	{"gyro", "rad/s", DEVICE_DT_GET_OR_NULL(DT_ALIAS(gyro0)), SENSOR_CHAN_GYRO_XYZ, 0},
};

/* Raw access to the accelerometer, used to finish configuring its interrupt
 * Register numbers are the LIS2DH/LSM303DLHC-compatible map.
 */
const device *const kAccelBus = DEVICE_DT_GET(DT_BUS(DT_ALIAS(accel0)));
constexpr uint16_t kAccelAddr = DT_REG_ADDR(DT_ALIAS(accel0));
const gpio_dt_spec kInt1Pin = GPIO_DT_SPEC_GET(DT_ALIAS(accel0), irq_gpios);
constexpr uint8_t kRegCtrl1 = 0x20;
constexpr uint8_t kRegReference = 0x26;
constexpr uint8_t kRegInt1Cfg = 0x30;
constexpr uint8_t kRegInt1Src = 0x31;
constexpr uint8_t kRegInt1Ths = 0x32;
constexpr uint8_t kRegInt1Dur = 0x33;

uint8_t read_reg(uint8_t reg)
{
	uint8_t v = 0xEE; /* visible sentinel if the read fails */

	(void)i2c_reg_read_byte(kAccelBus, kAccelAddr, reg, &v);
	return v;
}

/* Runs on the LIS2DH driver's own thread (CONFIG_LIS2DH_TRIGGER_OWN_THREAD),
 * not in interrupt context, after the driver has already read INT1_SRC to
 * release the latched line. Every interrupt is published: this is the power
 * task's inactivity clock. K_NO_WAIT because this is the driver's thread; a
 * full subscriber queue just means one is already pending.
 */
void motion_handler(const device *, const sensor_trigger *)
{
#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)
	const ct::ImuMotion msg{static_cast<uint32_t>(k_uptime_get())};

	(void)zbus_chan_pub(&chan_imu_motion, &msg, K_NO_WAIT);
#endif
}

/* High-pass filter + hardware threshold/duration, then attach the trigger.
 * Order matters: the filter must be on before the comparator is armed, or
 * gravity alone trips it.
 */
void arm_motion_interrupt(const device *accel)
{
	/* CTRL_REG2 = 0x01: HPIS1 -- the high-pass filter feeds the any-motion
	 * (AOI1) generator. HPM/HPCF at 0 (normal mode, highest cutoff).
	 */
	const sensor_value hpf = {.val1 = 0x01, .val2 = 0};
	/* m/s^2. The driver quantises to 128/range steps: at +/-2 g that is
	 * ~15.6 mg per step, so 0.32 m/s^2 (~33 mg) lands on step 2. That is the
	 * "small motion" setting: well above a still board's few-mg noise, but a
	 * gentle nudge clears it. Step 1 (~16 mg) is the floor and risks false
	 * triggers; raise this if the interrupt fires on a board that is sitting still.
	 */
	const sensor_value thresh = {.val1 = 0, .val2 = 320000};
	/* Samples above threshold before INT1 asserts: 1 at 100 Hz = 10 ms. */
	const sensor_value dur = {.val1 = 1, .val2 = 0};
	const sensor_trigger trig = {.type = SENSOR_TRIG_DELTA, .chan = SENSOR_CHAN_ACCEL_XYZ};
	int rc;

	rc = sensor_attr_set(accel, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_CONFIGURATION, &hpf);
	if (rc == 0) {
		rc = sensor_attr_set(accel, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SLOPE_TH, &thresh);
	}
	if (rc == 0) {
		rc = sensor_attr_set(accel, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SLOPE_DUR, &dur);
	}
	if (rc == 0) {
		rc = sensor_trigger_set(accel, &trig, motion_handler);
	}

	if (rc != 0) {
		LOG_ERR("accel: failed to arm motion interrupt: %d", rc);
		return;
	}

	/* The driver writes INT1_CFG later, on its own thread ("serialize start of
	 * int2 in thread"), so give it time to finish before reading it back.
	 *
	 * Only the driver's three HIGH events (INT1_CFG 0x2A) are used, and that is
	 * enough for motion in either direction: the comparator tests the
	 * magnitude of the high-passed signal, so a push and its rebound both clear
	 * it. Do NOT add the LOW events (0x15): on the LIS2DH family "low" means
	 * magnitude BELOW the threshold (the free-fall detector), which is true on
	 * every axis whenever the board is still -- INT1 then asserts permanently.
	 */
	k_sleep(K_MSEC(200));

	/* High-pass filter is in normal mode (HPM=00), where reading REFERENCE
	 * resets its baseline to the current reading. Without this the filter
	 * starts from zero at boot and gravity itself looks like motion until it
	 * settles.
	 */
	(void)read_reg(kRegReference);
}

/* sensor_value -> "-1.234" (milli-units). snprintk with plain ints rather than
 * a signed split of val1/val2, which prints "-0.5" as "0.500".
 */
void fmt(char *out, size_t n, const sensor_value &v)
{
	const int64_t micro = sensor_value_to_micro(&v);
	const int64_t mag = micro < 0 ? -micro : micro;

	snprintk(out, n, "%s%d.%03d", micro < 0 ? "-" : "", static_cast<int>(mag / 1000000),
		 static_cast<int>((mag % 1000000) / 1000));
}

static_assert(IS_ENABLED(CONFIG_LIS2DH_ODR_5) && ct::config::kImuVibrationSampleRateHz == 100,
	      "the FFT window assumes the accelerometer's output data rate equals "
	      "kImuVibrationSampleRateHz; change prj.conf and app_config.hpp together");

/* One analysis window, gravity included, in mm/s^2. Static: ~1.5 KB that
 * would be a poor fit for a thread stack.
 */
std::int16_t win_x[ct::spectrum::kWindowSize];
std::int16_t win_y[ct::spectrum::kWindowSize];
std::int16_t win_z[ct::spectrum::kWindowSize];

/* sensor_value (m/s^2) -> mm/s^2. +/-2 g is +/-19613, inside int16; clamped
 * anyway so a driver surprise cannot wrap.
 */
std::int16_t to_mms2(const sensor_value &v)
{
	const int32_t mms2 = v.val1 * 1000 + v.val2 / 1000;

	return static_cast<std::int16_t>(CLAMP(mms2, -32767, 32767));
}

/* Samples one window at a steady rate, runs the FFT and logs the dominant
 * frequency. Takes 2.56 s. Returns false if a sensor read failed.
 *
 * Timed against absolute deadlines (K_TIMEOUT_ABS_MS), not "sleep 10 ms after
 * each read": a relative sleep would add each read's own duration to the
 * period and drift, and the FFT needs the samples evenly spaced. The sensor's
 * own output clock is asynchronous to ours, so now and then one reading is
 * repeated or skipped; that adds a little broadband noise but does not move a
 * real tone's peak.
 */
bool report_vibration(const device *accel)
{
	constexpr int64_t kPeriodMs = 1000 / ct::config::kImuVibrationSampleRateHz;
	int64_t next = k_uptime_get();

	for (size_t i = 0; i < ct::spectrum::kWindowSize; ++i) {
		next += kPeriodMs;
		k_sleep(K_TIMEOUT_ABS_MS(next));

		sensor_value v[3]{};
		int rc = sensor_sample_fetch(accel);

		if (rc == 0) {
			rc = sensor_channel_get(accel, SENSOR_CHAN_ACCEL_XYZ, v);
		}

		if (rc != 0) {
			LOG_WRN("vibration: accel read failed (%d); window discarded", rc);
			return false;
		}

		win_x[i] = to_mms2(v[0]);
		win_y[i] = to_mms2(v[1]);
		win_z[i] = to_mms2(v[2]);
	}

	const auto r = ct::spectrum::analyze(win_x, win_y, win_z,
					     ct::config::kImuVibrationSampleRateHz,
					     ct::config::kImuVibrationMinRmsMms2);

	ct::VibrationState state{};

	state.significant = r.significant;
	state.freq_mhz = r.freq_mhz;
	state.peak_share_pct = r.peak_share_pct;
	state.rms_mms2 = r.rms_mms2;
	(void)zbus_chan_pub(&chan_vibration, &state, K_MSEC(50));

	if (r.significant) {
		LOG_INF("accel vibration: dominant %u.%02u Hz (%u%% of power in the peak, "
			"rms %d mm/s^2)",
			r.freq_mhz / 1000, (r.freq_mhz % 1000) / 10, r.peak_share_pct, r.rms_mms2);
	} else {
		LOG_INF("accel vibration: none (rms %d mm/s^2, below %d)", r.rms_mms2,
			ct::config::kImuVibrationMinRmsMms2);
	}

	return true;
}

void probe_thread_entry(void *, void *, void *)
{
	LOG_INF("IMU task started");

	for (auto &s : sensors) {
		if (s.dev == nullptr) {
			LOG_INF("%s: not in devicetree (disabled); skipped", s.name);
		} else if (device_is_ready(s.dev)) {
			LOG_INF("%s: ready", s.name);
		} else {
			LOG_ERR("%s: NOT ready (driver init failed -- check wiring, I2C address, "
				"pull-ups)",
				s.name);
		}
	}

	if (sensors[0].dev != nullptr && device_is_ready(sensors[0].dev)) {
		arm_motion_interrupt(sensors[0].dev);
	}

	for (;;) {
		if (atomic_get(&sleep_requested) != 0) {
			k_sem_give(&parked_sem);
			k_sem_take(&resume_sem, K_FOREVER);
		}

		for (auto &s : sensors) {
			if (s.dev == nullptr || !device_is_ready(s.dev)) {
				continue;
			}

			int rc = sensor_sample_fetch(s.dev);
			sensor_value v[3]{};

			if (rc == 0) {
				rc = sensor_channel_get(s.dev, s.chan, v);
			}

			if (rc == -EAGAIN) {
				continue; /* gyro settling / no new sample yet: normal */
			}

			if (rc != 0) {
				if (rc != s.last_rc) {
					LOG_ERR("%s: read failed: %d", s.name, rc);
				}
				s.last_rc = rc;
				continue;
			}

			s.last_rc = 0;

			char x[16], y[16], z[16];

			fmt(x, sizeof(x), v[0]);
			fmt(y, sizeof(y), v[1]);
			fmt(z, sizeof(z), v[2]);
			LOG_INF("%s: x=%s y=%s z=%s %s", s.name, x, y, z, s.unit);
		}

		/* The vibration window doubles as the loop's pacing (it takes 2.56 s),
		 * so the raw lines above now repeat about every 2.6 s rather than 1 s.
		 */
		if (sensors[0].dev == nullptr || !device_is_ready(sensors[0].dev) ||
		    !report_vibration(sensors[0].dev)) {
			k_sleep(K_SECONDS(1));
		}
	}
}

} /* namespace */

namespace ct::imu {

void start()
{
	k_thread_create(&probe_thread_data, probe_stack, K_THREAD_STACK_SIZEOF(probe_stack),
			probe_thread_entry, nullptr, nullptr, nullptr,
			ct::config::kImuPriority, 0, K_NO_WAIT);
	k_thread_name_set(&probe_thread_data, "imu");
}

} /* namespace ct::imu */

#if defined(CONFIG_CAR_TRACKER_IDLE_SLEEP)

namespace {

constexpr uint16_t kMagnAddr = DT_REG_ADDR(DT_ALIAS(magn0));
constexpr uint8_t kRegMagnMode = 0x02; /* MR_REG_M */
constexpr uint8_t kMagnModeContinuous = 0x00;
constexpr uint8_t kMagnModeSleep = 0x03;

/* CTRL_REG1_A for the sleeping accelerometer: ODR 25 Hz (0011), LPen (bit 3),
 * XYZ enables kept. The wake source must keep running, so it cannot be powered
 * down -- but at 25 Hz in low-power mode it draws a few microamps instead of
 * the ~100 uA of normal mode at 100 Hz. Its threshold and the filter setup are
 * untouched; only the sampling rate (and so the duration unit) changes.
 */
constexpr uint8_t kCtrl1SleepOdrLp = 0x38;
constexpr uint8_t kCtrl1AxisMask = 0x07;
constexpr int kQuietAttempts = 10;
constexpr int kFinalQuietAttempts = 20;

uint8_t saved_ctrl1;
uint8_t saved_int1_ths;
uint8_t saved_int1_dur;

/* One settle-and-check pass. Reading REFERENCE resets the high-pass filter's
 * baseline (HPM=00), so the sample-rate change's start-up transient does not
 * look like motion; reading INT1_SRC releases the latched line.
 */
bool wake_line_quiet()
{
	(void)read_reg(kRegReference);
	k_sleep(K_MSEC(300));
	(void)read_reg(kRegInt1Src);
	k_sleep(K_MSEC(100));

	return gpio_pin_get_raw(kInt1Pin.port, kInt1Pin.pin) == 0;
}

} /* namespace */

namespace ct::imu {

int prepare_for_sleep()
{
	const device *accel = sensors[0].dev;

	if (accel == nullptr || !device_is_ready(accel)) {
		return -ENODEV;
	}

	k_sem_reset(&parked_sem);
	atomic_set(&sleep_requested, 1);

	if (k_sem_take(&parked_sem, K_SECONDS(6)) != 0) {
		/* Leave a resume token in case the thread parks just after this. */
		atomic_set(&sleep_requested, 0);
		k_sem_give(&resume_sem);
		return -ETIMEDOUT;
	}

	/* Magnetometer: sleep mode. Gyro, if it is enabled at all: its driver's
	 * own suspend. Neither is needed to wake the device.
	 */
	(void)i2c_reg_write_byte(kAccelBus, kMagnAddr, kRegMagnMode, kMagnModeSleep);

	if (sensors[2].dev != nullptr && device_is_ready(sensors[2].dev)) {
		(void)pm_device_action_run(sensors[2].dev, PM_DEVICE_ACTION_SUSPEND);
	}

	if (i2c_reg_read_byte(kAccelBus, kAccelAddr, kRegCtrl1, &saved_ctrl1) != 0) {
		cancel_sleep();
		return -EIO;
	}

	const uint8_t sleeping = (saved_ctrl1 & kCtrl1AxisMask) | kCtrl1SleepOdrLp;

	if (i2c_reg_write_byte(kAccelBus, kAccelAddr, kRegCtrl1, sleeping) != 0) {
		cancel_sleep();
		return -EIO;
	}

	/* Desensitise for sleep (see kImuSleepIntThsReg for why), keeping the
	 * awake values to put back if the sleep is abandoned.
	 */
	if (i2c_reg_read_byte(kAccelBus, kAccelAddr, kRegInt1Ths, &saved_int1_ths) != 0 ||
	    i2c_reg_read_byte(kAccelBus, kAccelAddr, kRegInt1Dur, &saved_int1_dur) != 0 ||
	    i2c_reg_write_byte(kAccelBus, kAccelAddr, kRegInt1Ths,
			       ct::config::kImuSleepIntThsReg) != 0 ||
	    i2c_reg_write_byte(kAccelBus, kAccelAddr, kRegInt1Dur,
			       ct::config::kImuSleepIntDurReg) != 0) {
		cancel_sleep();
		return -EIO;
	}

	/* The line must be low before sleeping: the wake input is level
	 * sensitive, so a latched or still-settling INT1 would wake the device
	 * straight back up. If the board will not settle, it is not idle.
	 */
	bool quiet = false;

	for (int i = 0; i < kQuietAttempts && !quiet; ++i) {
		quiet = wake_line_quiet();
	}

	if (!quiet) {
		LOG_WRN("accel: INT1 will not settle; staying awake");
		cancel_sleep();
		return -EBUSY;
	}

	LOG_INF("accel: low-power 25 Hz, INT1 quiet; wake source not yet armed");
	return 0;
}

void arm_wake_source()
{
	/* Last step before System OFF, after the caller has made its last load
	 * change (GPS to backup, USB detached). Those steps step the supply
	 * current, which on a shared rail can shift the accelerometer's reading
	 * enough for its high-pass filter to call it motion -- an interrupt that
	 * a quiet check made earlier cannot see. So clear and re-check here, as
	 * close to the power-off as possible. One 25 Hz sample is 40 ms, hence
	 * 50 ms between checks. If INT1 never settles the board really is moving,
	 * and waking straight away is the right outcome anyway.
	 */
	for (int i = 0; i < kFinalQuietAttempts; ++i) {
		(void)read_reg(kRegInt1Src);
		k_sleep(K_MSEC(50));

		if (gpio_pin_get_raw(kInt1Pin.port, kInt1Pin.pin) == 0) {
			break;
		}
	}

	/* Take the pin away from the driver's callback and arm it as the
	 * System OFF wake source. Written directly so it does not depend on how
	 * the GPIO driver maps interrupt modes onto SENSE.
	 */
	(void)gpio_pin_interrupt_configure_dt(&kInt1Pin, GPIO_INT_DISABLE);
	nrf_gpio_cfg_sense_input(
		NRF_GPIO_PIN_MAP(DT_PROP(DT_GPIO_CTLR(DT_ALIAS(accel0), irq_gpios), port),
				 DT_GPIO_PIN(DT_ALIAS(accel0), irq_gpios)),
		NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_HIGH);
}

void cancel_sleep()
{
	(void)i2c_reg_write_byte(kAccelBus, kAccelAddr, kRegCtrl1, saved_ctrl1);
	(void)i2c_reg_write_byte(kAccelBus, kAccelAddr, kRegInt1Ths, saved_int1_ths);
	(void)i2c_reg_write_byte(kAccelBus, kAccelAddr, kRegInt1Dur, saved_int1_dur);
	(void)i2c_reg_write_byte(kAccelBus, kMagnAddr, kRegMagnMode, kMagnModeContinuous);

	if (sensors[2].dev != nullptr && device_is_ready(sensors[2].dev)) {
		(void)pm_device_action_run(sensors[2].dev, PM_DEVICE_ACTION_RESUME);
	}

	(void)read_reg(kRegReference); /* settle the high-pass filter at the restored rate */
	(void)gpio_pin_interrupt_configure_dt(&kInt1Pin, GPIO_INT_LEVEL_HIGH);

	atomic_set(&sleep_requested, 0);
	k_sem_give(&resume_sem);
}

} /* namespace ct::imu */

#endif /* CONFIG_CAR_TRACKER_IDLE_SLEEP */
