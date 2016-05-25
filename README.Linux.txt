
From projects root directory (where this file is):

[BUILD]
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
