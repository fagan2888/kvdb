#include "util/log.h"
#include "Store.h"

using namespace kvdb;

int main(int argc, char **argv){
	uint64_t index = 0;
	Store store;

	srand(time(0));
	for(int i=0; i<10000; i++){
		int num = rand() % 1000;
		char buf[16];
		snprintf(buf, sizeof(buf), "%06d", num);
		std::string key = buf;
		std::string val = buf;
		index ++;

		store.set(key, val, index);

		std::string v;
		uint64_t idx;
		store.get(key, &v, &idx);
		if(v != val){
			log_debug("[%s] [%s] %d", val.c_str(), v.c_str(), idx);
		}
	}
	return 0;
}
