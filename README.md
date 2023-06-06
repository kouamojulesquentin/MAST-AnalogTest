-------------------------------------------------------------------------------------------------
# MAST : Manager for SOC Test

MAST provides an implemenation of the IEEE 1687 and IEEE P1687.1 standard based on the handing of complex topologies and on the support for 
interactive, massively parallel routines. 
It does not have the vocation to be a complete reference implementation: as such, only a subset of features are fully implemented. However, 
the Kernel is optimized and highly flexibile, so for a user it will be easy to add teh desired features. 

MAST implements an ICL Parser, but it also support "Simplified ICL Tree" (SIT), a simplified version of ICL that is easier to write, parse and extend
 and it is ideal for experimenting with the Standards' most advanced features without the rigour and difficulty of SIT. As such, most examples in this 
 repository use SIT for P1687.1 et al. A specifi documentation is given for ICL.

The only deviation from the IEEE 1687-2014 document is the implementation of PDL-1: instead of using TCL, MAST expresses PDL as a C/C++ API, 
allowing PDL operations to be freely mixed with algorithms. This provides both an important performance boost and the flexibility of  using a real
programming language for PDL-1 routines. 

-------------------------------------------------------------------------------------------------
## Compilation Details

MAST uses Cmake >= 3.82 for both compilation and Unit Testing. This version has been verified for Ubuntu 18.04 with gcc/g++ 7.5.0 

Unit Testing use extensively C++14 experimental features : when compiling with higher gxx version some UT might not pass, but this does not impact
 MAST's fucntionality. 

### Configuration 
MAST is a modular software, based on a Core and several Optional and External libraries their compilation and inclusion is defined by the *UserOption.cmake* file, not included in the distribution to avoid excessive versioning: you can build your own based on the  *UserOption.model.cmake*

Platform-specific compilation instructions can be found in *README.Linux.txt* and *README.Windows.txt* respectively

### Compilation
To compile, simply type

```bash
make
```
This will configure, compile and install the "debug" build in the subdirectory ./cmake_debug

Other useful commands: 
### Unitary Tests

```bash
#Unitary Test can be launched with 
make test

#Details of tests of the Mast_Core can be launched separately: 
make run_debug

#To clean current build : 
make clean

#To completely delete current build 
make distclean

#To install files in the build tree (useful when adding new files)
make install
```

### External Libraries
Some external libraries like *OpenOCD* might require independent configuration and building. Please refer to their own README
## Usage

By defaut, all files are compiled in the build directory (ex: cmake_debug). The main executable is **./Bin/MAST**. For execution, it references the Dynamic Libraries (.so files) in the **./Lib/** directory. 

-------------------------------------------------------------------------------------------------
### Examples
 Examples of MAST execution are available in the ./Examples directory, which is installed to
  <cmake_build_dir>/Install/Examples
 Documentation about them is given both in Readme and in the MAST User Manual 

-------------------------------------------------------------------------------------------------
### Documentation

Doxygen documentation can be generated with 
```bash
make docs
```
Documentation is generated as HTML files in the *./Doxygen_Docs/* subdirectory. 
This will generate 4 versions: 
+ ./MastDev/html/index.html  : the Full Documentation
+ ./FULL_API/html/index.html : Documentation for the internal MAST Cpre
+ ./CPP_API/html/index.html  : Documentation for the C++ external interfaces (PDL and environment/startup)
+ ./C_API/html/index.html    : Documentation for the C PDL external interfaces 

Detailed User Manuals and other documentation can be found in the ./Docs directory


## Directory Organization
-----------------------------------------------------------------------
New mast project is organized into sub-projects.

  - Docs provides detailes documentation
  - Mast_Core is mast building blocks per se
  - Mast provide C/C++ API
  - Mast_UT is for unit testing Mast_Core and Mast libraries
  - Logger is an asynchronous logging library
  - SIT_reader is a library for parsing "SIT" formatted file in order to build a SystemModel tree
  - ICL_reader is a library for parsing "ICL" formatted file in order to build a SystemModel tree
  - Examples give some MAST simple usages examples


```
All non-generated files are under source control using GIT

## License

[MIT](https://choosealicense.com/licenses/mit/)
