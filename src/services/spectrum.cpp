#include "spectrum.hpp"

namespace ct::spectrum {

namespace {

constexpr std::size_t N = kWindowSize;
static_assert((N & (N - 1)) == 0 && N == 256, "tables below are for a 256-point FFT");

/* Periodic Hann window, Q15: 0.5 * (1 - cos(2*pi*n/N)). Tapers the capture to
 * zero at both ends so the tone's energy stays in a few bins instead of
 * smearing across the spectrum (spectral leakage).
 */
constexpr std::int16_t kHann[N] = {
	0, 5, 20, 44, 79, 123, 177, 241, 315, 398, 491, 593,
	705, 827, 958, 1098, 1247, 1406, 1573, 1749, 1935, 2128, 2331, 2542,
	2761, 2989, 3224, 3468, 3719, 3978, 4244, 4518, 4799, 5086, 5381, 5682,
	5990, 6304, 6624, 6950, 7281, 7618, 7961, 8308, 8660, 9017, 9379, 9744,
	10114, 10487, 10864, 11244, 11628, 12014, 12403, 12794, 13187, 13583, 13980, 14378,
	14778, 15178, 15580, 15981, 16383, 16786, 17187, 17589, 17989, 18389, 18787, 19184,
	19580, 19973, 20364, 20753, 21139, 21523, 21903, 22280, 22653, 23023, 23388, 23750,
	24107, 24459, 24806, 25149, 25486, 25817, 26143, 26463, 26777, 27085, 27386, 27681,
	27968, 28249, 28523, 28789, 29048, 29299, 29543, 29778, 30006, 30225, 30436, 30639,
	30832, 31018, 31194, 31361, 31520, 31669, 31809, 31940, 32062, 32174, 32276, 32369,
	32452, 32526, 32590, 32644, 32688, 32723, 32747, 32762, 32767, 32762, 32747, 32723,
	32688, 32644, 32590, 32526, 32452, 32369, 32276, 32174, 32062, 31940, 31809, 31669,
	31520, 31361, 31194, 31018, 30832, 30639, 30436, 30225, 30006, 29778, 29543, 29299,
	29048, 28789, 28523, 28249, 27968, 27681, 27386, 27085, 26777, 26463, 26143, 25817,
	25486, 25149, 24806, 24459, 24107, 23750, 23388, 23023, 22653, 22280, 21903, 21523,
	21139, 20753, 20364, 19973, 19580, 19184, 18787, 18389, 17989, 17589, 17187, 16786,
	16384, 15981, 15580, 15178, 14778, 14378, 13980, 13583, 13187, 12794, 12403, 12014,
	11628, 11244, 10864, 10487, 10114, 9744, 9379, 9017, 8660, 8308, 7961, 7618,
	7281, 6950, 6624, 6304, 5990, 5682, 5381, 5086, 4799, 4518, 4244, 3978,
	3719, 3468, 3224, 2989, 2761, 2542, 2331, 2128, 1935, 1749, 1573, 1406,
	1247, 1098, 958, 827, 705, 593, 491, 398, 315, 241, 177, 123,
	79, 44, 20, 5,
};

/* Twiddle factors for k = 0..N/2-1, Q15: cos(2*pi*k/N) and sin(2*pi*k/N).
 * (cos(0) is 1.0, which does not fit Q15 -- stored as 32767, an error of
 * 0.003%.)
 */
constexpr std::int16_t kCos[N / 2] = {
	32767, 32758, 32729, 32679, 32610, 32522, 32413, 32286, 32138, 31972, 31786, 31581,
	31357, 31114, 30853, 30572, 30274, 29957, 29622, 29269, 28899, 28511, 28106, 27684,
	27246, 26791, 26320, 25833, 25330, 24812, 24279, 23732, 23170, 22595, 22006, 21403,
	20788, 20160, 19520, 18868, 18205, 17531, 16846, 16151, 15447, 14733, 14010, 13279,
	12540, 11793, 11039, 10279, 9512, 8740, 7962, 7180, 6393, 5602, 4808, 4011,
	3212, 2411, 1608, 804, 0, -804, -1608, -2411, -3212, -4011, -4808, -5602,
	-6393, -7180, -7962, -8740, -9512, -10279, -11039, -11793, -12540, -13279, -14010, -14733,
	-15447, -16151, -16846, -17531, -18205, -18868, -19520, -20160, -20788, -21403, -22006, -22595,
	-23170, -23732, -24279, -24812, -25330, -25833, -26320, -26791, -27246, -27684, -28106, -28511,
	-28899, -29269, -29622, -29957, -30274, -30572, -30853, -31114, -31357, -31581, -31786, -31972,
	-32138, -32286, -32413, -32522, -32610, -32679, -32729, -32758,
};

constexpr std::int16_t kSin[N / 2] = {
	0, 804, 1608, 2411, 3212, 4011, 4808, 5602, 6393, 7180, 7962, 8740,
	9512, 10279, 11039, 11793, 12540, 13279, 14010, 14733, 15447, 16151, 16846, 17531,
	18205, 18868, 19520, 20160, 20788, 21403, 22006, 22595, 23170, 23732, 24279, 24812,
	25330, 25833, 26320, 26791, 27246, 27684, 28106, 28511, 28899, 29269, 29622, 29957,
	30274, 30572, 30853, 31114, 31357, 31581, 31786, 31972, 32138, 32286, 32413, 32522,
	32610, 32679, 32729, 32758, 32767, 32758, 32729, 32679, 32610, 32522, 32413, 32286,
	32138, 31972, 31786, 31581, 31357, 31114, 30853, 30572, 30274, 29957, 29622, 29269,
	28899, 28511, 28106, 27684, 27246, 26791, 26320, 25833, 25330, 24812, 24279, 23732,
	23170, 22595, 22006, 21403, 20788, 20160, 19520, 18868, 18205, 17531, 16846, 16151,
	15447, 14733, 14010, 13279, 12540, 11793, 11039, 10279, 9512, 8740, 7962, 7180,
	6393, 5602, 4808, 4011, 3212, 2411, 1608, 804,
};

/* Working buffers: static so nothing lands on a thread stack (these are
 * ~3 KB) and nothing touches the heap. Not reentrant -- analyze() must be
 * called from one thread at a time, which its only caller does.
 */
std::int32_t re[N];
std::int32_t im[N];
std::uint64_t power[N / 2];

/* In-place radix-2 decimation-in-time FFT. Q15 twiddles, int64 products, no
 * per-stage scaling: inputs are at most ~2^15 and the 8 stages can add at most
 * 8 bits, so every value stays well inside int32.
 */
void fft()
{
	/* Bit-reversal permutation. */
	for (std::size_t i = 1, j = 0; i < N; ++i) {
		std::size_t bit = N >> 1;

		for (; j & bit; bit >>= 1) {
			j ^= bit;
		}
		j ^= bit;

		if (i < j) {
			const std::int32_t tr = re[i];
			const std::int32_t ti = im[i];

			re[i] = re[j];
			im[i] = im[j];
			re[j] = tr;
			im[j] = ti;
		}
	}

	for (std::size_t len = 2; len <= N; len <<= 1) {
		const std::size_t half = len >> 1;
		const std::size_t step = N / len;

		for (std::size_t i = 0; i < N; i += len) {
			for (std::size_t k = 0; k < half; ++k) {
				/* w = cos - j*sin (forward transform). */
				const std::int64_t wr = kCos[k * step];
				const std::int64_t wi = -static_cast<std::int64_t>(kSin[k * step]);
				const std::size_t u = i + k;
				const std::size_t v = u + half;

				const std::int32_t tr = static_cast<std::int32_t>(
					(re[v] * wr - im[v] * wi) >> 15);
				const std::int32_t ti = static_cast<std::int32_t>(
					(re[v] * wi + im[v] * wr) >> 15);

				re[v] = re[u] - tr;
				im[v] = im[u] - ti;
				re[u] += tr;
				im[u] += ti;
			}
		}
	}
}

std::uint32_t isqrt(std::uint64_t v)
{
	std::uint64_t r = 0;

	for (std::uint64_t bit = 1ULL << 62; bit != 0; bit >>= 2) {
		if (v >= r + bit) {
			v -= r + bit;
			r = (r >> 1) + bit;
		} else {
			r >>= 1;
		}
	}

	return static_cast<std::uint32_t>(r);
}

} /* namespace */

Result analyze(const std::int16_t *x, const std::int16_t *y, const std::int16_t *z,
	       std::uint32_t sample_rate_hz, std::int32_t min_rms_mms2)
{
	const std::int16_t *axes[3] = {x, y, z};
	std::uint64_t sumsq = 0; /* of the gravity-removed signal, all axes */

	for (auto &p : power) {
		p = 0;
	}

	for (const std::int16_t *a : axes) {
		/* Remove DC (gravity plus any constant offset) so it cannot swamp
		 * the spectrum and so the result is independent of orientation.
		 */
		std::int64_t sum = 0;

		for (std::size_t n = 0; n < N; ++n) {
			sum += a[n];
		}

		const std::int32_t mean = static_cast<std::int32_t>(sum / static_cast<std::int64_t>(N));

		for (std::size_t n = 0; n < N; ++n) {
			const std::int32_t d = a[n] - mean;

			sumsq += static_cast<std::uint64_t>(static_cast<std::int64_t>(d) * d);
			re[n] = static_cast<std::int32_t>((static_cast<std::int64_t>(d) * kHann[n]) >> 15);
			im[n] = 0;
		}

		fft();

		/* Power spectrum, summed across axes so the verdict does not depend on
		 * which axis the vibration happens to lie along. Bin 0 (DC) is
		 * skipped; the window above has already driven it to ~0.
		 */
		for (std::size_t k = 1; k < N / 2; ++k) {
			power[k] += static_cast<std::uint64_t>(static_cast<std::int64_t>(re[k]) * re[k]) +
				    static_cast<std::uint64_t>(static_cast<std::int64_t>(im[k]) * im[k]);
		}
	}

	Result r{};

	r.rms_mms2 = static_cast<std::int32_t>(isqrt(sumsq / (3 * N)));

	if (r.rms_mms2 < min_rms_mms2) {
		return r; /* too still: significant stays false */
	}

	std::size_t peak = 1;
	std::uint64_t total = 0;

	for (std::size_t k = 1; k < N / 2; ++k) {
		total += power[k];

		if (power[k] > power[peak]) {
			peak = k;
		}
	}

	std::uint64_t near = power[peak];

	if (peak > 1) {
		near += power[peak - 1];
	}
	if (peak + 1 < N / 2) {
		near += power[peak + 1];
	}

	r.significant = true;
	r.freq_mhz = static_cast<std::uint32_t>(static_cast<std::uint64_t>(peak) * sample_rate_hz *
						1000ULL / N);
	r.peak_share_pct = total == 0 ? 0 : static_cast<std::uint32_t>(near * 100 / total);

	return r;
}

} /* namespace ct::spectrum */
