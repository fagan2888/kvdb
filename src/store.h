#ifndef KVDB_STORE_H_
#define KVDB_STORE_H_

#include <vector>
#include <list>
#include <map>
#include <string>
#include "item.h"
#include "range.h"

namespace kvdb{

class Store{
public:
	Store(){}
	void set(const std::string &key, const std::string &val, int64_t index);
	void get(const std::string &key, std::string *val, int64_t *index);

private:
    Store(const Store&);
    void operator=(const Store&);

	void compact();
	Range* compact_write_range(std::map<std::string, Item> *mm);
	
	const static int MAX_WAL_ITEMS = 200;
	const static int MAX_RANGE_ITEMS = 100;
	const static int COMPACT_BUFFER_SIZE = MAX_RANGE_ITEMS * 3;

	// mm: mutable memtable, imm: immutable memtable
	std::map<std::string, Item> cache;
	// append only
	std::list<Item> items;

	// TODO: interval tree, range tree
	std::vector<Range *> ranges;
};

}; // end namespace kvdb

#endif
