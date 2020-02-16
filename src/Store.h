#ifndef KVDB_STORE_H_
#define KVDB_STORE_H_

#include <vector>
#include <map>
#include <string>
#include "Item.h"
#include "Range.h"

namespace kvdb{

class Store{
public:
	void set(const std::string &key, const std::string &val, uint64_t index);
	void get(const std::string &key, std::string *val, uint64_t *index);

private:
	void compact();
	Range* compact_write_range(std::map<std::string, Item> *mm);
	
	const static int MAX_WAL_ITEMS = 100;
	const static int MAX_RANGE_ITEMS = 100;
	const static int COMPACT_BUFFER_SIZE = MAX_RANGE_ITEMS * 3;

	std::map<std::string, Item> cache;
	// append only
	std::vector<Item> items;

	// TODO: interval tree, range tree
	std::vector<Range *> ranges;
};

}; // end namespace kvdb

#endif
