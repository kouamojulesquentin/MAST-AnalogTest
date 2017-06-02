MAST Overview
=============

[Acronyms]:   Acronyms.html
[API]:        Acronyms.html "Application Programming Interface"
[ATE]:        Acronyms.html "Automatic Test Equipment"
[DLL]:        Acronyms.html "Dynamically Linked Library"
[JTAG]:       Acronyms.html "Joint Test Action Group"
[POC]:        Acronyms.html "Proof Of Concept"
[SPI]:        Acronyms.html "Serial Peripheral Interface"
[SUT]:        Acronyms.html "System Under Test"

[Plugins]:    Plugins.html
[SIT Reader]: SIT_Reader.html
[SIT]:        SIT_Files.html
[MAST API]:   MAST_API.html
[Options]:    MastOptions.html

[TOC]

@section Introduction
MAST project is composed of sub-projects:

  - @ref MAST_Core is the main MAST library that provides all capabilities of MAST
  - @ref [SIT_reader][SIT Reader] is a library for parsing [SIT] files, building a MAST logical representation of [SUT]
  - @ref MAST_API provides 'C' and 'C++' __simplified API__ to MAST, especially a __PDL like interface__
  - Logger is an asychronous log facility library based upon [g3log](https://github.com/KjellKod/g3log)
  - Mast_UT is for unit testing Mast_Core and Mast libraries
  - SIT_reader_UT is for unit testing [SIT_reader][SIT Reader]
  - Optional_Libs gather libraries, developed by MAST project, can be used for some usages of MAST but are
    not part of the core nor the simplified API:
    - Intel is used for a [POC]
    - ATE_Server is used to run MAST in an [ATE] context
    - MastRpc provides example(s) of remote AccessInterface protocol
    - MastXmlRpc is used as wrapper around Xml-RPC for communication between MAST and a remote AccessInterface
    - [SPI] provides an [SPI] compatible AccessInterface protocol
    - OpenOCD provides an Open OCD based AccessInterface protocol
    - Plugins holds the [plugin][Plugins] [API]
  - External_Libs gather libraries not developed by MAST project but useful for some usages. It includes:
    - openocd is a library to use [JTAG]/[SPI] USB adapters to access [SUT] through [JTAG] or [SPI]
    - FTDI_SPI for interfacing MAST to [SUT] using a [SPI] port
    - kissfft is used by some examples algorithm to compute an FFT on some audio sample
    - xmlrpc-c is used to provide RPC (e.g. to run MAST on a remote machine)
    - Yaml is [yaml-cpp](https://github.com/jbeder/yaml-cpp) library for parsing yaml files
    - [tclap](http://tclap.sourceforge.net/) is a Templatized C++ Command Line Parser Library
  - TestCasesApp is an application for testing integration of the different libraries with different kind of testcases
  - MastExample_CPP give some MAST simple usages examples
  - License manages the licensing rights to use MAST

@section MAST_Core
It provides complete capabilities of MAST through object oriented design (using C++14).
All provided classes, structs, enums, functions are within mast namespace.

The two main parts of MAST_Core:
  - mast::SystemModel describes the [SUT] [JTAG] register static configuration and dynamic state
  - mast::SystemModelManager, upon requested actions of algorithms manages new state of [JTAG] chain,
    serializing data from and to [SUT]

Another important class is mast::BinaryVector that manages an ordered set of bits with conversion
from/to string and integral types (up to 64 bits).

mast::SystemModel represents [SUT]

Algorithms can be written in C++ using mast::SystemModelManager instance or use simpler 'C++' or 'C' PDL like [API]s (cf. @ref MAST_API).

@section SIT_reader SIT Files and reader
[SIT_Reader][SIT Reader] library is used internally by [MAST API] to create a mast::SystemModel from [SIT] file, can be used along with
@ref MAST_Core for specific needs.

@section Plugins
Mast behavior can be adapted to user needs using [plugins][Plugins] extension points.

@section Mast Options
Mast [Options] can be configured through configuration file and/or command line switches.


@section Build
For building Mast projects, see README.txt, README.linux.txt, README.Windows.txt and README.cmake.txt
