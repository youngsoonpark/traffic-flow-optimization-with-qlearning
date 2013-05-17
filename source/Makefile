
CPP=g++
INCLUDES=-I../include -I/usr/local/include  -I/usr/include/irrlicht

CFLAGS = ${INCLUDES} -g -Wall -O2 -pipe
LDFLAGS = -L/usr/local/lib
LDLIBS = -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -ljpeg -lpng 

SRC := $(shell echo *.cpp)
OBJS := $(addprefix obj/, $(SRC:.cpp=.o))

all: ${OBJS}
	$(CPP) -o ../Roadster ${OBJS} ${LDFLAGS} ${LDLIBS}

obj/%.o: %.cpp
	${CPP} -c -o $@ ${CFLAGS} $<

clean:
	rm obj/*.o
