-----------------------------------------------------------------------
New mast project is organized into sub-projects.

  - Mast_Core is mast building blocks per se
  - Mast provide C/C++ API
  - Mast_UT is for unit testing Mast_Core and Mast libraries
  - Logger is an asynchronous logging library
  - SIT_reader is a library for parsing "SIT" formatted file in order to build a SystemModel tree
  - SIT_reader_UT is for unit testing SIT_reader
  - openocd is a library to use JTAG/SPI USB adapters
  - TestCasesApp is an application for testing integration of the different libraries with different kind of testcases
  - MastExample_CPP give some MAST simple usages examples
  - Licence manages the licencing right to use MAST


Dependencies are as follow:

                     Lib_UT--\
                              \
                               -->Lib-->Logger
  SIT_parser_UT-->SIT_parser--/

All, non-generated, files are under source control using GIT

To compile and run unit tests on Linux,   see README.Linux.txt
To compile and run unit tests on Windows, see README.Windows.txt

-----------------------------------------------------------------------
ENVIRONMENT VARIABLES

MAST uses two optional environment variables:

 - "MAST_LICENSE_PATH" points to the installation directory of the license file. If not present, the license is fetched from the execution directory
 - "MAST_CONFIGURATION_PATH" points to the directory where  configuration files are stored (ex: for openOCD). If not present, the execution directory is used

 Scripts debug_settings.sh and release_settings.sh are provided for default values


                                                                                                         5-
