#include "packet.h"
#include <fstream>
#include <random>

packet::packet(packet_header& header, const std::vector<char>& data) {
	this->m_data.insert(this->m_data.end(), (char*)&header, (char*)&header + header_size);
	this->m_data.insert(this->m_data.end(), data.begin(), data.end());

	this->header()->len = this->m_data.size();
	this->header()->checksum = this->checksum();
}

void packet::push(const void* src, size_t size) {
	m_data.insert(m_data.end(), (char*)src, (char*)src + size);
}

void packet::push(std::vector<char>& v) {
	if (m_data.size() == 0) {
		if (v.size() < header_size)
			return;

		else {
			m_data.insert(m_data.end(), v.begin(), v.begin() + header_size);
			v.erase(v.begin(), v.begin() + header_size);
		}
	}

	size_t size_to_push = std::min(v.size(), m_data.size() - (size_t)header()->len);
	m_data.insert(m_data.end(), v.begin(), v.begin() + size_to_push);
	v.erase(v.begin(), v.begin() + size_to_push);
}

bool packet::finished() {
	if (m_data.size() == 0)
		return false;

	else
		return header()->len == m_data.size();
}

bool packet::valid() {
	if (m_data.size() == 0)
		return false;

	else
		return header()->checksum == checksum();
}

packet_header* packet::header() {
	if (m_data.size() == 0)
		return NULL;

	return (packet_header*)(m_data.data());
}

uint16_t packet::checksum() {
	if (!finished())
		return 0;

	uint16_t ret = 0;
	uint16_t tmp;
	uint16_t old = header()->checksum;
	header()->checksum = 0;

	for (int i = 0; i < m_data.size() / 2; ++i) {
		tmp = ((uint16_t*)m_data.data())[i];
		ret += tmp;
		if (ret < tmp)
			ret += 1;
	}

	if (m_data.size() % 2 != 0) {
		ret += m_data[m_data.size() - 1];
		if (ret < m_data[m_data.size() - 1])
			ret += 1;
	}

	ret = ~ret;
	header()->checksum = old;
	return ret;
}

packet& packet::operator=(const packet& p) {
	if (&p == this)
		return *this;

	m_data = p.m_data;

	return *this;
}

packet& packet::operator=(packet&& p) noexcept {
	if (&p == this)
		return *this;

	m_data = std::move(p.m_data);

	return *this;
}

void packet_queue::push(packet* p) {
	mtx.lock();

	data.push_back(p);

	mtx.unlock();
}

packet* packet_queue::pop() {
	packet* ret;

	mtx.lock();
	
	if (data.size() == 0) {
		mtx.unlock();
		return NULL;
	}

	ret = data[0];
	data.erase(data.begin());

	mtx.unlock();

	return ret;
}

bool packet_queue::empty() {
	mtx.lock();

	bool ret = data.size() == 0;

	mtx.unlock();

	return ret;
}