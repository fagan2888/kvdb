OBJS = item.o range.o store.o
CFLAGS = -std=c++11
CFLAGS += -DNDEBUG -D__STDC_FORMAT_MACROS -Wall

all: ${OBJS}

item.o: item.h item.cpp
	${CXX} ${CFLAGS} -c item.cpp
range.o: range.h range.cpp
	${CXX} ${CFLAGS} -c range.cpp
store.o: store.h store.cpp
	${CXX} ${CFLAGS} -c store.cpp

test: ${OBJS}
	${CXX} -o test.out ${OBJS} util/libutil.a test.cpp

clean:
	rm -f *.a *.o
