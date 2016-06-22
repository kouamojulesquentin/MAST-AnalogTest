set (CMAKE_CXX_COMPILER "g++-4.9")
set (CMAKE_C_COMPILER "gcc-4.9")

cmake_minimum_required(VERSION 3.5)


project(mast)

add_definitions(${EXTRA_COMPILER_FLAGS}) 
set (EXTRA_COMPILER_FLAGS "-std=gnu++1y")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILER_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_COMPILER_FLAGS}")
 
#Bring the headers, such as Student.h into the project
include_directories(Lib/include/)
include_directories(Lib/public_include/)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:

file(GLOB Lib_SOURCES "Lib/src/*")
add_library(Lib OBJECT ${Lib_SOURCES})


include_directories(cxxtest)
include_directories(Lib_UT/SystemModel/)
include_directories(Lib_UT/Utility/)
include_directories(Lib_UT/CxxTest_Traits/)
include_directories(Lib_UT/UT_Helpers/)

file(GLOB Lib_UT_SOURCES "Lib_UT/Utility/*")
file(GLOB Lib_UT_SOURCES "Lib_UT/CxxTest_Traits/*")
file(GLOB Lib_UT_SOURCES "Lib_UT/UT_Helpers/*")
file(GLOB Lib_UT_SOURCES "Lib_UT/SystemModel/*")

#add_library(Lib_UT OBJECT ${Lib_UT_SOURCES})

