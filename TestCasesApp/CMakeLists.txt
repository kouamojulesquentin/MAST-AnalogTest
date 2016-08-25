# Test cases application

cmake_minimum_required(VERSION 3.5)
project(TestCasesApp)

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(../Lib/include/)
include_directories(../Lib/public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)
include_directories(../SIT_reader/)

if (NOT WIN32)
  include_directories(../openocd/)
  include_directories(../openocd/openocd/src/)
  include_directories(../openocd/openocd/jimtcl/)
endif()

if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()

file(GLOB APP_SOURCES "*")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable 				(TestCasesApp ${APP_SOURCES})
set_target_properties	(TestCasesApp PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32)
	target_link_libraries(TestCasesApp Lib Logger SIT_Reader_Lib DbgHelp )
else ()
    # lusb-1.0, ldl and libjim.a args are required by OpenOCD.
	target_link_libraries(TestCasesApp Lib Logger SIT_Reader_Lib pthread openocd -lusb-1.0 -ldl ${CMAKE_CURRENT_SOURCE_DIR}/../openocd/openocd/jimtcl/libjim.a)
endif ()
