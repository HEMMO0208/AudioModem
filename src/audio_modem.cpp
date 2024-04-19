#include "audio_modem.h"
#include "fsk.h"
#include <iostream>

int audio_modem::callback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    io_buffer* buffer = (io_buffer*)userData;

    buffer->input_buffer.push(inputBuffer);

    if (!buffer->output_buffer.empty()) {
        buffer->output_buffer.pop(outputBuffer);
    }

    else {
        std::fill((short*)outputBuffer, (short*)outputBuffer + framesPerBuffer, 0);
    }
    
    return paContinue;
}

void audio_modem::demod_callback() {
    std::vector<short> v;
    std::vector<char> received;
    packet* buff = new packet();

    while (demod_flag) {
        buffer->input_buffer.pop(v);

        if (v.size() < 4096) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        if (!m_device->is_synchronized()) {
            int result = m_device->sync(v);
            continue;
        }

        int ret = m_device->demoulate(v, received);

        buff->push(received);
        
        if (buff->header())
            m_signal_sender.packet_receiving(buff->packet_data().size(), buff->header()->len);

        if (buff->finished()) {
            m_packet_queue.push(buff);
            buff = new packet();
            m_signal_sender.packet_received();
        }

        if (ret == -1) {
            if (buff->header())
                m_signal_sender.packet_lost();

            buff->clear();
            received.clear();
        }
    }

    delete buff;
}

audio_modem::audio_modem(int chunk_size, int sample_rate, modem_device* device) {
    Pa_Initialize();
    
    if (!device)
        device = new fsk(sample_rate, 1225);

    buffer = new io_buffer(chunk_size);

    this->m_chunk_size = chunk_size;
    this->m_sample_rate = sample_rate;
    this->m_device = device;
    this->stream = NULL;

    this->m_input_device = Pa_GetDefaultInputDevice();
    this->m_output_device = Pa_GetDefaultOutputDevice();
}

audio_modem::~audio_modem() {
    stop_stream();
    stop_demodulate();

    if (m_device)
        delete m_device;

    delete buffer;

    Pa_Terminate();
}

bool audio_modem::start_stream() {
    if (stream != NULL)
        return false;

    const PaDeviceInfo *i_device_info, *o_device_info;
    i_device_info = Pa_GetDeviceInfo(m_input_device);
    o_device_info = Pa_GetDeviceInfo(m_output_device);

    if (i_device_info == NULL || o_device_info == NULL) {
        stream = NULL;
        return false;
    }

    PaStreamParameters i_params { m_input_device, 1, paInt16, i_device_info->defaultLowInputLatency, NULL };
    PaStreamParameters o_params { m_output_device, 1, paInt16, o_device_info->defaultLowOutputLatency, NULL };
    
    PaError result;

    result = Pa_OpenStream(&stream, &i_params, &o_params, m_sample_rate, m_chunk_size, paNoFlag, &callback, buffer);

    if (result != paNoError) {
        stream = NULL;
        return false;
    }

    result = Pa_StartStream(stream);

    if (result != paNoError) {
        Pa_CloseStream(stream);
        stream = NULL;

        return false;
    }

    return true;
}

bool audio_modem::stop_stream() {
    if (stream == NULL)
        return false;

    Pa_StopStream(stream);
    Pa_CloseStream(stream);

    stream = NULL;
    buffer->input_buffer.clear();
    buffer->output_buffer.clear();

    return true;
}

bool audio_modem::start_demodulate() {
    if (stream == NULL)
        return false;

    buffer->input_buffer.clear();

    demod_flag = true;
    auto thread = std::thread(&audio_modem::demod_callback, this);
    thread.detach();

    return true;
}

bool audio_modem::stop_demodulate() {
    demod_flag = false;

    buffer->input_buffer.clear();

    return true;
}

void audio_modem::modulate(char* src, size_t size) {
    std::vector<short> modulated;

    m_device->modulate(src, size, modulated);
    buffer->output_buffer.push(std::move(modulated));
}

void audio_modem::modulate(std::vector<char>& src) {
    modulate(src.data(), src.size());
}

void audio_modem::set_chunk_size(int chunk_size) {
    stop_demodulate();
    stop_stream();

    this->m_chunk_size = chunk_size;
}

void audio_modem::set_sample_rate(int sample_rate) {
    stop_demodulate();
    stop_stream();

    modem_device* tmp = m_device->new_device(sample_rate, m_device->baud_rate());
    delete this->m_device;

    this->m_device = tmp;
    this->m_sample_rate = sample_rate;
}

void audio_modem::set_modem_device(modem_device* device) {
    if (!device)
        return;

    stop_demodulate();
    stop_stream();

    delete this->m_device;
    this->m_device = device;
}

void audio_modem::set_io_device(PaDeviceIndex input_device, PaDeviceIndex output_device) {
    stop_demodulate();
    stop_stream();

    if (input_device != -1)
        this->m_input_device = input_device;

    if (output_device != -1)
        this->m_output_device = output_device;
}

void audio_modem::volume(double& input_volume, double& output_volume) {
    PxMixer* mixer = Px_OpenMixer(stream, m_input_device, m_output_device, 0);

    if (!mixer) {
        input_volume = -1.0;
        output_volume = -1.0;
    }

    input_volume = Px_GetInputVolume(mixer);
    output_volume = Px_GetMasterVolume(mixer);
    Px_CloseMixer(mixer);
}

void audio_modem::set_volume(const double& input_volume, const double& output_volume) {
    PxMixer* mixer = Px_OpenMixer(stream, m_input_device, m_output_device, 0);
    Px_SetInputVolume(mixer, input_volume);
    Px_SetMasterVolume(mixer, output_volume);
    Px_CloseMixer(mixer);
}

void audio_modem::devices(std::vector<const PaDeviceInfo*>& devices) {
    devices.clear();

    int device_count = Pa_GetDeviceCount();

    if (device_count <= 0)
        return;

    for (int i = 0; i < device_count; ++i) {
        devices.push_back(Pa_GetDeviceInfo(i));
    }
}

modem_config audio_modem::config() {
    double input_volume, output_volume;
    volume(input_volume, output_volume);

    return modem_config{ 
        m_input_device, 
        m_output_device, 
        m_sample_rate,
        m_chunk_size,
        m_device->baud_rate(),
        m_device->type(),
        input_volume,
        output_volume
    };
}

void audio_modem::set_config(const modem_config& config) {
    set_volume(config.input_volume, config.output_volume);

    if (
        m_input_device == config.input_device &&
        m_output_device == config.output_device &&
        m_chunk_size == config.chunk_size &&
        m_sample_rate == config.sample_rate &&
        m_device->baud_rate() == config.baud_rate &&
        m_device->type() == config.device_type) 
    {
        return;
    }

    stop_demodulate();
    stop_stream();
     
    if (m_chunk_size != config.chunk_size) {
        m_chunk_size = config.chunk_size;
        delete buffer;
        buffer = new io_buffer(m_chunk_size);
    }

    m_input_device = config.input_device;
    m_output_device = config.output_device;
    m_sample_rate = config.sample_rate;

    delete m_device;
    m_device = modem_device::new_device(config.device_type, m_sample_rate, config.baud_rate);
    m_signal_sender.configuration_changed();
}