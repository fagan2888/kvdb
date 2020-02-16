#ifndef KVDB_RANGE_H_
#define KVDB_RANGE_H_

#include <string>
#include <vector>
#include <map>
#include "Item.h"

namespace kvdb{

// sst
class Range{
public:
	std::string start;
	std::string end;

	Range(const std::map<std::string, Item> &mm);

	int size() const;
	void get(const std::string &key, std::string *val, uint64_t *index);

private:
	Range();

	// sorted by Item.key
	std::vector<Item> items;
};

}; // end namespace kvdb

#endif
