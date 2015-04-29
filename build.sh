#!/bin/bash

rm libmem* libdsk.so*

g++ -c -fPIC lib.cpp -o lib.o
g++ -shared -Wl,-soname,libmem.so.1 -o libmem.so.1.0.0 lib.o

g++ -c -fPIC lib2.cpp -o lib2.o
g++ -shared -Wl,-soname,libdsk.so.1 -o libdsk.so.1.0.0 lib2.o

g++ -g main.cpp tinyxml2.cpp -o main -ldl -lpthread
