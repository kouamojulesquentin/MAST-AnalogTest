#!/usr/bin/env bash
# Helper script to build xmlrpc-c lib for MAST Usage
# It can be used only as a model (to check that each steps run smoothly)

if [ -z "$1" ]
then
  ./configure  --prefix=$(pwd)/build
else
./configure  --prefix=$1
fi

make
make install
make clean
