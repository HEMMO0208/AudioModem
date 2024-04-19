#pragma once
#include <vector>
#include <mutex>
#include <QtCore/QObject>

constexpr size_t MAX_FILENAME_LEN = 256;

enum packet_control {
	text = 0b00000001,
	file = 0b00000010
};

#pragma pack(push, 1)
struct packet_header {
	uint8_t id;
	uint8_t control;
	uint16_t len;
	uint16_t checksum;

	packet_header() : id(0), control(0), len(0), checksum(0) {}
	bool operator==(packet_header& other) { return memcmp(this, &other, sizeof(packet_header)) == 0; }
};

struct file_header {
	char file_name[MAX_FILENAME_LEN];
	int data_length;

	file_header(const std::string& file_name = "", int len = 0) : data_length(len) { 
		std::strcpy(this->file_name, file_name.c_str());
	}
};
#pragma pack(pop)

constexpr size_t header_size = sizeof(packet_header);
constexpr size_t file_header_size = sizeof(file_header);
constexpr size_t max_packet_size = UINT16_MAX;
constexpr size_t max_data_size = max_packet_size - header_size;

class packet
{
private:
	std::vector<char> m_data;

public:
	packet() {}
	packet(packet_header& header, const std::vector<char>& data);
	packet(const packet& p) : m_data(p.m_data) {}
	packet(packet&& p) noexcept : m_data(std::move(p.m_data)) {}

	packet& operator=(const packet& p);
	packet& operator=(packet&& p) noexcept;

	void clear() { m_data.clear(); }
	void push(const void* src, size_t size);
	void push(std::vector<char>& v);
	bool finished();
	bool valid();
	size_t size() { return m_data.size(); }
	uint16_t checksum();

	packet_header* header();
	char* data() { return m_data.data() + header_size;  }
	std::vector<char>& packet_data() { return m_data; }
};

struct packet_queue {
	std::vector<packet*> data;
	std::mutex mtx;

	bool empty();
	void push(packet*);
	packet* pop();

	packet_queue() {}
};

