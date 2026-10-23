/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * ST L3GD20 3-axis gyroscope driver (I2C or SPI).
 *
 * Responsibility: expose the L3GD20's angular rate through Zephyr's sensor
 * API (sample_fetch / channel_get / attr_set) and nothing else. No
 * application logic lives here -- what to do with the rotation rate belongs
 * to whichever task consumes it.
 *
 * Why a new driver rather than an in-tree one: Zephyr has none for this part.
 * Its nearest sibling, i3g4250d, is SPI-only, depends on ST's hal_st module,
 * and is a poor template -- it reports degrees/s where the sensor API
 * specifies rad/s, and returns success without touching its cache when no
 * new sample is ready, so a caller cannot tell a fresh reading from a stale
 * one. This driver is self-contained (plain register access, no HAL) and
 * does neither.
 *
 * Conventions
 * -----------
 *  - Units: SENSOR_CHAN_GYRO_{X,Y,Z,XYZ} are rad/s, as sensor.h specifies.
 *    SENSOR_ATTR_FULL_SCALE is likewise taken in rad/s and rounded to a whole
 *    dps. SENSOR_ATTR_SAMPLING_FREQUENCY is in Hz.
 *  - sample_fetch never returns a stale sample as if it were fresh. It
 *    returns -EAGAIN if the device is still settling after power-up or has
 *    no new sample yet, and leaves the cached sample untouched in that case.
 *    Polling faster than the output data rate therefore sees -EAGAIN between
 *    samples; that is normal, not an error to log.
 *  - Not safe for concurrent use from several threads (fetch/get share one
 *    cache), like most in-tree sensor drivers: give each device one consumer.
 *
 * Power management
 * ----------------
 * Implements the PM_DEVICE actions. TURN_ON probes and configures the chip,
 * leaving it in power-down; RESUME powers it up; SUSPEND powers it back down
 * (about 5 uA vs roughly 6 mA running). pm_device_driver_init() runs RESUME
 * at boot unless the node opts into runtime-PM-auto, in which case the
 * application brings it up with pm_device_runtime_get(). After a power-up
 * the output is not trusted for kTurnOnMs -- see below.
 *
 * Non-goals, explicitly
 * ---------------------
 *  - No interrupts. The data-ready and FIFO/threshold interrupt pins are left
 *    disabled (CTRL_REG3 = 0) and there is no sensor_trigger_set(). A
 *    gyroscope is normally sampled at a fixed rate, which polling covers.
 *  - No temperature channel. OUT_TEMP is 1 LSB/degC with no factory offset,
 *    so it is a relative number, and reporting it as a die temperature would
 *    be misleading.
 *  - No on-chip high-pass filter and no FIFO: CTRL_REG2 and CTRL_REG5 are
 *    written to 0 so a previous firmware's settings cannot leak through an
 *    MCU-only reset, and the output is the plain low-pass-filtered rate.
 *  - 4-wire SPI only (CTRL_REG4.SIM = 0); the 3-wire mode is not supported.
 *  - L3GD20H and I3G4250D are rejected at probe by WHO_AM_I rather than
 *    half-working on a near-identical register map.
 */

#define DT_DRV_COMPAT st_l3gd20

#include <errno.h>
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/pm/device.h>
#include <zephyr/sys/byteorder.h>

#include "l3gd20.h"

LOG_MODULE_REGISTER(l3gd20, CONFIG_SENSOR_LOG_LEVEL);

/* Full-scale range, indexed by CTRL_REG4.FS (00, 01, 10). */
static const uint16_t l3gd20_range_dps[] = {250, 500, 2000};

/* Sensitivity in micro-degrees/s per LSB for each range above: 8.75, 17.5
 * and 70 mdps/LSB. Held in micro-units so the conversion stays in integers.
 */
static const uint32_t l3gd20_udps_per_lsb[] = {8750, 17500, 70000};

/* Output data rate in Hz, indexed by CTRL_REG1.DR (00, 01, 10, 11). */
static const uint16_t l3gd20_odr_hz[] = {95, 190, 380, 760};

