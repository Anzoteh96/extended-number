CXX=g++
CXXFLAGS=-std=c++14 -g -Wall -MMD
OBJECTS=big_int.o main.o
EXEC=extnum
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include:${DEPENDS}

.PHONY: clean

clean:
	rm *.o extnum
