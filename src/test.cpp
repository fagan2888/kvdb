#include "util/log.h"
#include "Store.h"
#include <map>
#include <string>

using namespace kvdb;

int main(int argc, char **argv){
	uint64_t index = 0;
	Store store;

	std::map<std::string, std::string> mm;

	srand(time(0));
	for(int j=0; j<10; j++){
		for(int i=0; i<1000; i++){
			int num = rand() % 1000;
			char buf[16];
			snprintf(buf, sizeof(buf), "%06d", num);
			std::string key = buf;
			std::string val = buf;
			index ++;

			mm[key] = val;
			store.set(key, val, index);
		}
	}

	log_debug("set %d items", mm.size());
	for(int i=0; i<10000; i++){
		int num = rand() % 1000;
		char buf[16];
		snprintf(buf, sizeof(buf), "%06d", num);
		std::string k = buf;
		
		std::string v;
		uint64_t idx;
		store.get(k, &v, &idx);
		if(mm.count(k) > 0){
			if(v != mm[k]){
				log_debug("[%s] [%s] %d", v.c_str(), mm[k].c_str(), idx);
			}
		}
	}
	log_debug("");

	return 0;
}
