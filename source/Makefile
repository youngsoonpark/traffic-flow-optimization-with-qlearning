# Author Benjamin James Wright <bwright@cse.unsw.edu.au>

CPP=g++
INCLUDES= -I../include -I../lib/irrlicht/include

CFLAGS = ${INCLUDES} -g -Wall -O2 -pipe -std=c++0x 

LDFLAGS = -L../lib/irrlicht/
LDLIBS = -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -ljpeg -lpng

SRC := $(shell echo **/*.cpp main.cpp)
OBJS := $(addprefix obj/, $(SRC:.cpp=.o))

all: ${OBJS}
	$(CPP) -o ../Roadster ${OBJS} ${LDFLAGS} ${LDLIBS}

obj/%.o: %.cpp
	${CPP} -c -o $@ ${CFLAGS} $<
obj/core/%.o: core/%.cpp
	${CPP} -c -o $@ ${CFLAGS} $<
obj/sim/%.o: sim/%.cpp
	${CPP} -c -o $@ ${CFLAGS} $<
obj/graphics/%.o: graphics/%.cpp
	${CPP} -c -o $@ ${CFLAGS} $<
obj/io/%.o: io/%.cpp
	${CPP} -c -o $@ ${CFLAGS} $<
obj/ml/%.o: ml/%.cpp
	${CPP} -c -o $@ ${CFLAGS} $<

clean:
	rm obj/*.o
	rm obj/core/*.o
	rm obj/sim/*.o
	rm obj/graphics/*.o
	rm obj/io/*.o
	rm obj/ml/*.o
