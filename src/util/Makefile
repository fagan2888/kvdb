OBJS = log.o
EXES = 

all: ${OBJS}
	ar -cru ./libutil.a ${OBJS}

log.o: log.h log.cpp
	${CXX} ${CFLAGS} -c log.cpp

clean:
	rm -f ${EXES} ${OBJS} *.o *.exe *.a

