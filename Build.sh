#!/bin/bash

target=""
target=$1

if [ $target != "" ]
then
  echo
  echo  "Building for target: $target"
  echo  "============================"
  echo
else
  target=debug
  echo
  echo  "Building for default target: $target"
  echo  "===================================="
  echo
fi

#Clean executable before build
rm ./bin/$target/Lib_UT

make_exe="usr/bin/make"
makefile_lib=Lib_Linux.mak
makefile_lib_ut=Lib_UT_Linux.mak

echo ===================== Building Lib =====================
make -C Lib -f $makefile_lib $target

echo
echo ===================== Building Lib_UT Runner.cpp =====================
make -C Lib_UT -f Runner.mak

echo
echo ===================== Building Lib_UT =====================
make -C Lib_UT -f $makefile_lib_ut $target

echo
echo Executing \"$target\" unit tests...
echo
cd Lib_UT
../bin/$target/Lib_UT
cd ../..
