#!/bin/bash

g++ -Wall -O2 -std=c++14 -g \
    -o digital_signature digital_signature.cpp \
    -lssl -lcrypto \
    -L. ../lab1/cryptolib.so

#export LD_LIBRARY_PATH=/home/denis/Documents/protection_info/lab1:$LD_LIBRARY_PATH

#./digital_signature