# Test cases application

cmake_minimum_required(VERSION 3.5)
project(TestCasesApp)

message("")
message("================================================================================")
message(STATUS "TestCasesApp: INTEL_EXPERIMENT:         ${INTEL_EXPERIMENT}")
message(STATUS "TestCasesApp: LIBMASTSPI_INCLUDE_DIRS:  ${LIBMASTSPI_INCLUDE_DIRS}")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBMAST_C_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBSIT_READER_INCLUDE_DIRS})
include_directories(${LIBMASTSPI_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

if (WIN32)
  message(STATUS "TestCasesApp: kissfft not used on Windows!!!")
  message("================================================================================")
else()
  include_directories(../MastExample_CPP/)
  include_directories(../MastExample_CPP/kissfft/)
  include_directories(../MastExample_CPP/kissfft/tools/)
endif()
message("================================================================================")

if (${USE_OPEN_OCD})
  set (P_CFLAGS "${P_CFLAGS} -DUSE_OPEN_OCD")

  set(LIBUSB_CFLAGS "")
  set(HIDAPI_CFLAGS "")

  include_directories(SYSTEM ../openocd/include/)
  include_directories(SYSTEM ../openocd/include/src/)
  include_directories(SYSTEM ../openocd/include/jimtcl/)

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

  # Trying to find HIDAPI
  find_library(HIDAPI_LIBRARY
	  NAMES hidapi hidapi-libusb)

  find_path(HIDAPI_INCLUDE_DIR
	  NAMES hidapi.h
	  PATH_SUFFIXES "hidapi")

  if(HIDAPI_INCLUDE_DIR AND HIDAPI_LIBRARY)
      set(HIDAPI_FOUND true)
      set(HIDAPI_CFLAGS "--Ihidapi/hidapi")
  endif(HIDAPI_INCLUDE_DIR AND HIDAPI_LIBRARY)
endif()

if (WIN32)
  include_directories($ENV{FLEX_DIR})
  set (APP_SOURCES "main.cpp" "Options.cpp" "Zybo.cpp")
else()
  set (P_CFLAGS "${P_CFLAGS} -DUSE_KISS_FFT")
  file(GLOB APP_SOURCES "*")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable 				(TestCasesApp ${APP_SOURCES})
set_target_properties	(TestCasesApp PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32)
	target_link_libraries(TestCasesApp Mast Mast_Core Logger SIT_Reader_Lib DbgHelp)
elseif( NOT ${USE_OPEN_OCD})
	target_link_libraries(TestCasesApp Mast 
                                     Mast_Core 
                                     Logger 
                                     SIT_Reader_Lib 
                                     ${LIBMASTSPI_LIBRARIES} 
                                     kiss_fft
                                     pthread
                                     dl)
else ()
    # $LIBUSB_CFLAGS, ldl and libjim.a args are required by OpenOCD.
	target_link_libraries(TestCasesApp Mast Mast_Core Logger SIT_Reader_Lib ${LIBMASTSPI_LIBRARIES} pthread ${CMAKE_CURRENT_SOURCE_DIR}/../openocd/lib/libopenocd.so dl kiss_fft)
endif ()
