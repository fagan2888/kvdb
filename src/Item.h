#ifndef KVDB_ITEM_H_
#define KVDB_ITEM_H_

#include <string>

namespace kvdb{

struct Item{
	uint64_t seq;
	std::string key;
	std::string val;
};

}; // end namespace kvdb

#endif
