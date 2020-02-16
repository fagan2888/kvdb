#include "Store.h"
#include "util/log.h"

namespace kvdb{

void Store::set(const std::string &key, const std::string &val, uint64_t index){
	Item item;
	item.index = index;
	item.key = key;
	item.val = val;

	items.push_back(item);
	cache[item.key] = item;

	if(items.size() == 100){
		Range *r = new Range(cache);
		ranges.push_back(r);
		log_debug("new range, s:%s, e:%s, c:%d", r->start.c_str(), r->end.c_str(), r->size());

		items.clear();
		cache.clear();
	}
}

void Store::get(const std::string &key, std::string *val, uint64_t *index){
	*index = 0;

	auto it = cache.find(key);
	if(it != cache.end()){
		Item &item = it->second;
		*val = item.val;
		*index = item.index;
	}else{
		for(auto r : ranges){
			std::string v;
			uint64_t i;
			r->get(key, &v, &i);
			if(i > *index){
				*val = v;
				*index = i;
			}
		}
	}
}

}; // end namespace kvdb
