#!/bin/bash

export LD_LIBRARY_PATH=/home/denis/Documents/protection_info/lab1:$LD_LIBRARY_PATH

# Run programm
./lab2

# Validate methods
names=(
el_gamal
vernama
rsa)

len=${#names[@]}
cnt=0

while [[ $cnt -lt $len ]]; do
    diff -q --report-identical-files example ${names[$cnt]}_decode
    cnt=$((cnt + 1))
done
