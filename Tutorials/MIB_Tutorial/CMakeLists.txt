cmake_minimum_required(VERSION 3.5)
project(Mast_Debug_mux)

message("")
message("================================================================================")
message(STATUS "MIB_Tutorial: ")
message("================================================================================")
message("")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MIB_Tutorial")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library           (MIB_Tutorial SHARED MIB_Tutorial.cpp)
set_target_properties (MIB_Tutorial PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (MIB_Tutorial Mast_API_CPP ${EXTRA_LIBS})

install(FILES MIB_Tutorial.yml DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

