
From projects root directory (where this file is):

[SETUP]
 MAST uses C++ 2014, so gcc should be version 4.9 or higher.
 MAST uses Gnu Make with version >= 3.82
 The unitary test interface Needs "phyton3" and "phyton3-ply" module
 NB: on newer Linux installation, a manual install of ply 3.8 might be needed
				http://www.dabeaz.com/ply/
 CMake based build require Bison and Flex to be installed

[CMAKE BUILD]
To Build in release mode, run "make release"
To Build in debug mode, run "make debug"
To Build for ARM mode, run "make arm"

To Run Unitary Tests in release mode, run "make run_release"
To Run Unitary Tests in debug mode, run "make run_debug"
To Run Unitary Tests for ARM mode, "Lib_UT_Runner" must be executed on a ARM target (ex: QEMU)

"make clean" 		cleans all modes
"make distclean" 	cleans all mode directories


[MANUAL BUILD (deprecated)]
To Build in release mode, either run  ./Build.sh release or ./Build_Release.sh
To Build in debug mode, either run:   ./Build.sh debug   or ./Build_Debug.sh

Script will end up running make with (successively):
  - Lib/Lib_Linux.mak
  - Lib_UT/Runner.mak
  - Lib_UT/Lib_UT_Linux.mak

Compiled object files are in generated to ./obj/debug and ./obj/release
Target dll and executable files are generated to ./bin/debug and ./bin/release

ATTENTION, note that, for now, there is no proper management of headers dependencies.
So, when modifying an header, it is recommended to clean the project(s) - unless the source files that use them are also modified -

To clean the projects, run ./Build.sh clean

[TESTS]
Unit tests are run automatically when build is successful.

They can also be run on demand, FROM Lib_UT directory
../Bin/release/Lib_UT
or
../Bin/debug/Lib_UT
