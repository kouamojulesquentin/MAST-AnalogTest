@mainpage MAST overview

@section  Introduction
MAST project is composed of sub-projects:

  - @ref MAST_Core is the main MAST library that provides all capabilities of MAST
  - @ref SIT_reader is a library for parsing SIT files, building a MAST view of system under test
  - @ref MAST provides 'C' and 'C++' __simplified API__ to MAST, especially a __PDL like interface__
  - Logger is an asychronous log facility library based upon [g3log](https://github.com/KjellKod/g3log)

@section MAST_Core
It provides complete capabilities of MAST through object oriented design (using C++14).
All provided classes, structs, enums, functions are within mast namespace.

The two main parts of MAST_Core:
  - mast::SystemModel describes the SUT JTAG register static configuration and dynamic state
  - mast::SystemModelManager, upon requested actions of algorithms manages new state of JTAG chain,
    serializing data from and to SUT

Algorithms can be written in C++ using mast::SystemModelManager instance or use simpler 'C' or 'C++' PDL like APIs (cf. @ref MAST).

@section MAST
MAST project provides simple 'C' or 'C++' API to use MAST.

@note Simple API implies it does not have the full-fledged capabilities of @ref MAST_Core.

@section SIT_reader
SIT_reader library is used internaly by @ref MAST to create a mast::SystemModel from SIT files but can be used along with
@ref MAST_Core for specific needs.
