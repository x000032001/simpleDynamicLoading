CC = gcc 
CXX = g++
CFLAGS = -Wall -Wextra -O2 
CXXFLAGS = -Wall -Wextra -O2 -std=c++11 -g

INCFLAGS = -Iutils
LDFLAGS = -ldl -lpthread
RM = rm -f

SRCS=$(shell ls *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
 
.PHONY: all
all: main
 
main: $(OBJS) 
	make -C lib && \
	make -C utils && \
	$(CXX) $(CXXFLAGS) -o $@ $^ utils/*.o $(LDFLAGS) 

%.o: %.cpp
	-$(CXX) ${CXXFLAGS} $(INCFLAGS) -c -o $@ $^
 
.PHONY: clean
clean:
	-${RM} *.o
