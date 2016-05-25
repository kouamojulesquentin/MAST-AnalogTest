
From projects root directory (where this file is):

To Build in release mode, either run  "Build release" or "Build_Release"
To Build in debug mode, either run:   "Build debug"   or "Build_Debug"

Script will end up running make with (successively):
  - Lib/Lib_Windows.mak
  - Lib_UT/Runner.mak
  - Lib_UT/Lib_UT_Windows.mak

Compiled object files are in generated to ./obj/debug and ./obj/release
Target dll and executable files are generated to ./bin/debug and ./bin/release

ATTENTION, note that, for now, there is no proper management of headers dependencies.
So, when modifying an header, it is recommended to clean the project(s) - unless the source files that use them are also modified -

To clean the projects, run "Build clean"

Unit tests can be run:
Debug   version: "UT_Run debug"   or "UT_Run_Debug"
Release version: "UT_Run release" or "UT_Run_Release" or "UT_Run_Release_Verbose"

All tests results are then saved into Lib_UT\UT_Results\UT_Result.txt
The verbose version will report all individual assertions actual values (even when successful)

