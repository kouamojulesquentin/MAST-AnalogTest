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

make -C Lib    -f $makefile_lib    $target


make -C Lib_UT -f $makefile_lib_ut $target



echo  
echo Executing \"$target\" unit tests... 
echo 
cd Lib_UT
../bin/$target/Lib_UT
cd ../..


# pushd Lib
# g++.exe -shared -Wl,--output-def=../bin/Debug/libLib.def -Wl,--out-implib=../bin/Debug/libLib.a -Wl,--dll  ../obj/Debug/src/AccessInterface.o ../obj/Debug/src/BinaryVector.o ../obj/Debug/src/Chain.o ../obj/Debug/src/DefaultBinaryPathSelector.o ../obj/Debug/src/GmlPrinterVisitor.o ../obj/Debug/src/Linker.o ../obj/Debug/src/ParentNode.o ../obj/Debug/src/PrettyPrinterVisitor.o ../obj/Debug/src/Register.o ../obj/Debug/src/SVFVector.o ../obj/Debug/src/SystemModel.o ../obj/Debug/src/SystemModelNode.o ../obj/Debug/src/Tap.o ../obj/Debug/src/Utility.o  -o ../bin/Debug/Lib.dll  -luser32
# g++.exe -shared -Wl, --output-def=../bin/Debug/libLib.def -Wl, --out-implib=../bin/Debug/libLib.a -Wl, --dll  ../obj/Debug/src/AccessInterface.o ../obj/Debug/src/BinaryVector.o ../obj/Debug/src/Chain.o ../obj/Debug/src/DefaultBinaryPathSelector.o ../obj/Debug/src/GmlPrinterVisitor.o ../obj/Debug/src/Linker.o ../obj/Debug/src/ParentNode.o ../obj/Debug/src/PrettyPrinterVisitor.o ../obj/Debug/src/Register.o ../obj/Debug/src/SVFVector.o ../obj/Debug/src/SystemModel.o ../obj/Debug/src/SystemModelNode.o ../obj/Debug/src/Tap.o ../obj/Debug/src/Utility.o  -o ../bin/Debug/Lib.dll  -luser32
# popd

