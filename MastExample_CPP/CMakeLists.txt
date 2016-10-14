# Mast usage example using C++ API

cmake_minimum_required(VERSION 3.5)
project(MastExample_CPP)

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(../Mast_Core/include/)
include_directories(../Mast/CPP_API/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)
include_directories(../SIT_reader/)

include_directories(kissfft/)
include_directories(kissfft/tools)


if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable 				(MastExample_CPP main.cpp Example.cpp)
set_target_properties	(MastExample_CPP PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

add_executable 		(Tutorial main.cpp Tutorial.cpp)
set_target_properties	(Tutorial PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

add_library(kiss_fft SHARED
  # main FFT implementation
  kissfft/kiss_fft.c
  # from the tools/ directory
  kissfft/tools/kfc.c 
  kissfft/tools/kiss_fftnd.c 
  kissfft/tools/kiss_fftndr.c 
  kissfft/tools/kiss_fftr.c
  )

add_executable 				(ML505 main.cpp ml505_demo.cpp)
set_target_properties	(ML505 PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

add_executable 				(ML505SPI main.cpp ml505_spi.cpp)
set_target_properties	(ML505SPI PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32)
	target_link_libraries(MastExample_CPP Mast Mast_Core Logger SIT_Reader_Lib DbgHelp )
	target_link_libraries(Tutorial Mast Mast_Core Logger SIT_Reader_Lib DbgHelp )
	target_link_libraries(ML505 Mast Mast_Core Logger SIT_Reader_Lib DbgHelp kiss_fft)
	target_link_libraries(ML505SPI Mast Mast_Core Logger SIT_Reader_Lib DbgHelp )
else ()
	target_link_libraries(MastExample_CPP Mast Mast_Core Logger SIT_Reader_Lib pthread dl)
	target_link_libraries(Tutorial Mast Mast_Core Logger SIT_Reader_Lib pthread dl)
	target_link_libraries(ML505 Mast Mast_Core Logger SIT_Reader_Lib pthread dl kiss_fft)
	target_link_libraries(ML505SPI Mast Mast_Core Logger SIT_Reader_Lib pthread dl)
endif ()
