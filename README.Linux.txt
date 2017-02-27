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

 Some parts need to intall:
  - apt-get install libftdi-dev
  - apt-get install libpng++-dev
  - apt-get install zlibc
  -

OpenOCD need Lib to be installed (sudo apt-get install libusb-1.0-0-dev)
To build OpenOCD, the following tools are required:
	- Automake
	- Autoconf
	- m4
	- perl


Xml RPC may be needed for optional features.
  - It is included in the build environment as dual parts
    - xmlrpc-c library (in External_Libs directory)
      - Build of xmlrpc-c must be done (normally once) before building Mast parts
        - Make sure curl library is installed for development: apt-get install curl-dev (or, if it is a virtual package: apt-get install libcurl4-gnutls-dev)
        - cd External_Libs/xmlrpc-c
        - make sure you have execution right on files
          - configure
          - install-sh
        - ./configure  --prefix=$(pwd)/build
        - make
        - make intall
        - That should be all
    - MastXmlRpc Mast libraries related to XmlRpc (in Optional_Libs directory)

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
    make distclean
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

