#!/usr/bin/env bash

if [ -z "$1" ]
then
  ./configure  --prefix=$(pwd)/build
else
./configure  --prefix=$1
fi

make
make install
make clean
