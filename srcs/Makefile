OBJS = Item.o Range.o Store.o
CFLAGS = -std=c++11
CFLAGS += -DNDEBUG -D__STDC_FORMAT_MACROS -Wall

all: ${OBJS}

Item.o: Item.h Item.cpp
	${CXX} ${CFLAGS} -c Item.cpp
Range.o: Range.h Range.cpp
	${CXX} ${CFLAGS} -c Range.cpp
Store.o: Store.h Store.cpp
	${CXX} ${CFLAGS} -c Store.cpp

test: ${OBJS}
	${CXX} -o test.out ${OBJS} util/libutil.a test.cpp

clean:
	rm -f *.a *.o
