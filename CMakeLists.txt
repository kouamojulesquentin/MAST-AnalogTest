cmake_minimum_required(VERSION 3.5)

message("")
message("================================================================================")
if (WIN32)
	if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
		message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
	endif()
  set (CMAKE_CXX_COMPILER "g++")
  set (CMAKE_C_COMPILER   "gcc")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/External_Libs/CMake ${CMAKE_MODULE_PATH})

project(mast)

add_definitions(${WARNINGS})
set (WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
add_definitions(${CFLAGS})
set (CFLAGS "${WARNINGS} -std=c++14 -g")

if (WIN32)
  set (CFLAGS ${CFLAGS})
else()
  set (CFLAGS "${CFLAGS} -fPIC")
endif()

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set (CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Lib")

message(STATUS "")
message(STATUS "Master: USE_OPEN_OCD:       ${USE_OPEN_OCD}")
message(STATUS "Master: USE_LIBFTDISPI:     ${USE_LIBFTDISPI}")
message(STATUS "Master: USE_XML_RPC:        ${USE_XML_RPC}")
message(STATUS "Master: INTEL_EXPERIMENT:   ${INTEL_EXPERIMENT}")
message(STATUS "")
message(STATUS "Master: CMAKE_MODULE_PATH:              ${CMAKE_MODULE_PATH}")
message(STATUS "Master: CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Master: CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

if (USE_XML_RPC)
  set(USE_EXTERNAL_LIBS ON)
  set(XmlRpcInstall ${CMAKE_CURRENT_LIST_DIR}/External_Libs/xmlrpc-c/build)
  message(STATUS "Master: XmlRpcInstall:                  ${XmlRpcInstall}")
endif()

message("================================================================================")
message("")

add_subdirectory (Logger)
add_subdirectory (Mast_Core)
add_subdirectory (SIT_reader)
add_subdirectory (Mast)

if (USE_EXTERNAL_LIBS)
  add_subdirectory(External_Libs)
  add_subdirectory(Optional_Libs)
  add_subdirectory(Optional_Libs_UT)
endif()

add_subdirectory (TestCasesApp)
add_subdirectory (MastExample_CPP)
if (NOT WIN32)
add_subdirectory (License)
endif()
add_subdirectory (Mast_UT)
add_subdirectory (SIT_reader_UT)

if(${USE_LIBFTDISPI})
	add_subdirectory (libftdispi)
endif()

if(${INTEL_EXPERIMENT})
	add_subdirectory (Intel)
endif()

