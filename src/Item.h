#ifndef KVDB_ITEM_H_
#define KVDB_ITEM_H_

#include <string>

namespace kvdb{

struct Item{
	uint64_t index;
	std::string key;
	std::string val;
};

inline static bool operator < (const Item &a, const Item &b){
	if(a.key < b.key){
		return true;
	// }else if(a.key == b.key){
		// return a.index < b.index;
	}else{
		return false;
	}
}

}; // end namespace kvdb

#endif
