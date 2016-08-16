[SETUP]
 MAST uses C++ 2014, so gcc should be version 4.9 or higher.
 MAST uses Gnu Make with version >= 3.82
 The unitary test interface Needs "phyton3" and "phyton3-ply" module
 NB: on newer Linux installation, a manual install of ply 3.8 might be needed
				http://www.dabeaz.com/ply/


[Build-Qt]

For debug and release configurations: Add custom build step using make with parameters: -C  %{CurrentProject:Path}/Lib_UT -f Runner.mak
Place it before project make or even as first step (before Qmake is run)

[Build-CMake]
  CMake based build require Bison and Flex to be installed

  Building:
    Debug:           "make debug"
    Release:         "make release"
    For ARM:         "make arm"
    Clean (shallow): "make clean"     (cleans all debug and release obj, lib and exe)
    Clean (deep):    "make distclean" (cleans all build directories)

  Running Lib Unit tests:
    Debug:      "make run_debug"
    Release:    "make run_release"
    for ARM: "Lib_UT_Runner" must be executed on a ARM target (ex: QEMU)

  Running SIT Parser Unit tests:
    Debug:      "make run_parser_debug"
    Release:    "make run_parser_release"

