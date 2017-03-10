From projects root directory (where this file is):

[SETUP]
 MAST uses C++ 2014, so gcc should be version 4.9 or higher.
 MAST uses Gnu Make with version >= 3.82
 The unitary test interface Needs "phyton3" and "phyton3-ply" module
 NB: on newer Linux installation, a manual install of ply 3.8 might be needed
				http://www.dabeaz.com/ply/
 On Windows, FLEX_DIR environment variable must point to where flex is intalled (to get access to FlexLexer.h)
 XmlRpc related features are only available on Linux (see README.Linux.txt)

[BUILD]
Create a build directory, for instance cmake_build

cd cmake_build

Generate Makefiles: cmake ..
It is also possible to define the build type:
 cmake --DCMAKE_BUILD_TYPE=Debug ..
 cmake --DCMAKE_BUILD_TYPE=Release ..

Build: make

Clean make clean

Alternatively, one can use makefile in project root directory:
  make debug|release
  make clean|distclean

To override default values of variables that drive build process, see UserOptions.model.cmake

[TESTS]
Standard CMAKE test: make run_debug or make run_release
Custom MAST test: ./Mast_UT

