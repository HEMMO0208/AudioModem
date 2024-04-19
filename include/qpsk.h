#pragma once
#include "modem_device.h"
#include <vector>
#include <string>

constexpr double sqr = 0.70710678118;

class qpsk : public modem_device
{
private:
	int samples_per_baud;
	int min_samples;
	int max_volume;
	int received;
	double threshold;

	std::string buff;
	std::vector<double> _cos, _sin;

public:
	qpsk(int sample_rate = 48000, int baud_rate = 600);

	int sync(std::vector<short>& v);
	int demoulate(std::vector<short>& v, std::vector<char>& dst);
	void modulate(char* src, size_t size, std::vector<short>& dst);
	modem_device* new_device(int sample_rate, int baud_rate) { return new qpsk(sample_rate, baud_rate); }
	modem_type type() { return modem_type::qpsk; }

	void phase(std::vector<short>& v, size_t idx, double& cos, double& sin, size_t len = 0);
	void write(double cos, double sin, std::vector<short>& dst);
};

