#!/bin/bash

SRC_NAME=lib
LIB_NAME=libmem.so

rm ${LIB_NAME}*

g++ -c -fPIC ${SRC_NAME}.cpp -o ${SRC_NAME}.o
g++ -shared -Wl,-soname,${LIB_NAME}.1 -o ${LIB_NAME}.1.0.0 lib.o

ln -s ${LIB_NAME}.1.0.0 ${LIB_NAME}
ln -s ${LIB_NAME}.1.0.0 ${LIB_NAME}.1

g++ main.cpp tinyxml2.cpp -o main -ldl -lpthread
