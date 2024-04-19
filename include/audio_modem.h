#pragma once

#include <thread>
#include <atomic>
#include "portaudio.h"
#include "buffer.h"
#include "modem_device.h"
#include "packet.h"
#include "modem_signal_sender.h"
#include "portmixer.h"

struct modem_config {
	PaDeviceIndex input_device;
	PaDeviceIndex output_device;
	int sample_rate;
	int chunk_size;
	int baud_rate;
	modem_type device_type;
	double input_volume;
	double output_volume;
};

struct io_buffer {
	circular_buffer input_buffer;
	array_buffer output_buffer;

	io_buffer(int chunk_size) : input_buffer(chunk_size), output_buffer(chunk_size) {}
};

class audio_modem
{
private:
	PaStream* stream;
	modem_device* m_device;
	io_buffer* buffer;
	std::atomic_bool demod_flag;
	packet_queue m_packet_queue;
	PaDeviceIndex m_input_device, m_output_device;
	modem_signal_sender m_signal_sender;

	static PaStreamCallback callback;
	void demod_callback();

	int m_sample_rate;
	int m_chunk_size;

public:
	audio_modem(int chunk_size, int sample_rate, modem_device* device = NULL);
	~audio_modem();

	bool start_stream();
	bool stop_stream();
	bool start_demodulate();
	bool stop_demodulate();

	void modulate(char* src, size_t size);
	void modulate(std::vector<char>& src);

	void set_chunk_size(int chunk_size);
	void set_sample_rate(int sample_rate);
	void set_modem_device(modem_device* device);
	void set_io_device(PaDeviceIndex input_device = -1, PaDeviceIndex output_device = -1);
	void volume(double& input_volume, double& output_volume);
	void set_volume(const double& input_volume, const double& output_volume);

	packet_queue& get_packet_queue() { return m_packet_queue; }
	modem_signal_sender* get_signal() { return &m_signal_sender; }

	int chunk_size() { return m_chunk_size; }
	int sample_rate() { return m_sample_rate; }
	int baud_rate() { return m_device->baud_rate(); }
	bool audio_stream_operating() { return stream != NULL; }
	bool demodulation_operating() { return demod_flag == true; }
	PaDeviceIndex get_input_device() { return m_input_device; }
	PaDeviceIndex get_output_device() { return m_output_device; }

	void devices(std::vector<const PaDeviceInfo*>& devices);
	modem_config config();
	void set_config(const modem_config& config);
};

