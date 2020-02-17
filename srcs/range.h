#ifndef KVDB_RANGE_H_
#define KVDB_RANGE_H_

#include <string>
#include <list>
#include <map>
#include "item.h"

namespace kvdb{

// sst
class Range{
public:
	const std::string& start();
	const std::string& end();
	int size() const;

	// sorted by Item.key, but not for random acess
	std::list<Item> items;

	Range(const std::map<std::string, Item> &mm);
	Range(const std::list<Item> &sorted);

	void get(const std::string &key, std::string *val, uint64_t *index);

private:
	Range();
    Range(const Range&);
    void operator=(const Range&);
};

}; // end namespace kvdb

#endif
