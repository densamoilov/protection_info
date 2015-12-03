#!/bin/bash

g++ -O0 -g -Wall -std=c++14 -o gamilton_cycle gamilton_cycle.cpp \
    -L. ../lab1/cryptolib.so

g++ -O0 -g -Wall -std=c++14 -o gen_data gen_data.cpp