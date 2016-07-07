### SET(CMAKE_SYSTEM_NAME Linux)  # Tell CMake we're cross-compiling
### include(CMakeForceCompiler)
### # Prefix detection only works with compiler id "GNU"
### # CMake will look for prefixed g++, cpp, ld, etc. automatically
### CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)


# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/bin/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/bin/)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

INCLUDE(CMakeForceCompiler)


# specify the cross compiler
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)

