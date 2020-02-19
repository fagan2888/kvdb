#include "range.h"
#include "util/log.h"

namespace kvdb{

static std::string empty_str;

Range::Range(const std::map<std::string, Item> &mm){
	for(auto p : mm){
		items.push_back(p.second);
		// printf("    %s\n", p.second.key.c_str());
	}
}

Range::Range(const std::list<Item> &sorted){
	items = sorted;
}

int Range::size() const{
	return (int)items.size();
}

const std::string& Range::start(){
	if(!items.empty()){
		return items.front().key;
	}
	return empty_str;
}

const std::string& Range::end(){
	if(!items.empty()){
		return items.back().key;
	}
	return empty_str;
}

void Range::get(const std::string &key, std::string *val, int64_t *seq){
	*seq = 0;
	if(start() > key || end() < key){
		return;
	}

	for(auto &i : items){
		int c = i.key.compare(key);
		if(c < 0){
			continue;
		}else if(c > 0){
			break;
		}else{
			*val = i.val;
			*seq = i.seq;
			break;
		}
	}
}

}; // end namespace kvdb
