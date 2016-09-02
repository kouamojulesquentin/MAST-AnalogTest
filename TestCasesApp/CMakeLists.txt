# Test cases application

cmake_minimum_required(VERSION 3.5)
project(TestCasesApp)

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(../Mast_Core/include/)
include_directories(../Mast_Core/public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)
include_directories(../SIT_reader/)

set(LIBUSB_CFLAGS "")
if (NOT WIN32)
  include_directories(../openocd/)
  include_directories(../openocd/openocd/src/)
  include_directories(../openocd/openocd/jimtcl/)

  # Trying to find libusb0
  find_path(LIBUSB0_INCLUDE_DIR
    NAMES usb.h
    PATH_SUFFIXES "include" "libusb")

  find_library(LIBUSB0_LIBRARY
    NAMES usb
    PATH_SUFFIXES "lib" "lib32" "lib64" )

  # Trying to find libusb1
  find_path(LIBUSB1_INCLUDE_DIR
    NAMES libusb.h
    PATH_SUFFIXES "include" "libusb-1.0")

  find_library(LIBUSB1_LIBRARY
    NAMES usb-1.0
    PATH_SUFFIXES "lib" "lib32" "lib64" )

  if(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)
      set(LIBUSB_CFLAGS "-lusb-1.0")
  else(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)

      if(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)
          set(LIBUSB_CFLAGS "-lusb")
      endif(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)

  endif(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)
endif()

if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()

file(GLOB APP_SOURCES "*")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable 				(TestCasesApp ${APP_SOURCES})
set_target_properties	(TestCasesApp PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32)
	target_link_libraries(TestCasesApp Mast_Core Logger SIT_Reader_Lib DbgHelp )
else ()
    # $LIBUSB_CFLAGS, ldl and libjim.a args are required by OpenOCD.
	target_link_libraries(TestCasesApp Mast_Core Logger SIT_Reader_Lib pthread openocd ${LIBUSB_CFLAGS} -ldl ${CMAKE_CURRENT_SOURCE_DIR}/../openocd/openocd/jimtcl/libjim.a dl)
endif ()
