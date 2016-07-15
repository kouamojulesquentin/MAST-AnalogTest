if (WIN32)
	if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
		message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
	endif()
  set (CMAKE_CXX_COMPILER "g++")
  set (CMAKE_C_COMPILER   "gcc")
endif()

cmake_minimum_required(VERSION 3.5)


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

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

#add_compile_options(${CFLAGS})

#Bring the headers, such as Student.h into the project
include_directories(Lib/include/)
include_directories(Lib/public_include/)
include_directories(Logger/)
include_directories(Logger/g3log/)
include_directories(Logger/Unix)
include_directories(Logger/Windows)

add_subdirectory (Logger)

# Lib:
file(GLOB Lib_SOURCES "Lib/src/*")
#add_library(Lib OBJECT ${Lib_SOURCES})
add_library(Lib SHARED ${Lib_SOURCES})

set_target_properties(Lib  PROPERTIES
  COMPILE_FLAGS "${CFLAGS}"
  )

if (WIN32)
	target_link_libraries(Lib Logger DbgHelp)
else ()
	target_link_libraries(Lib Logger pthread)
endif ()

add_subdirectory (Lib_UT)
add_subdirectory (Lib_UT_Runner)
add_subdirectory (SIT_parser)