/* How long after power-up before the output is trusted.
 *
 * This is a deliberately conservative allowance, sized for the slowest data
 * rate (the datasheet's turn-on time shrinks as the rate rises), not a
 * figure taken from the datasheet's turn-on-time table. Verify it against
 * that table before relying on a shorter wait for fast-start use; too short
 * only costs a few settling-transient samples, too long only delays the
 * first one.
 */
#define L3GD20_TURN_ON_MS 250

/* ---------------------------------------------------------------------
 * Bus transports. Selected per instance in L3GD20_DEFINE() via function
 * pointers in the config, so one build can mix I2C and SPI instances.
 * ------------------------------------------------------------------ */

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)

static int l3gd20_i2c_ready(const struct device *dev)
{
	const struct l3gd20_config *cfg = dev->config;

	return i2c_is_ready_dt(&cfg->bus.i2c) ? 0 : -ENODEV;
}

static int l3gd20_i2c_read(const struct device *dev, uint8_t reg, uint8_t *buf, size_t len)
{
	const struct l3gd20_config *cfg = dev->config;

	if (len > 1) {
		reg |= L3GD20_I2C_AUTOINC;
	}

	return i2c_burst_read_dt(&cfg->bus.i2c, reg, buf, len);
}

static int l3gd20_i2c_write(const struct device *dev, uint8_t reg, uint8_t val)
{
	const struct l3gd20_config *cfg = dev->config;

	return i2c_reg_write_byte_dt(&cfg->bus.i2c, reg, val);
}

#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c) */

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)

/* Mode 3 (CPOL=1, CPHA=1), 8-bit words, MSB first (the default). */
#define L3GD20_SPI_OPERATION (SPI_WORD_SET(8) | SPI_OP_MODE_MASTER | SPI_MODE_CPOL | SPI_MODE_CPHA)

static int l3gd20_spi_ready(const struct device *dev)
{
	const struct l3gd20_config *cfg = dev->config;

	return spi_is_ready_dt(&cfg->bus.spi) ? 0 : -ENODEV;
}

static int l3gd20_spi_read(const struct device *dev, uint8_t reg, uint8_t *buf, size_t len)
{
	const struct l3gd20_config *cfg = dev->config;
	uint8_t cmd = L3GD20_SPI_READ | (reg & L3GD20_SPI_ADDR);

	if (len > 1) {
		cmd |= L3GD20_SPI_MS;
	}

	const struct spi_buf tx_buf = {.buf = &cmd, .len = 1};
	const struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};

	/* The first rx byte is clocked in while the command goes out; discard it. */
	const struct spi_buf rx_buf[2] = {
		{.buf = NULL, .len = 1},
		{.buf = buf, .len = len},
	};
	const struct spi_buf_set rx = {.buffers = rx_buf, .count = 2};

	return spi_transceive_dt(&cfg->bus.spi, &tx, &rx);
}

static int l3gd20_spi_write(const struct device *dev, uint8_t reg, uint8_t val)
{
	const struct l3gd20_config *cfg = dev->config;
	/* R/W = 0, MS = 0: a single-register write. */
	uint8_t frame[2] = {reg & L3GD20_SPI_ADDR, val};

	const struct spi_buf tx_buf = {.buf = frame, .len = sizeof(frame)};
	const struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};

	return spi_write_dt(&cfg->bus.spi, &tx);
}

#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(spi) */

/* ---------------------------------------------------------------------
 * Register helpers
 * ------------------------------------------------------------------ */

static int l3gd20_commit_ctrl1(const struct device *dev, uint8_t new_cfg)
{
	const struct l3gd20_config *cfg = dev->config;
	struct l3gd20_data *data = dev->data;
	const uint8_t reg = new_cfg | (data->powered ? L3GD20_CTRL1_PD : 0);
	const int rc = cfg->write(dev, L3GD20_REG_CTRL1, reg);

	if (rc == 0) {
		data->ctrl1_cfg = new_cfg;
	}

	return rc;
}

static int l3gd20_commit_range(const struct device *dev, uint8_t range_idx)
{
	const struct l3gd20_config *cfg = dev->config;
	struct l3gd20_data *data = dev->data;
	const int rc = cfg->write(dev, L3GD20_REG_CTRL4,
				  L3GD20_CTRL4_BDU | (range_idx << L3GD20_CTRL4_FS_SHIFT));

	if (rc == 0) {
		data->range_idx = range_idx;
	}

	return rc;
}

