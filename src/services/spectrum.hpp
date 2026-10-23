#pragma once

#include <cstddef>
#include <cstdint>

/*
 * Frequency-domain analysis of a short accelerometer capture.
 *
 * Pure integer maths, no Zephyr dependency (so it can be unit-tested on a
 * host compiler): the build has no FPU, and soft-float would be both slow and
 * pointless for this. It reduces three axes of acceleration to the single
 * dominant vibration frequency, which is what tells an idling engine
 * (steady tone near its firing rate) apart from a door slam (broadband) or a
 * still car (nothing).
 *
 * Frequency resolution is sample_rate / kWindowSize (0.39 Hz at 100 Hz), and
 * nothing above sample_rate / 2 (the Nyquist limit) can be seen -- a faster
 * vibration folds back down and is reported at the wrong frequency. Choose the
 * sensor's output data rate with that ceiling in mind.
 */
namespace ct::spectrum {

/* Samples per analysis window. Must stay a power of two (radix-2 FFT) and
 * must match the precomputed tables in spectrum.cpp.
 */
inline constexpr std::size_t kWindowSize = 256;

struct Result {
	/* False if the capture was too still to say anything: the dominant "peak"
	 * of pure sensor noise is a random bin, and printing it as a frequency
	 * would be actively misleading.
	 */
	bool significant;

	/* Dominant frequency in millihertz (1500 == 1.5 Hz). Valid only if
	 * `significant`.
	 */
	std::uint32_t freq_mhz;

	/* Share of the total non-DC power sitting in the dominant bin and its two
	 * neighbours. High (>50%) means a clean tone; low means broadband noise or
	 * several competing frequencies, and the "dominant" bin is weak evidence.
	 */
	std::uint32_t peak_share_pct;

	/* RMS of the dynamic (gravity-removed) acceleration across all three axes,
	 * in mm/s^2.
	 */
	std::int32_t rms_mms2;
};

/* x, y, z: kWindowSize samples each, in mm/s^2, taken at a uniform
 * `sample_rate_hz`. Gravity/DC is removed internally, per axis, so no
 * particular mounting orientation is assumed. Captures whose dynamic RMS is
 * below `min_rms_mms2` are reported as not significant.
 */
Result analyze(const std::int16_t *x, const std::int16_t *y, const std::int16_t *z,
	       std::uint32_t sample_rate_hz, std::int32_t min_rms_mms2);

} /* namespace ct::spectrum */
