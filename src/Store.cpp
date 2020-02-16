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

	if(items.size() >= MAX_WAL_ITEMS){
		Range *r = new Range(cache);
		ranges.push_back(r);
		log_debug("new range, s:%s, e:%s, c:%d", r->start.c_str(), r->end.c_str(), r->size());

		items.clear();
		cache.clear();

		compact();
	}
}

static bool range_cmp_func(Range *a, Range *b){
	return a->start < b->start;
}

void Store::compact(){
	if(ranges.size() < 3){
		return;
	}
	
	std::vector<Range *> new_ranges;
	std::map<std::string, Item> mm;
	for(auto r : ranges){
		for(auto it = r->items.begin(); it != r->items.end(); it++){
			Item &item = *it;
			
			auto old = mm.find(item.key);
			if(old != mm.end() && old->second.index > item.index){
				continue;
			}
			
			mm[item.key] = item;
			if(mm.size() == COMPACT_BUFFER_SIZE){
				Range *nr = compact_write_range(&mm);
				new_ranges.push_back(nr);
			}
		}
	}
	while(!mm.empty()){
		Range *nr = compact_write_range(&mm);
		new_ranges.push_back(nr);
	}
	
	log_debug("old ranges:");
	for(auto r : ranges){
		log_debug("    s:%s, e:%s, c:%d", r->start.c_str(), r->end.c_str(), r->size());
	}

	ranges.swap(new_ranges);
	std::sort(ranges.begin(), ranges.end(), range_cmp_func);
	
	log_debug("new ranges:");
	for(auto r : ranges){
		log_debug("    s:%s, e:%s, c:%d", r->start.c_str(), r->end.c_str(), r->size());
	}
}

Range* Store::compact_write_range(std::map<std::string, Item> *mm){
	std::vector<Item> sorted;
	for(auto it = mm->begin(); it != mm->end(); it ++){
		sorted.push_back(it->second);
		if(sorted.size() == MAX_RANGE_ITEMS){
			break;
		}
	}
	for(auto &item : sorted){
		mm->erase(item.key);
	}
	
	Range *range = NULL;
	if(sorted.size() > 0){
		range = new Range(sorted);
	}
	return range;
}

void Store::get(const std::string &key, std::string *val, uint64_t *index){
	*index = 0;

	auto it = cache.find(key);
	if(it != cache.end()){
		Item &item = it->second;
		*val = item.val;
		*index = item.index;
		return;
	}

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

}; // end namespace kvdb
