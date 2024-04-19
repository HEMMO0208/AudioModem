#include "fsk.h"
#include <cmath>
#include <bitset>

fsk::fsk(int sample_rate, int baud_rate) : modem_device(sample_rate, baud_rate) {
	samples_per_baud = sample_rate / baud_rate;
	min_samples = 20 * samples_per_baud;
	max_volume = 32767;
	threshold = 0.5;
	received = 0;

	hi_cos.assign(samples_per_baud, 0);
	hi_sin.assign(samples_per_baud, 0);
	lo_cos.assign(samples_per_baud, 0);
	lo_sin.assign(samples_per_baud, 0);

	high.assign(samples_per_baud, 0);
	low.assign(samples_per_baud, 0);

	for (int i = 0; i < samples_per_baud; ++i) {
		double theta = 2 * pi * i / samples_per_baud;

		hi_cos[i] = std::cos(2 * theta);
		hi_sin[i] = std::sin(2 * theta);
		lo_cos[i] = std::cos(theta);
		lo_sin[i] = std::sin(theta);

		high[i] = hi_cos[i] * max_volume * 0.9;
		low[i] = lo_cos[i] * max_volume * 0.9;
	}
}

void fsk::fft(std::vector<short>& v, size_t idx, double& hi, double& lo) {
	double hi_c = 0, lo_c = 0;
	double hi_s = 0, lo_s = 0;

	for (int i = 0; i < samples_per_baud; ++i) {
		double x = (double)v[idx + i] / max_volume;
		
		hi_c += hi_cos[i] * x;
		hi_s += hi_sin[i] * x;
		lo_c += lo_cos[i] * x;
		lo_s += lo_sin[i] * x;
	}

	hi_c = hi_c * 2 / samples_per_baud;
	hi_s = hi_s * 2 / samples_per_baud;
	lo_c = lo_c * 2 / samples_per_baud;
	lo_s = lo_s * 2 / samples_per_baud;

	hi = std::sqrt(hi_c * hi_c + hi_s * hi_s);
	lo = std::sqrt(lo_c * lo_c + lo_s * lo_s);
}

void fsk::phase(std::vector<short>& v, size_t idx, double& cos, double& sin, size_t len) {
	cos = 0, sin = 0;

	if (len == 0)
		len = samples_per_baud;

	for (int i = 0; i < len; ++i) {
		double x = (double)v[idx + i] / max_volume;
		int t = i % samples_per_baud;

		cos += lo_cos[t] * x;
		sin -= lo_sin[t] * x;
	}

	cos = cos * 2 / len;
	sin = sin * 2 / len;
}

int fsk::sync(std::vector<short>& v) {
	if (v.size() < min_samples)
		return -1;

	size_t idx;
	double cos, sin;
	double hi, lo;
	bool detected = false;

	for (idx = 0; idx + min_samples < v.size(); idx += samples_per_baud) {
		phase(v, idx, cos, sin);
		double power = std::sqrt(cos * cos + sin * sin);

		if (power > threshold) {
			detected = true;
			idx += samples_per_baud;
			break;
		}
	}

	if (!detected) {
		v.erase(v.begin(), v.begin() + idx);
		return -1;
	}

	size_t max_idx = 0;
	double max_val = -inf;

	for (int i = idx; i < idx + samples_per_baud; ++i) {
		phase(v, i, cos, sin, samples_per_baud * 2LL);
		double val = cos - std::abs(sin);

		if (val > max_val) {
			max_val = val;
			max_idx = i;
		}
	}

	for (idx = max_idx; idx + min_samples < v.size(); idx += samples_per_baud) {
		fft(v, idx, hi, lo);
		if (hi > lo) {
			v.erase(v.begin(), v.begin() + idx + samples_per_baud);
			synchronized = true;
			return 1;
		}
	}

	v.erase(v.begin(), v.begin() + idx);
	return  -1;
}

int fsk::demoulate(std::vector<short>& v, std::vector<char>& dst) {
	double hi, lo;
	size_t idx;

	for (idx = 0; idx + min_samples < v.size(); idx += samples_per_baud) {
		fft(v, idx, hi, lo);

		if (std::max(hi, lo) < threshold) {
			synchronized = false;
			buff = "";
			received = 0;

			v.erase(v.begin(), v.begin() + idx + samples_per_baud);
			return -1;
		}

		buff += hi > lo ? '1' : '0';
		
		if (buff.size() == 8) {
			unsigned char c = std::bitset<8>(buff).to_ulong();
			dst.push_back(c);
			buff = "";
			received += 1;
		}

		if (received == 128) {
			received = 0;
			synchronized = false;
			v.erase(v.begin(), v.begin() + idx + samples_per_baud);
			
			return 1;
		}
	}

	v.erase(v.begin(), v.begin() + idx);
	return 1;
}

void fsk::modulate(char* src, size_t size, std::vector<short>& dst) {
	for (int i = 0; i < size; ++i) {
		if (i % 128 == 0) {
			dst.insert(dst.end(), low.begin(), low.end());
			dst.insert(dst.end(), low.begin(), low.end());
			dst.insert(dst.end(), low.begin(), low.end());
			dst.insert(dst.end(), low.begin(), low.end());
			dst.insert(dst.end(), low.begin(), low.end());
			dst.insert(dst.end(), low.begin(), low.end());
			dst.insert(dst.end(), high.begin(), high.end());
		}

		std::bitset<8> bits(src[i]);

		for (int i = 7; i >= 0; --i) {
			if (bits[i])
				dst.insert(dst.end(), high.begin(), high.end());

			else
				dst.insert(dst.end(), low.begin(), low.end());
		}
	}

	dst.insert(dst.end(), low.begin(), low.end());
	dst.insert(dst.end(), low.begin(), low.end());
}