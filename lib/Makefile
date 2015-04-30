CC = gcc 
CXX = g++
CFLAGS = -c -fPIC -Wall -Wextra -O2 -I..
CXXFLAGS = -c -fPIC -Wall -Wextra -O2 -I..
LDFLAGS = -shared
RM = rm -f

SRCS=$(shell ls *.cpp)
TARGET_LIB=$(patsubst %.cpp,%.so,$(SRCS))
 
.PHONY: all
all: ${TARGET_LIB}
 
%.so: %.o
	-$(CXX) ${LDFLAGS} -o $@ $^
 
%.o: %.cpp
	-$(CXX) ${CXXFLAGS} -o $@ $^
 
.PHONY: clean
clean:
	-${RM} *.so *.o
