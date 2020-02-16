#include "Range.h"
#include "util/log.h"

namespace kvdb{

Range::Range(const std::map<std::string, Item> &mm){
	for(auto p : mm){
		items.push_back(p.second);
		// printf("    %s\n", p.second.key.c_str());
	}
	if(!items.empty()){
		start = items[0].key;
		end = items[items.size()-1].key;
	}
}

Range::Range(const std::vector<Item> &sorted){
	items = sorted;
	if(!items.empty()){
		start = items[0].key;
		end = items[items.size()-1].key;
	}
}

int Range::size() const{
	return (int)items.size();
}


inline static bool item_cmp_func(const Item &a, const Item &b){
	return a.key < b.key;
}

void Range::get(const std::string &key, std::string *val, uint64_t *seq){
	*seq = 0;
	if(start > key || end < key){
		return;
	}

	Item item;
	item.key = key;
	auto it = std::lower_bound(items.begin(), items.end(), item, item_cmp_func);
	if(it == items.end()){
		return;
	}

	Item &f = *it;
	if(f.key == key){
		*val = f.val;
		*seq = f.seq;
	}
}

}; // end namespace kvdb
