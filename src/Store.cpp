#include "Store.h"
#include "util/log.h"

namespace kvdb{

void Store::set(const std::string &key, const std::string &val, uint64_t seq){
	Item item;
	item.seq = seq;
	item.key = key;
	item.val = val;

	items.push_back(item);
	cache[item.key] = item;

	if(items.size() >= MAX_WAL_ITEMS || cache.size() >= MAX_RANGE_ITEMS){
		Range *r = new Range(cache);
		ranges.push_back(r);
		log_debug("new range, s:%s, e:%s, c:%d", r->start.c_str(), r->end.c_str(), r->size());

		items.clear();
		cache.clear();

		compact();
	}
}

void Store::compact(){
	std::vector<Range *> new_ranges;
	std::map<std::string, Item> mm;
	for(auto r : ranges){
		for(auto it = r->items.begin(); it != r->items.end(); it++){
			Item &item = *it;
			
			auto old = mm.find(item.key);
			if(old != mm.end() && old->second.seq > item.seq){
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
	std::sort(ranges.begin(), ranges.end(), [](Range *a, Range *b){
		return a->start < b->start;
	});
	
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
	
	return new Range(sorted);
}

void Store::get(const std::string &key, std::string *val, uint64_t *seq){
	*seq = 0;

	auto it = cache.find(key);
	if(it != cache.end()){
		Item &item = it->second;
		*val = item.val;
		*seq = item.seq;
		return;
	}

	for(auto r : ranges){
		std::string v;
		uint64_t i;
		r->get(key, &v, &i);
		if(i > *seq){
			*val = v;
			*seq = i;
		}
	}
}

}; // end namespace kvdb
