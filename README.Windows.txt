[SETUP]
  MAST uses C++ 2014, so gcc should be version 4.9 or higher.
  Install mingw-w64:              http://mingw-w64.org/doku.php
  Gnu Make with version >= 3.82
  CMake version >= 3.5
  MAST "SIT" & ICL parsers uses Flex and Bison (validated with Flex 2.5.37 and Bison 3.0)
    - It seems that Linux version (Flex 2.6.0 and Bison 3.0.4) works better (specially for error reporting)
    - NOTICE: It is recommended to patch Windows installation data sub-dir with equivalent files from Linux installation !!!
    -
  FLEX_DIR environment variable must point to where flex is intalled (to get access to FlexLexer.h)
  The unitary test interface Needs "python3" and "python3-ply" module
  To generate Doxygen documentation, doxygen and graphviz must be installed and dot.exe added to PATH environment variable

  Qt build has been tested with Qt 5.5 and QtCreator 3.6.1 but should be compatible with more recent versions
  Note that QtCreator has the ability to use CMake files (but seems to have a lesser level of support than for native .pro files)


[Build-CMake]

  Building:
    Debug:           "make debug"
    Release:         "make release"
    For ARM:         "make arm"
    Clean (shallow): "make clean"     (cleans all debug and release obj, lib and exe)
    Clean (deep):    "make distclean" (cleans all build directories)
    Doxygen docs:    "make docs"

  Running Mast Core Unit tests:
    Debug:      "make run_debug"
    Release:    "make run_release"


  Running Mast API Unit tests:
    Debug:      "make run_all_debug"
    Release:    "make run_all_release"


  Running SIT & ICL Parsers Unit tests:
    Debug:      "make run_readers_ut_debug"
    Release:    "make run_readers_ut_release"

  Running all tests (unit tests, smoke tests and application tests)
    Debug:      "make test_debug"
    Release:    "make test_release"


  Compiled object files are in generated to:        .\cmake_debug     and .\cmake_release
  Target dll and executable files are generated to: .\cmake_debug\Bin and .\cmake_release\Bin
  Doxygen docs are generated into:
    - Doxygen_Doc\MastDev,
    - Doxygen_Doc\Full_API,
    - Doxygen_Doc\CPP_API
    - Doxygen_Doc\C_API
  Their entry point is index.html in the respective directories

