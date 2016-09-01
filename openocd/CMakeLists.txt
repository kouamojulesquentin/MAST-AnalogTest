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

set(LIBUSB_CFLAGS "")

if(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)
    set(LIBUSB1_FOUND true)
    set(LIBUSB_CFLAGS "-lusb-1.0")
endif(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)

if(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)
    set(LIBUSB0_FOUND true)
    if(NOT ${LIBUSB1_FOUND})
        set(LIBUSB_CFLAGS "-lusb")
    endif(NOT ${LIBUSB1_FOUND})
endif(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)
  

set(whole_libraries)
set(whole_lib_files)

# Here we get the .o files.
set(openocd_src_dir "${CMAKE_CURRENT_SOURCE_DIR}/openocd/src")

# Each line in openocd_o_files.txt corresponds to an existing .o file once OpenOCD is built.
FILE(READ "${CMAKE_CURRENT_SOURCE_DIR}/openocd_o_files.txt" file_content)
STRING(REGEX REPLACE ";" "\\\\;" file_content "${file_content}")
STRING(REGEX REPLACE "\n" ";" file_content "${file_content}")

FOREACH(lib_file IN LISTS file_content)
	list(APPEND whole_lib_files "${openocd_src_dir}/${lib_file}")
	#message(STATUS "Processing ${openocd_src_dir}/${lib_file}")
ENDFOREACH(lib_file)

# LIBUSB-RELATED DEPENDENCIES:
# Some adapter drivers are generated according to the locally installed versions of libusb.
# Please check openocd/configure (tests are on variable '$use_libusbX', where X is the libusb version),
# and openocd/src/jtag/drivers/Makefile.am to establish the link between the dependencies
# of the libusb version, and each driver.

# Some .o files are generated only if >libusb 1.0.0 is installed.
if(${LIBUSB1_FOUND})
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-libusb1_common.o")

   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-ftdi.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-mpsse.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/hla/hla_transport.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/hla/hla_tcl.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/hla/hla_interface.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/hla/hla_layout.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-stlink_usb.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-ti_icdi_usb.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-ulink.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/usb_blaster/usb_blaster.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/usb_blaster/ublast2_access_libusb.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/versaloon/usbtoxxx/libocdjtagdrivers_la-usbtogpio.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/versaloon/usbtoxxx/libocdjtagdrivers_la-usbtojtagraw.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/versaloon/usbtoxxx/libocdjtagdrivers_la-usbtoswd.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/versaloon/usbtoxxx/libocdjtagdrivers_la-usbtopwr.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/versaloon/usbtoxxx/libocdjtagdrivers_la-usbtoxxx.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/versaloon/libocdjtagdrivers_la-versaloon.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-vsllink.o")

    # Common drivers with libusb0 and libusb1
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_transport.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_interface.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_port.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_usb.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_pipe.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-jlink.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-osbdm.o")
   list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-opendous.o")

endif(${LIBUSB1_FOUND})

# Sometimes libusb0 may already be installed. Hence, OpenOCD configuration script will automatically 
# build libusb0 specific .o file. Here we add them when needed.
IF(${LIBUSB0_FOUND})
    list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-usb_common.o")
    list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-rlink.o")
    list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-rlink_speed_table.o")
    list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-arm-jtag-ew.o")
    list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-usbprog.o")

    if(NOT ${LIBUSB1_FOUND})
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-libusb0_common.o")
    # Common drivers with libusb0 and libusb1
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_transport.o")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_interface.o")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_port.o")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_usb.o")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/aice/aice_pipe.o")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-jlink.o")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-osbdm.o")
       list(APPEND whole_lib_files "${openocd_src_dir}/jtag/drivers/libocdjtagdrivers_la-opendous.o")
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
set(CMAKE_C_FLAGS "-std=c99 -D_GNU_SOURCE -lm ${LIBUSB_CFLAGS}")

#set_property(TARGET dev_openocd_app PROPERTY C_STANDARD 99) # TODO: plus joli mais ne fonctionne pas WTF?

add_library(openocd SHARED dummy.c) #dev_openocd.c)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/openocd/src)
#target_include_directories(dev_openocd_app ${LIBUSB_INCLUDE_DIR})
IF(${LIBUSB1_FOUND})

    IF(${LIBUSB0_FOUND})
        target_link_libraries(openocd
${whole_lib_files} ${LIBUSB1_LIBRARY} ${LIBUSB0_LIBRARY} ${CMAKE_CURRENT_SOURCE_DIR}/openocd/jimtcl/libjim.a dl)
    ELSE(${LIBUSB0_FOUND})
        target_link_libraries(openocd
${whole_lib_files} ${LIBUSB1_LIBRARY} ${CMAKE_CURRENT_SOURCE_DIR}/openocd/jimtcl/libjim.a dl)
    ENDIF(${LIBUSB0_FOUND})

ELSE(${LIBUSB1_FOUND})

    IF(${LIBUSB0_FOUND})
        target_link_libraries(openocd
${whole_lib_files} ${LIBUSB0_LIBRARY} ${CMAKE_CURRENT_SOURCE_DIR}/openocd/jimtcl/libjim.a dl)
    ELSE(${LIBUSB0_FOUND})
        target_link_libraries(openocd
${whole_lib_files} ${CMAKE_CURRENT_SOURCE_DIR}/openocd/jimtcl/libjim.a dl)
    ENDIF(${LIBUSB0_FOUND})


ENDIF(${LIBUSB1_FOUND})
#target_link_libraries(dev_openocd_app openocd)

SET_SOURCE_FILES_PROPERTIES( ${CMAKE_CURRENT_SOURCE_DIR}/openocd/config.h GENERATED)
#set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/openocd/config.h OBJECT_DEPENDS OpenOCD)
#add_dependencies(openocd OpenOCD ${CMAKE_CURRENT_SOURCE_DIR}/openocd/config.h)
#add_dependencies(dev_openocd_app openocd)
