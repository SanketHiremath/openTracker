/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * ST L3GD20 gyroscope driver -- register map and driver structs.
 *
 * Register layout and bit positions are from the L3GD20 datasheet (the
 * original part, WHO_AM_I 0xD4). The L3GD20H is a different register layout
 * and is not handled here.
 */

#ifndef CAR_TRACKER_DRIVERS_SENSOR_L3GD20_L3GD20_H_
#define CAR_TRACKER_DRIVERS_SENSOR_L3GD20_L3GD20_H_

#include <stdbool.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>

/* --- Registers --------------------------------------------------------- */

#define L3GD20_REG_WHO_AM_I 0x0F
#define L3GD20_REG_CTRL1    0x20
#define L3GD20_REG_CTRL2    0x21
#define L3GD20_REG_CTRL3    0x22
#define L3GD20_REG_CTRL4    0x23
#define L3GD20_REG_CTRL5    0x24
#define L3GD20_REG_STATUS   0x27
#define L3GD20_REG_OUT_X_L  0x28 /* X_L, X_H, Y_L, Y_H, Z_L, Z_H follow */

#define L3GD20_WHO_AM_I_L3GD20   0xD4
#define L3GD20_WHO_AM_I_I3G4250D 0xD3 /* sibling part, own driver */
#define L3GD20_WHO_AM_I_L3GD20H  0xD7 /* different register layout */

/* CTRL_REG1: DR1 DR0 BW1 BW0 PD Zen Yen Xen. Note the axis-enable bits run
 * Z, Y, X from bit 2 down to bit 0 -- the reverse of the usual X, Y, Z order.
 */
#define L3GD20_CTRL1_DR_SHIFT 6
#define L3GD20_CTRL1_DR_MASK  (0x3 << L3GD20_CTRL1_DR_SHIFT)
#define L3GD20_CTRL1_BW_SHIFT 4
#define L3GD20_CTRL1_BW_MAX   (0x3 << L3GD20_CTRL1_BW_SHIFT) /* widest low-pass */
#define L3GD20_CTRL1_PD       BIT(3) /* 0 = power-down, 1 = normal/sleep */
#define L3GD20_CTRL1_ZEN      BIT(2)
#define L3GD20_CTRL1_YEN      BIT(1)
#define L3GD20_CTRL1_XEN      BIT(0)
#define L3GD20_CTRL1_AXES_ALL (L3GD20_CTRL1_ZEN | L3GD20_CTRL1_YEN | L3GD20_CTRL1_XEN)

/* CTRL_REG4: BDU BLE FS1 FS0 - ST1 ST0 SIM. FS 00 = 250 dps, 01 = 500 dps,
 * 10/11 = 2000 dps. BLE=0 keeps little-endian output (low byte first).
 */
#define L3GD20_CTRL4_BDU      BIT(7) /* output regs frozen until both bytes read */
#define L3GD20_CTRL4_FS_SHIFT 4

/* STATUS_REG: ZYXOR ZOR YOR XOR ZYXDA ZDA YDA XDA */
#define L3GD20_STATUS_ZYXDA BIT(3) /* new data on all three axes */

/* I2C: setting the register address MSB makes the device auto-increment. */
#define L3GD20_I2C_AUTOINC BIT(7)

/* SPI: first byte is R/W (bit 7, 1 = read), MS (bit 6, 1 = auto-increment),
 * then the 6-bit register address.
 */
#define L3GD20_SPI_READ  BIT(7)
#define L3GD20_SPI_MS    BIT(6)
#define L3GD20_SPI_ADDR  BIT_MASK(6)

/* --- Driver structs ---------------------------------------------------- */

union l3gd20_bus {
	struct i2c_dt_spec i2c;
	struct spi_dt_spec spi;
};

struct l3gd20_config {
	union l3gd20_bus bus;
	int (*bus_ready)(const struct device *dev);
	int (*read)(const struct device *dev, uint8_t reg, uint8_t *buf, size_t len);
	int (*write)(const struct device *dev, uint8_t reg, uint8_t val);
	uint16_t gyro_range_dps; /* from devicetree: initial full scale */
	uint16_t gyro_odr_hz;    /* from devicetree: initial output data rate */
};

struct l3gd20_data {
	/* Latest sample, raw counts, X/Y/Z. Written only by sample_fetch. */
	int16_t raw[3];

	/* Sensitivity (micro-degrees/s per LSB) the sample above was taken at.
	 * Captured at fetch time rather than looked up at channel_get time so a
	 * SENSOR_ATTR_FULL_SCALE change between the two cannot make a sample
	 * taken at one range be converted at another.
	 */
	uint32_t sample_udps_per_lsb;

	/* Indices into the range/rate tables in l3gd20.c. */
	uint8_t range_idx;
	uint8_t odr_idx;

	/* CTRL_REG1 minus the PD bit; PD is applied from `powered`. */
	uint8_t ctrl1_cfg;

	/* True once resumed (PD=1). False in power-down. */
	bool powered;

	/* Uptime (ms) before which output is not trusted. */
	int64_t ready_at_ms;
};

#endif /* CAR_TRACKER_DRIVERS_SENSOR_L3GD20_L3GD20_H_ */
