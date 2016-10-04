[SETUP]
 MAST uses C++ 2014, so gcc should be version 4.9 or higher.
 MAST uses Gnu Make with version >= 3.82
 The unitary test interface Needs "python3" and "python3-ply" module
 NB: on newer Linux installation, a manual install of ply 3.8 might be needed
				http://www.dabeaz.com/ply/
 MAST uses Flex and Bison
 To generate Doxygen documentation, doxygen and graphviz must be installed
  - apt-get install doxygen
  - apt-get install graphviz


OpenOCD need Lib to be installed (sudo apt-get install libusb-1.0-0-dev)
To build OpenOCD, the following tools are required:
	- Automake
	- Autoconf
	- m4
	- perl


[Build-Qt]

For debug and release configurations: Add custom build step using make with parameters: -C  %{CurrentProject:Path}/Lib_UT -f Runner.mak
Place it before project make or even as first step (before Qmake is run)

[Build-CMake]
  CMake based build require Bison and Flex to be installed

  When building with OpenOCD, OpenOCD must be build before MAST ==> see openocd/openocd-builder/README.txt

  Building:
    Debug:           "make debug"
    Release:         "make release"
    For ARM:         "make arm"
    Clean (shallow): "make clean"     (cleans all debug and release obj, lib and exe)
    Clean (deep):    "make distclean" (cleans all build directories)
    Doxygen docs:    "make docs"

  To disable build using OpenOCD
    make disclean
    then:
    Debug:           "make debug   USE_OPEN_OCD=OFF"
    Release:         "make release USE_OPEN_OCD=OFF"

  Running Lib Unit tests:
    Debug:      "make run_debug"
    Release:    "make run_release"
    for ARM: "Lib_UT_Runner" must be executed on a ARM target (ex: QEMU)

  Running SIT Parser Unit tests:
    Debug:      "make run_sit_ut_debug"
    Release:    "make run_sit_ut_release"

  Compiled object files are in generated to: .\cmake_debug     and .\cmake_release
  Target dll files are generated to:         .\cmake_debug\Lib and .\cmake_release\Lib
  Target executable files are generated to:  .\cmake_debug\Bin and .\cmake_release\Bin
  Doxygen docs are generated into:
    - Doxygen_Doc\MastDev,
    - Doxygen_Doc\Full_API,
    - Doxygen_Doc\CPP_API
    - Doxygen_Doc\C_API
  Their entry point is index.html in the respective directories

