#include "file_manager.h"



void file_manager::push(packet* p) {
	queue.push_back(p);
}