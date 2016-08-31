project (mast)

include (CheckTypeSize)
INCLUDE (CheckIncludeFiles)
include(ExternalProject)

check_include_files (stdint.h HAVE_STDINT_H)
check_type_size(uint8_t HAVE_UINT8_T)
#check_include_files (inttypes.h HAVE_INTTYPES_H)
set(CMAKE_REQUIRED_INCLUDES stdint.h)

if (HAVE_STDINT_H)
  add_definitions(-DHAVE_STDINT)
endif()

# Configure, build and install OpenOCD from its source code through its own configure and makefile scripts.
# During ./configure, OpenOCD binaries are installed into the openocd/build dir. -fPIC CFLAGS option enables the exportation of some driver functions into a .so file.
ExternalProject_Add(OpenOCD
   DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}
   UPDATE_COMMAND ""
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openocd
   BUILD_IN_SOURCE 1
   CONFIGURE_COMMAND ./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/openocd/build CFLAGS=-fPIC
   BUILD_COMMAND make
   INSTALL_COMMAND make install
   )
   
find_path(LIBUSB0_INCLUDE_DIR
  NAMES usb.h
  PATH_SUFFIXES "include" "libusb")  
     
find_library(LIBUSB0_LIBRARY
  NAMES usb
  PATH_SUFFIXES "lib" "lib32" "lib64" )

find_path(LIBUSB1_INCLUDE_DIR
  NAMES libusb.h
  PATH_SUFFIXES "include" "libusb-1.0")  
     
find_library(LIBUSB1_LIBRARY
  NAMES usb-1.0 
  PATH_SUFFIXES "lib" "lib32" "lib64" )

set(LIBUSB_LIBRARY "")
set(LIBUSB_CFLAGS "")
set(LIBUSB_INCLUDE_DIR "")

if(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)
    set(LIBUSB1_FOUND true)
    set(LIBUSB_CFLAGS "-lusb-1.0")
    set(LIBUSB_LIBRARY ${LIBUSB1_LIBRARY})
endif(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)

if(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)
    set(LIBUSB0_FOUND true)
    if(NOT ${LIBUSB1_FOUND})
        set(LIBUSB_CFLAGS "-lusb")
        set(LIBUSB_LIBRARY ${LIBUSB0_LIBRARY})
    endif(NOT ${LIBUSB1_FOUND})
endif(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)
  

set(whole_libraries)
set(whole_lib_files)

# Here we get the .o files.
set(openocd_src_dir "${CMAKE_CURRENT_SOURCE_DIR}/openocd/src")
#FILE(GLOB_RECURSE inFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/openocd/src/"
#    "${CMAKE_CURRENT_SOURCE_DIR}/openocd/src/*.o")

FILE(READ "${CMAKE_CURRENT_SOURCE_DIR}/openocd_o_files.txt" file_content)
STRING(REGEX REPLACE ";" "\\\\;" file_content "${file_content}")
STRING(REGEX REPLACE "\n" ";" file_content "${file_content}")

FOREACH(lib_file IN LISTS file_content)
	list(APPEND whole_lib_files "${openocd_src_dir}/${lib_file}")
	#message(STATUS "Processing ${openocd_src_dir}/${lib_file}")
ENDFOREACH(lib_file)

if(${LIBUSB1_FOUND})
    MESSAGE(STATUS "[USB1] Adding LIBUSB1_COMMON.O")
   #list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-usb_common.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-libusb1_common.o")
endif(${LIBUSB1_FOUND})

IF(${LIBUSB0_FOUND})
    MESSAGE(STATUS "[USB0] Adding USB_COMMON.O")
    list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-usb_common.o")
    if(NOT ${LIBUSB1_FOUND})
        MESSAGE(STATUS "[USB0] Adding LIBUSB0_COMMON.O")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-libusb0_common.o")
    endif(NOT ${LIBUSB1_FOUND})
endif(${LIBUSB0_FOUND})

set_source_files_properties(${whole_lib_files}
                            PROPERTIES GENERATED TRUE)

include_directories(openocd/src)

include_directories(openocd)

include_directories(openocd/jimtcl)

IF(${LIBUSB0_FOUND})
    include_directories(${LIBUSB0_INCLUDE_DIR})
ENDIF(${LIBUSB0_FOUND})

IF(${LIBUSB1_FOUND})
    include_directories(${LIBUSB1_INCLUDE_DIR})
ENDIF(${LIBUSB1_FOUND})


# note, this will only work on linux/unix platforms, also it does building
# in the source tree which is also sort of bad style and keeps out of source 
# builds from working.  
#add_executable (dev_openocd_app dev_openocd.c)

# -ldl is needed by JimTcl, -lusb-1.0 is needed for FTDI drivers, and -lm for <math.h>.
set(CMAKE_C_FLAGS "-std=c99 -D_GNU_SOURCE -lm -ldl ${LIBUSB_CFLAGS}")

#set_property(TARGET dev_openocd_app PROPERTY C_STANDARD 99) # TODO: plus joli mais ne fonctionne pas WTF?

add_library(openocd SHARED dummy.c) #dev_openocd.c)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/openocd/src)
#target_include_directories(dev_openocd_app ${LIBUSB_INCLUDE_DIR})
target_link_libraries(openocd
${whole_lib_files} ${LIBUSB1_LIBRARY} ${LIBUSB0_LIBRARY} ${CMAKE_CURRENT_SOURCE_DIR}/openocd/jimtcl/libjim.a)
#target_link_libraries(dev_openocd_app openocd)

add_dependencies(openocd OpenOCD)
#add_dependencies(dev_openocd_app openocd)
