/*
 * Message types carried on the zbus channels.
 *
 * These are deliberately plain aggregates (trivially copyable, no pointers,
 * no ownership): zbus moves messages by value with memcpy, so anything with
 * a non-trivial copy constructor or an owning pointer would be unsafe here.
 * static_asserts below enforce that at compile time rather than leaving it
 * to reviewer discipline.
 */

#pragma once

#include <cstdint>
#include <type_traits>

namespace ct {

/* A GNSS position/velocity sample. Units match Zephyr's navigation_data so
 * no lossy conversion happens on the acquisition path; formatting to
 * human/JSON units is done once, at the point of publishing.
 */
struct GpsFix {
	std::int64_t latitude_ndeg;   /* nanodegrees */
	std::int64_t longitude_ndeg;  /* nanodegrees */
	std::int32_t altitude_mm;     /* mm above MSL */
	std::uint32_t speed_mms;      /* mm/s */
	std::uint32_t bearing_mdeg;   /* millidegrees */
	std::uint32_t uptime_s;       /* device uptime when sampled */
	std::uint16_t satellites;
	bool valid;                   /* false => no fix; other fields meaningless */
};

/* Debounced motion verdict. Owned by the GPS task: it already has every fix
 * in hand as it arrives, so the debounce lives right next to the data it
 * debounces rather than being re-derived by a separate consumer. Whether
 * this verdict should *gate* anything (e.g. a telemetry publish) is a
 * separate policy question left to whoever consumes it.
 */
struct MotionState {
	bool moving;
};

/* Result of one accelerometer vibration capture (a 2.56 s FFT window).
 * Owned by the IMU task. Published once per window, so roughly every 2.6 s.
 * Distinct from MotionState: that is speed-derived (GPS), this is
 * vibration-derived (accelerometer) and says nothing about whether the car
 * is moving.
 */
struct VibrationState {
	bool significant;                /* false => too still to name a frequency */
	std::uint32_t freq_mhz;          /* dominant frequency, millihertz; valid iff significant */
	std::uint32_t peak_share_pct;    /* % of non-DC power in the dominant bin +/-1 */
	std::int32_t rms_mms2;           /* gravity-removed RMS over all axes, mm/s^2 */
};

/* Cellular -> observers: current WAN link state. `connected` is true for as
 * long as the cellular task is using the modem (one whole attempt, including
 * power-up and power-down), which is also what the power task reads as "do
 * not sleep now".
 */
struct LinkState {
	bool connected;
};

/* IMU -> observers: the accelerometer's any-motion interrupt fired. The
 * payload is only a timestamp; the event itself is the information. Consumed
 * by the power task to measure inactivity.
 */
struct ImuMotion {
	std::uint32_t uptime_ms;
};

/* zbus copies messages byte-wise; guarantee that is well-defined. */
static_assert(std::is_trivially_copyable_v<GpsFix>);
static_assert(std::is_trivially_copyable_v<MotionState>);
static_assert(std::is_trivially_copyable_v<VibrationState>);
static_assert(std::is_trivially_copyable_v<LinkState>);
static_assert(std::is_trivially_copyable_v<ImuMotion>);

} /* namespace ct */
