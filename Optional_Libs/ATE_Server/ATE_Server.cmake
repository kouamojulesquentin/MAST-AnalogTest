# This file is intended to build project(s) needed to build a "remote" server on ATE
# It must be renamed as CMakeLists.txt ATE server project top directory


cmake_minimum_required(VERSION 2.6)
project(ATE_Server)

set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/External_Libs/CMake ${CMAKE_MODULE_PATH})

set(BUILD_ATE_SERVER               ON)
set(USE_XML_RPC                    ON)
set(LIBMAST_CORE_INCLUDE_DIRS      "${CMAKE_CURRENT_SOURCE_DIR}/Mast_Headers")
set(XmlRpcInstall                  "${CMAKE_CURRENT_SOURCE_DIR}/External_Libs/xmlrpc-c/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Lib")
set(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

message("")
message("================================================================================")
message(STATUS "Master: CMAKE_MODULE_PATH:              ${CMAKE_MODULE_PATH}")
message(STATUS "Master: CMAKE_CURRENT_SOURCE_DIR:       ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Master: CMAKE_CURRENT_LIST_DIR:         ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "Master: CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Master: CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "Master: LIBMAST_CORE_INCLUDE_DIRS       ${LIBMAST_CORE_INCLUDE_DIRS}")
message(STATUS "Master: XmlRpcInstall:                  ${XmlRpcInstall}")
message("================================================================================")
message("")

add_subdirectory(Optional_Libs)