/* ---------------------------------------------------------------------
 * Sensor API
 * ------------------------------------------------------------------ */

static int l3gd20_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const struct l3gd20_config *cfg = dev->config;
	struct l3gd20_data *data = dev->data;
	uint8_t status;
	uint8_t buf[6];
	int rc;

	switch (chan) {
	case SENSOR_CHAN_ALL:
	case SENSOR_CHAN_GYRO_X:
	case SENSOR_CHAN_GYRO_Y:
	case SENSOR_CHAN_GYRO_Z:
	case SENSOR_CHAN_GYRO_XYZ:
		break;
	default:
		return -ENOTSUP;
	}

	if (!data->powered) {
		return -ENODEV; /* suspended: the caller must resume it first */
	}

	if (k_uptime_get() < data->ready_at_ms) {
		return -EAGAIN; /* still settling after power-up */
	}

	rc = cfg->read(dev, L3GD20_REG_STATUS, &status, 1);
	if (rc != 0) {
		return rc;
	}

	if ((status & L3GD20_STATUS_ZYXDA) == 0) {
		return -EAGAIN; /* nothing new since the last read */
	}

	/* One burst: X_L X_H Y_L Y_H Z_L Z_H. BDU (set at init) holds the
	 * output registers until both bytes of an axis have been read, so a
	 * sample can never be torn across an update.
	 */
	rc = cfg->read(dev, L3GD20_REG_OUT_X_L, buf, sizeof(buf));
	if (rc != 0) {
		return rc;
	}

	for (size_t i = 0; i < ARRAY_SIZE(data->raw); i++) {
		data->raw[i] = (int16_t)sys_get_le16(&buf[2 * i]);
	}

	data->sample_udps_per_lsb = l3gd20_udps_per_lsb[data->range_idx];

	return 0;
}

/* raw counts -> rad/s as a sensor_value.
 *
 * micro-rad/s = raw * (micro-deg/s per LSB) * pi / 180, and SENSOR_PI is
 * pi in millionths, so the divisor carries the extra 10^6. The worst case is
 * 32768 * 70000 * 3141592 ~= 7.2e15, comfortably inside int64.
 */
static void l3gd20_convert(struct sensor_value *val, int16_t raw, uint32_t udps_per_lsb)
{
	const int64_t urad_s = ((int64_t)raw * udps_per_lsb * SENSOR_PI) / (180LL * 1000000LL);

	(void)sensor_value_from_micro(val, urad_s);
}

static int l3gd20_channel_get(const struct device *dev, enum sensor_channel chan,
			      struct sensor_value *val)
{
	const struct l3gd20_data *data = dev->data;

	switch (chan) {
	case SENSOR_CHAN_GYRO_X:
		l3gd20_convert(val, data->raw[0], data->sample_udps_per_lsb);
		return 0;
	case SENSOR_CHAN_GYRO_Y:
		l3gd20_convert(val, data->raw[1], data->sample_udps_per_lsb);
		return 0;
	case SENSOR_CHAN_GYRO_Z:
		l3gd20_convert(val, data->raw[2], data->sample_udps_per_lsb);
		return 0;
	case SENSOR_CHAN_GYRO_XYZ:
		for (size_t i = 0; i < ARRAY_SIZE(data->raw); i++) {
			l3gd20_convert(&val[i], data->raw[i], data->sample_udps_per_lsb);
		}
		return 0;
	default:
		return -ENOTSUP;
	}
}

/* Picks the lowest supported rate that is at least the requested one, so
 * asking for "at least N Hz" never silently undershoots. A request above the
 * top rate is an error rather than being clamped.
 */
