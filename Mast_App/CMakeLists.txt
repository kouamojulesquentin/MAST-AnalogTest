# Mast usage example using C++ API

cmake_minimum_required(VERSION 3.5)
project(Mast_APP)

message("")
message("================================================================================")
message(STATUS "Mast_APP: ")
message("================================================================================")
message("")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})

if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()


add_executable        (Mast Mast_App.cpp)
set_target_properties (Mast PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
endif()

target_link_libraries(Mast Mast_API_CPP Mast_API_C SIT_Reader ${EXTRA_LIBS})
