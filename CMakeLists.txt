cmake_minimum_required(VERSION 3.5)

if (WIN32)
	if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
		message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
	endif()
  set (CMAKE_CXX_COMPILER "g++")
  set (CMAKE_C_COMPILER   "gcc")
endif()

message("NewMast.CMake:   USE_OPEN_OCD: ${USE_OPEN_OCD}")
message("NewMast.CMake:   USE_LIBFTDISPI: ${USE_LIBFTDISPI}")


project(mast)

add_definitions(${WARNINGS})
set (WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
add_definitions(${CFLAGS})
set (CFLAGS "${WARNINGS} -std=c++14")

if (WIN32)
  set (CFLAGS ${CFLAGS})
else()
  set (CFLAGS "${CFLAGS} -fPIC")
endif()

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "../Lib")

add_subdirectory (Logger)
if(${USE_LIBFTDISPI})
	add_subdirectory (libftdispi)
endif()
add_subdirectory (Mast_Core)
add_subdirectory (SIT_reader)
add_subdirectory (Mast)
add_subdirectory (Mast_UT)
add_subdirectory (SIT_reader_UT)
add_subdirectory (TestCasesApp)
if (NOT WIN32)
add_subdirectory (License)
endif()

