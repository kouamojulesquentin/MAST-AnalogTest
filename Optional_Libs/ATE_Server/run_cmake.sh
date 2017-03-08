#!/usr/bin/env bash

CMakeDir=cmake_debug
if [ ! -d "$CMakeDir" ]; then
  echo  "Creating directory: $CMakeDir"
  mkdir $CMakeDir
fi

cd $CMakeDir

cmake -DCMAKE_CXX_COMPILER=g++ ..
