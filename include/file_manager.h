#pragma once
#include <vector>
#include "packet.h"

class file_manager
{
private:
	std::vector<std::pair<std::time_t, packet*>> queue;

public:
	file_manager() {}
	~file_manager() {}

	void push(packet* p);
};

