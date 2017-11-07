### SET(CMAKE_SYSTEM_NAME Linux)  # Tell CMake we're cross-compiling
### include(CMakeForceCompiler)
### # Prefix detection only works with compiler id "GNU"
### # CMake will look for prefixed g++, cpp, ld, etc. automatically
### CMAKE_FORCE_CXX_COMPILER( arm-linux-gnueabihf-g++ GNU)

#SET(ARM_PREFIX "arm-none-eabi-")
# where is the target environment 
#SET(CMAKE_FIND_ROOT_PATH  /opt/gcc-arm-none-eabi-6-2017-q2-update/bin/)

SET(ARM_PREFIX "arm-linux-gnueabihf-")
#Use this when arm-linux-gnueabihf-gcc is installed system-wide
#SET(CMAKE_FIND_ROOT_PATH /usr/bin)

#Using petalinux local gcc beacause Petalinux requires gcc-multilib, so
# arm-linux-gnueabihf-gcc cannot be installed system-wide
#NB: FlexLexer.h must be manually added to includes
SET(CMAKE_FIND_ROOT_PATH /opt/Xilinx/petalinux/tools/linux-i386/gcc-arm-linux-gnueabi/bin)


# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   ${ARM_PREFIX}gcc)
SET(CMAKE_CXX_COMPILER ${ARM_PREFIX}g++)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

INCLUDE(CMakeForceCompiler)


# specify the cross compiler
CMAKE_FORCE_C_COMPILER(${ARM_PREFIX}gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${ARM_PREFIX}g++ GNU)

