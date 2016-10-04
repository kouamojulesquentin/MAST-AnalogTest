@mainpage MAST overview

@section  Introduction
MAST project is composed of sub-projects:

  - @ref MAST_Core is the main MAST library that provides all capabilities of MAST
  - @ref SIT_reader is a library for parsing SIT files, building a MAST view of system under test
  - @ref MAST provides 'C' and 'C++' __simplified API__ to MAST, especially a __PDL like interface__
  - Logger is an asychronous log facility library based upon [g3log](https://github.com/KjellKod/g3log)
  - Mast_UT is for unit testing Mast_Core and Mast libraries
  - SIT_reader_UT is for unit testing SIT_reader
  - openocd is a library to use JTAG/SPI USB adapters to access System Under Test through JTAG or SPI
  - TestCasesApp is an application for testing integration of the different libraries with different kind of testcases
  - MastExample_CPP give some MAST simple usages examples
  - License manages the licensing rights to use MAST

For [acronyms, see there] (@ref Acronyms).

@section MAST_Core
It provides complete capabilities of MAST through object oriented design (using C++14).
All provided classes, structs, enums, functions are within mast namespace.

The two main parts of MAST_Core:
  - mast::SystemModel describes the SUT JTAG register static configuration and dynamic state
  - mast::SystemModelManager, upon requested actions of algorithms manages new state of JTAG chain,
    serializing data from and to SUT

Another important class is mast::BinaryVector that manages an ordered set of bits with conversion from/to string and integral types (up to 64 bits)

mast::SystemModel represents SUT AppendSibling()
Algorithms can be written in C++ using mast::SystemModelManager instance or use simpler 'C' or 'C++' PDL like APIs (cf. @ref MAST).

@section SIT_reader
SIT_reader library is used internally by @ref MAST to create a mast::SystemModel from SIT files but, can be used along with
@ref MAST_Core for specific needs.



@section Build
For building Mast projects, see README.txt, README.linux.txt, README.Windows.txt and README.cmake.txt