static int l3gd20_odr_set(const struct device *dev, const struct sensor_value *val)
{
	struct l3gd20_data *data = dev->data;

	if (val->val1 < 0 || (val->val1 == 0 && val->val2 <= 0)) {
		return -EINVAL;
	}

	const int64_t want_hz = (int64_t)val->val1 + (val->val2 > 0 ? 1 : 0);

	for (size_t i = 0; i < ARRAY_SIZE(l3gd20_odr_hz); i++) {
		if (want_hz <= l3gd20_odr_hz[i]) {
			const uint8_t new_cfg = (data->ctrl1_cfg & ~L3GD20_CTRL1_DR_MASK) |
						(i << L3GD20_CTRL1_DR_SHIFT);
			const int rc = l3gd20_commit_ctrl1(dev, new_cfg);

			if (rc == 0) {
				data->odr_idx = i;
			}

			return rc;
		}
	}

	return -EINVAL;
}

/* The value is rad/s per the sensor API, rounded to whole dps; picks the
 * narrowest range that covers it.
 */
static int l3gd20_range_set(const struct device *dev, const struct sensor_value *val)
{
	const int32_t dps = sensor_rad_to_degrees(val);

	if (dps <= 0) {
		return -EINVAL;
	}

	for (size_t i = 0; i < ARRAY_SIZE(l3gd20_range_dps); i++) {
		if (dps <= l3gd20_range_dps[i]) {
			return l3gd20_commit_range(dev, i);
		}
	}

	return -EINVAL;
}

static int l3gd20_attr_set(const struct device *dev, enum sensor_channel chan,
			   enum sensor_attribute attr, const struct sensor_value *val)
{
	if (chan != SENSOR_CHAN_GYRO_XYZ && chan != SENSOR_CHAN_ALL) {
		return -ENOTSUP;
	}

	switch (attr) {
	case SENSOR_ATTR_SAMPLING_FREQUENCY:
		return l3gd20_odr_set(dev, val);
	case SENSOR_ATTR_FULL_SCALE:
		return l3gd20_range_set(dev, val);
	default:
		return -ENOTSUP;
	}
}

static DEVICE_API(sensor, l3gd20_driver_api) = {
	.attr_set = l3gd20_attr_set,
	.sample_fetch = l3gd20_sample_fetch,
	.channel_get = l3gd20_channel_get,
};

/* ---------------------------------------------------------------------
 * Init / power management
 * ------------------------------------------------------------------ */

static int l3gd20_index_of(const uint16_t *table, size_t n, uint16_t want)
{
	for (size_t i = 0; i < n; i++) {
		if (table[i] == want) {
			return (int)i;
		}
	}

	return -EINVAL;
}

/* Probe and configure, leaving the chip in power-down. Every control
 * register is written explicitly rather than trusting reset defaults: an
 * MCU-only reset does not reset the sensor, so it can still hold whatever
 * the previous firmware left in it.
 */
static int l3gd20_chip_init(const struct device *dev)
{
	const struct l3gd20_config *cfg = dev->config;
	struct l3gd20_data *data = dev->data;
	uint8_t id;
	int rc;

	rc = cfg->read(dev, L3GD20_REG_WHO_AM_I, &id, 1);
	if (rc != 0) {
		LOG_ERR("WHO_AM_I read failed: %d", rc);
		return rc;
	}

	if (id != L3GD20_WHO_AM_I_L3GD20) {
		LOG_ERR("unsupported device, WHO_AM_I=0x%02x (want 0x%02x; 0x%02x is an I3G4250D "
			"-- use st,i3g4250d; 0x%02x is an L3GD20H -- unsupported)",
			id, L3GD20_WHO_AM_I_L3GD20, L3GD20_WHO_AM_I_I3G4250D,
			L3GD20_WHO_AM_I_L3GD20H);
		return -ENODEV;
	}

	const int range_idx =
		l3gd20_index_of(l3gd20_range_dps, ARRAY_SIZE(l3gd20_range_dps), cfg->gyro_range_dps);
	const int odr_idx =
		l3gd20_index_of(l3gd20_odr_hz, ARRAY_SIZE(l3gd20_odr_hz), cfg->gyro_odr_hz);

	if (range_idx < 0 || odr_idx < 0) {
		return -EINVAL; /* unreachable while the binding's enum holds */
	}

	data->powered = false;
	data->odr_idx = odr_idx;
	data->ctrl1_cfg = (odr_idx << L3GD20_CTRL1_DR_SHIFT) | L3GD20_CTRL1_BW_MAX |
			  L3GD20_CTRL1_AXES_ALL;

	/* CTRL1 first, with PD clear: this forces power-down before the rest of
	 * the configuration goes in.
	 */
	rc = l3gd20_commit_ctrl1(dev, data->ctrl1_cfg);
	if (rc != 0) {
		return rc;
	}

	/* CTRL2: high-pass filter mode/cutoff reset. CTRL3: every interrupt and
	 * data-ready routing off. CTRL5: FIFO off, high-pass out of the data
	 * path, plain low-pass output.
	 */
	rc = cfg->write(dev, L3GD20_REG_CTRL2, 0);
	if (rc == 0) {
		rc = cfg->write(dev, L3GD20_REG_CTRL3, 0);
	}
	if (rc == 0) {
		rc = cfg->write(dev, L3GD20_REG_CTRL5, 0);
	}
	if (rc == 0) {
		rc = l3gd20_commit_range(dev, range_idx);
	}

	return rc;
}

