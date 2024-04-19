#pragma once
#include <vector>
#include <string>

constexpr double pi = 3.1415926535897931;
constexpr int inf = 987654321;
constexpr int modem_type_count = 2;
constexpr const char* modem_types[modem_type_count] = { "FSK", "QPSK"};

enum class modem_type { 
	fsk, 
	qpsk 
};

class modem_device {
protected:
	bool synchronized;
	int m_sample_rate;
	int m_baud_rate;

public:
	modem_device(int sample_rate, int baud_rate) : synchronized(false), m_sample_rate(sample_rate), m_baud_rate(baud_rate) {};
	static modem_device* new_device(modem_type type, int sample_rate, int baud_rate);

	virtual int sync(std::vector<short>& v) = 0;
	virtual int demoulate(std::vector<short>& v, std::vector<char>& dst) = 0;
	virtual void modulate(char* src, size_t size, std::vector<short>& dst) = 0;
	virtual modem_device* new_device(int sample_rate, int baud_rate) = 0;
	virtual modem_type type() = 0;

	void modulate(std::vector<char>& src, std::vector<short>& dst) { modulate(src.data(), src.size(), dst); }
	int sample_rate() { return m_sample_rate; }
	int baud_rate() { return m_baud_rate; }
	bool is_synchronized() { return synchronized;  }
};