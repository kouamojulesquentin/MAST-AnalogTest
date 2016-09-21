-----------------------------------------------------------------------
New mast project is organized into sub-projects.

 1 - Lib is mast building blocks per se
 2 - Lib_UT is for unit testing Lib
 3 - Lib_UT_Runner is a "pseudo" project for Codeblocks to create unit test main: Runner.cpp before Lib_UT build
 4 - Logger is an asynchronous logging library
 5 - SIT_parser is a library for parsing "SIT" formatted file in order to build a SystemModel tree
 6 - SIT_parser_UT is for unit testing SIT_parser


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