static int l3gd20_pm_action(const struct device *dev, enum pm_device_action action)
{
	struct l3gd20_data *data = dev->data;
	int rc;

	switch (action) {
	case PM_DEVICE_ACTION_TURN_ON:
		return l3gd20_chip_init(dev);

	case PM_DEVICE_ACTION_RESUME:
		data->powered = true;
		rc = l3gd20_commit_ctrl1(dev, data->ctrl1_cfg);
		if (rc != 0) {
			data->powered = false;
			return rc;
		}
		data->ready_at_ms = k_uptime_get() + L3GD20_TURN_ON_MS;
		return 0;

	case PM_DEVICE_ACTION_SUSPEND:
		data->powered = false;
		return l3gd20_commit_ctrl1(dev, data->ctrl1_cfg);

	case PM_DEVICE_ACTION_TURN_OFF:
		return 0; /* no power rail under our control */

	default:
		return -ENOTSUP;
	}
}

static int l3gd20_init(const struct device *dev)
{
	const struct l3gd20_config *cfg = dev->config;
	const int rc = cfg->bus_ready(dev);

	if (rc != 0) {
		LOG_ERR("bus not ready");
		return rc;
	}

	return pm_device_driver_init(dev, l3gd20_pm_action);
}

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "L3GD20 driver enabled without any devices"
#endif

#define L3GD20_BUS_I2C(inst)                                                                       \
	.bus = {.i2c = I2C_DT_SPEC_INST_GET(inst)}, .bus_ready = l3gd20_i2c_ready,                 \
	.read = l3gd20_i2c_read, .write = l3gd20_i2c_write

#define L3GD20_BUS_SPI(inst)                                                                       \
	.bus = {.spi = SPI_DT_SPEC_INST_GET(inst, L3GD20_SPI_OPERATION, 0)},                       \
	.bus_ready = l3gd20_spi_ready, .read = l3gd20_spi_read, .write = l3gd20_spi_write

#define L3GD20_DEFINE(inst)                                                                        \
	static struct l3gd20_data l3gd20_data_##inst;                                              \
	static const struct l3gd20_config l3gd20_config_##inst = {                                 \
		COND_CODE_1(DT_INST_ON_BUS(inst, spi), (L3GD20_BUS_SPI(inst)),                     \
			    (L3GD20_BUS_I2C(inst))),                                               \
		.gyro_range_dps = DT_INST_PROP(inst, gyro_range_dps),                              \
		.gyro_odr_hz = DT_INST_PROP(inst, gyro_odr_hz),                                    \
	};                                                                                         \
	PM_DEVICE_DT_INST_DEFINE(inst, l3gd20_pm_action);                                          \
	SENSOR_DEVICE_DT_INST_DEFINE(inst, l3gd20_init, PM_DEVICE_DT_INST_GET(inst),               \
				     &l3gd20_data_##inst, &l3gd20_config_##inst, POST_KERNEL,      \
				     CONFIG_SENSOR_INIT_PRIORITY, &l3gd20_driver_api);

DT_INST_FOREACH_STATUS_OKAY(L3GD20_DEFINE)
