cmake_minimum_required(VERSION 3.5)

if (WIN32)
	if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
		message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
	endif()
  set (CMAKE_CXX_COMPILER "g++")
  set (CMAKE_C_COMPILER   "gcc")
endif()



project(mast)

add_definitions(${WARNINGS})
set (WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
add_definitions(${CFLAGS})
set (CFLAGS "${WARNINGS} -std=c++14")

if (WIN32)
#+  set (CFLAGS "${CFLAGS} -DBUILD_DLL")
  set (CFLAGS ${CFLAGS})
else()
  set (CFLAGS "${CFLAGS} -fPIC")
endif()

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")

add_subdirectory (Logger)
if (NOT WIN32)
    add_subdirectory (openocd)
endif()
add_subdirectory (Mast_Core)
add_subdirectory (Mast)
add_subdirectory (Mast_UT)
add_subdirectory (SIT_reader)
add_subdirectory (SIT_reader_UT)
add_subdirectory (TestCasesApp)

