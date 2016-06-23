From projects root directory (where this file is):

[SETUP]
 MAST uses C++ 2014, so gcc should be version 4.9 or higher.
 MAST uses Gnu Make with version >= 3.82
 The unitary test interface Needs "phyton3" and "phyton3-ply" module

[BUILD]
Create a build directory, for instance cmake_build

cd cmake_build

Generate Makefiles: cmake ..
It is also possible to define the build type:
 cmake --DCMAKE_BUILD_TYPE=Debug ..
 cmake --DCMAKE_BUILD_TYPE=Release ..

Build: make

Clean make clean

[TESTS]
Standard CMAKE test: run make test
Custom MAST test: ./Lib_UT_Runner

