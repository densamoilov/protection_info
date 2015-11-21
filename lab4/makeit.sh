#!/bin/bash

g++ -O3 -Wall -std=c++14 -o poker poker.cpp \
    -L. ../lab1/cryptolib.so