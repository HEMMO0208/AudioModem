#include "qpsk.h"

#include <cmath>
#include <bitset>

qpsk::qpsk(int sample_rate, int baud_rate) : modem_device(sample_rate, baud_rate) {
	samples_per_baud = 2 * sample_rate / baud_rate;
	min_samples = 20 * samples_per_baud;
	max_volume = INT16_MAX;
	threshold = 0.5;
	received = 0;

	_cos.assign(samples_per_baud, 0);
	_sin.assign(samples_per_baud, 0);

	for (int i = 0; i < samples_per_baud; ++i) {
		double theta = 2 * pi * i / samples_per_baud;
		_cos[i] = std::cos(theta);
		_sin[i] = std::sin(theta);
	}
}

void qpsk::phase(std::vector<short>& v, size_t idx, double& cos, double& sin, size_t len) {
	cos = 0, sin = 0;

	if (len == 0)
		len = samples_per_baud;

	for (int i = 0; i < len; ++i) {
		double x = (double)v[idx + i] / max_volume;
		int t = i % samples_per_baud;

		cos += _cos[t] * x;
		sin -= _sin[t] * x;
	}

	cos = cos * 2 / len;
	sin = sin * 2 / len;
}

int qpsk::sync(std::vector<short>& v) {
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
		phase(v, idx, cos, sin);
		if (cos < 0 && sin < 0) {
			v.erase(v.begin(), v.begin() + idx + samples_per_baud);
			synchronized = true;
			return 1;
		}
	}

	v.erase(v.begin(), v.begin() + idx);
	return  -1;
}

int qpsk::demoulate(std::vector<short>& v, std::vector<char>& dst) {
	double cos, sin;
	size_t idx;

	for (idx = 0; idx + min_samples < v.size(); idx += samples_per_baud) {
		phase(v, idx, cos, sin);
		double power = std::sqrt(cos * cos + sin * sin);

		if (power < threshold) {
			synchronized = false;
			buff = "";
			received = 0;

			v.erase(v.begin(), v.begin() + idx + samples_per_baud);
			return -1;
		}

		buff += cos > 0 ? '1' : '0';
		buff += sin > 0 ? '1' : '0';

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

void qpsk::modulate(char* src, size_t size, std::vector<short>& dst) {
	double cos, sin;

	for (int i = 0; i < size; ++i) {
		if (i % 128 == 0) {
			write(1, 0, dst);
			write(1, 0, dst);
			write(1, 0, dst);
			write(1, 0, dst);
			write(1, 0, dst);
			write(1, 0, dst);
			write(-sqr, -sqr, dst);
		}

		std::bitset<8> bits(src[i]);

		for (int i = 7; i >= 0; i -= 2) {
			cos = bits[i] ? sqr : -sqr;
			sin = bits[i - 1] ? sqr : -sqr;

			write(cos, sin, dst);
		}
	}

	write(1, 0, dst);
}

void qpsk::write(double cos, double sin, std::vector<short>& dst) {
	size_t idx = dst.size();

	dst.insert(dst.end(), samples_per_baud, 0);

	for (int i = 0; i < samples_per_baud; ++i) {
		dst[idx + i] = max_volume * (_cos[i] * cos - _sin[i] * sin) * 0.9;
	}
}