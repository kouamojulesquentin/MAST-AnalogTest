[SETUP]
  MAST uses C++ 2014, so gcc should be version 4.9 or higher.
  Install mingw-w64:              http://mingw-w64.org/doku.php
  Gnu Make with version >= 3.82
  CMake version >= 3.5
  MAST "SIT" Parser uses Flex and Bison (validated with Flex 2.5.37 and Bison 3.0)
  FLEX_DIR environment variable must point to where flex is intalled (to get access to FlexLexer.h)
  The unitary test interface Needs "phyton3" and "phyton3-ply" module
  To generate Doxygen documentation, doxygen and graphviz must be installed and dot.exe added to PATH environment variable

  Qt build has been tested with Qt 5.5 and QtCreator 3.6.1 but should be compatible with more recent versions
  Note that QtCreator has the ability to use CMake files (but seems to have a lesser level of support than for native .pro files)

[Build-Qt]

For debug and release configurations: Add custom build step using make with parameters: -C  %{CurrentProject:Path}/Lib_UT -f Runner.mak
Place it before project make or even as first step (before Qmake is run)

To create makefiles from the command line (when a project file is modified), use MakeQtMakefiles.bat
Note that 'QMake' environment variable should point to qmake application (something like: "C:\Qt\5.6\mingw49_32\bin\qmake.exe")

  Compiled object files are in generated to Build_Qt
  Target dll and executable files are generated to Build_Qt\bin\debug and Build_Qt\bin\release

  Building (command line):
    Debug:   "Build debug"   or "Build_Debug"
    Release: "Build release" or "Build_Release"

  Running Unit tests:
    Debug:   "UT_Run debug"   or "UT_Run_Debug"
    Release: "UT_Run release" or "UT_Run_Release" or "UT_Run_Release_Verbose"

    All tests results are then saved into Lib_UT\UT_Results\UT_Result.txt
    The verbose will report all individual assertions actual values (even when successful)


[Build-CMake]

  Building:
    Debug:           "make debug"
    Release:         "make release"
    For ARM:         "make arm"
    Clean (shallow): "make clean"     (cleans all debug and release obj, lib and exe)
    Clean (deep):    "make distclean" (cleans all build directories)
    Doxygen docs:    "make docs"

  Running Lib Unit tests:
    Debug:      "make run_debug"
    Release:    "make run_release"

  Running SIT Parser Unit tests:
    Debug:      "make run_parser_debug"
    Release:    "make run_parser_release"

  Compiled object files are in generated to:        .\cmake_debug     and .\cmake_release
  Target dll and executable files are generated to: .\cmake_debug\Bin and .\cmake_release\Bin
  Doxygen docs are generated into:
    - Doxygen_Doc\MastDev,
    - Doxygen_Doc\Full_API,
    - Doxygen_Doc\CPP_API
    - Doxygen_Doc\C_API
  Their entry point is index.html in the respective directories

