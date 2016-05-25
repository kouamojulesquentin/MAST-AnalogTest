
New mast project is organized into 3-sub-projects.

 1 - Lib is mast building blocks per se
 2 - Lib_UT is the project for unit testing Lib
 3 - Lib_UT_Runner is a "pseudo" project for Codeblocks to create unit test main: Runner.cpp before Lib_UT build

Lib/Lib.cbp, Lib_UT/Lib_UT.cbp and Lib_UT_Runner/Lib_UT_Runner.cbp are project files for CodeBlocks IDE

All, non-generated, files are under source control using GIT

To compile and run unit tests on Linux,   see README.Linux.txt
To compile and run unit tests on Windows, see README.Windows.txt

