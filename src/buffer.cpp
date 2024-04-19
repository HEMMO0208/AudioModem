#include <buffer.h>
#include <iostream>

bool circular_buffer::empty() {
	bool ret;

	m.lock();
	ret = (size == 0);
	m.unlock();

	return ret;
}

void circular_buffer::push(std::vector<data_type>& src) {
	if (src.size() > chunk_size)
		return;

	m.lock();

	std::copy(src.begin(), src.end(), data.begin() + (tail * chunk_size));
	tail = (tail + 1) % ring_size;

	if (size != ring_size)
		size += 1;

	m.unlock();
}

void circular_buffer::push(const void* src) {
	m.lock();

	std::copy((data_type*)src, (data_type*)src + chunk_size, data.begin() + (tail * chunk_size));
	tail = (tail + 1) % ring_size;

	if (size != ring_size)
		size += 1;

	m.unlock();
}

void circular_buffer::pop(std::vector<data_type>& dst) {
	m.lock();

	if (size == 0) {
		m.unlock();
		return;
	}

	int idx = (tail - size + ring_size) % ring_size;
	idx *= chunk_size;
	size -= 1;

	dst.insert(dst.end(), data.begin() + idx, data.begin() + idx + chunk_size);

	m.unlock();
}

void circular_buffer::pop(void* dst) {
	m.lock();

	if (size == 0) {
		m.unlock();
		return;
	}

	int idx = (tail - size + ring_size) % ring_size;
	idx *= chunk_size;
	size -= 1;

	std::copy(data.begin() + idx, data.begin() + idx + chunk_size, (data_type*)dst);

	m.unlock();
}

void circular_buffer::clear() {
	m.lock();

	size = 0;
	tail = 0;

	m.unlock();
}

bool array_buffer::empty() {
	bool ret;

	m.lock();
	ret = (queue.size() == 0);
	m.unlock();

	return ret;
}

void array_buffer::push(std::vector<data_type>&& src) {
	int pad = chunk_size - (src.size() % chunk_size);
	pad = pad % chunk_size;

	src.insert(src.end(), pad, 0);
	src.push_back(0);

	m.lock();

	queue.push_back(std::move(src));

	m.unlock();
}

void array_buffer::pop(void* dst) {
	m.lock();

	if (queue.size() == 0) {
		m.unlock();
		return;
	}

	std::vector<data_type>& data = queue[0];
	size_t idx = data[data.size() - 1];

	std::copy(data.begin() + idx * chunk_size, data.begin() + ((idx + 1) * chunk_size), (data_type*)dst);

	data[data.size() - 1] += 1;

	if ((idx + 1) * chunk_size >= data.size() - 1)
		queue.erase(queue.begin());

	m.unlock();
}

void array_buffer::clear() {
	m.lock();

	queue.clear();

	m.unlock();
}