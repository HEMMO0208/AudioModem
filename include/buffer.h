#pragma once
#include <mutex>
#include <vector>

using data_type = short;

class circular_buffer
{
private:
	const int ring_size;
	const int chunk_size;

	int size;
	int tail;
	std::vector<data_type> data;
	std::mutex m;

public:
	circular_buffer(int chunk_size, int ring_size = 128) : ring_size(ring_size), chunk_size(chunk_size),
		data((size_t)ring_size * chunk_size), size(0), tail(0) {}

	bool empty();
	void push(std::vector<data_type>& src);
	void push(const void* src);
	void pop(std::vector<data_type>& dst);
	void pop(void* dst);
	void clear();
};

class array_buffer {
private:
	const int chunk_size;

	std::vector<std::vector<data_type>> queue;
	std::mutex m;

public:
	array_buffer(int chunk_size) : chunk_size(chunk_size) {}

	bool empty();
	void push(std::vector<data_type>&& src);
	void pop(void* dst);
	void clear();
};
