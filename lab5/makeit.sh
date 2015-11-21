#!/bin/bash

g++ -O3 -Wall -std=c++14 -o money money.cpp \
    -L. ../lab1/cryptolib.so