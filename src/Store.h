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
	const static int MAX_WAL_ITEMS = 100;

	std::map<std::string, Item> cache;
	// append only
	std::vector<Item> items;

	// TODO: interval tree, range tree
	std::vector<Range *> ranges;
};

}; // end namespace kvdb

#endif
