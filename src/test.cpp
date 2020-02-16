#include "util/log.h"
#include "Store.h"
#include <map>
#include <string>

using namespace kvdb;

static inline double microtime(){
	struct timeval now;
	gettimeofday(&now, NULL);
	double ret = now.tv_sec + now.tv_usec/1000.0/1000.0;
	return ret;
}

int main(int argc, char **argv){
	double stime;
	uint64_t index = 0;
	Store store;

	std::map<std::string, std::string> mm;

	srand(time(0));
	stime = microtime();
	for(int i=0; i<10000; i++){
		int num = rand() % 1000;
		char buf[16];
		snprintf(buf, sizeof(buf), "%06d", num);
		std::string key = buf;
		std::string val = buf;
		index ++;

		mm[key] = val;
		store.set(key, val, index);
	}
	{
		double etime = microtime();
		double ts = etime - stime + 0.0000001;
		log_debug("write qps: %d", (int)(10000/ts));
	}

	stime = microtime();
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
	{
		double etime = microtime();
		double ts = etime - stime + 0.0000001;
		log_debug("read qps: %d", (int)(10000/ts));
	}

	return 0;
}
