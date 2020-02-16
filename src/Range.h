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
	// sorted by Item.key
	std::vector<Item> items;

	Range(const std::map<std::string, Item> &mm);
	Range(const std::vector<Item> &sorted);

	int size() const;
	void get(const std::string &key, std::string *val, uint64_t *index);

private:
	Range();
};

}; // end namespace kvdb

#endif
