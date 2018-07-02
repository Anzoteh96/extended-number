CXX=g++
CXXFLAGS=-std=c++14 -g -O2 -Wall -MMD
OBJECTS=big_int.o num_ring.o main.o
EXEC=extnum
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include:${DEPENDS}

.PHONY: clean

clean:
	rm *.o *.d extnum
