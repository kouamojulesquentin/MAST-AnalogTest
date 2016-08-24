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

find_path(LIBUSB_INCLUDE_DIR
  NAMES libusb.h
  PATH_SUFFIXES "include" "libusb" "libusb-1.0")
find_library(LIBUSB_LIBRARY
  NAMES usb
  PATH_SUFFIXES "lib" "lib32" "lib64")

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
set_source_files_properties(${whole_lib_files}
                            PROPERTIES GENERATED TRUE)

include_directories(openocd/src)

include_directories(openocd)

include_directories(openocd/jimtcl)

include_directories(${LIBUSB_INCLUDE_DIR})


# note, this will only work on linux/unix platforms, also it does building
# in the source tree which is also sort of bad style and keeps out of source 
# builds from working.  
#add_executable (dev_openocd_app dev_openocd.c)

# -ldl is needed by JimTcl, -lusb-1.0 is needed for FTDI drivers, and -lm for <math.h>.
set(CMAKE_C_FLAGS "-std=c99 -D_GNU_SOURCE -lm -ldl -lusb-1.0")

#set_property(TARGET dev_openocd_app PROPERTY C_STANDARD 99) # TODO: plus joli mais ne fonctionne pas WTF?

add_library(openocd SHARED dummy.c) #dev_openocd.c)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/openocd/src)
#target_include_directories(dev_openocd_app ${LIBUSB_INCLUDE_DIR})
target_link_libraries(openocd
${whole_lib_files} ${LIBUSB_LIBRARY} ${CMAKE_CURRENT_SOURCE_DIR}/openocd/jimtcl/libjim.a)
#target_link_libraries(dev_openocd_app openocd)

add_dependencies(openocd OpenOCD)
#add_dependencies(dev_openocd_app openocd)
