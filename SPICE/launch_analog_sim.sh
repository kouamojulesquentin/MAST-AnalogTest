#!/bin/bash

CMAKE_BUILD_DIR=../../cmake_debug

[ ! -d "$CMAKE_BUILD_DIR" ] && (echo "Build dir "$CMAKE_BUILD_DIR" does not exist" ;  exit 1 )

echo "Creating symlinks to MAST executables"

ln -s -f "$CMAKE_BUILD_DIR/Bin/" Bin
ln -s -f "$CMAKE_BUILD_DIR/Lib/" Lib
ln -s -f "$CMAKE_BUILD_DIR/Bin/Mast" Mast
ln -s -f "$CMAKE_BUILD_DIR/Install/Examples/" Examples
