project(Mast_Core)

include_directories(include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)

set(LIBUSB_CFLAGS "")
set(HIDAPI_CFLAGS "")
set(LIBFTDISPI_CFLAGS "")

message("Mast_Core.CMake: USE_OPEN_OCD: ${USE_OPEN_OCD}")
message("Mast_Core.CMake: USE_LIBFTDISPI: ${USE_LIBFTDISPI}")


if (${USE_OPEN_OCD})
	set (CFLAGS "${CFLAGS} -DUSE_OPEN_OCD")

  # OpenOCD has C header files but using gcc with -std=c++14 args creates warnings
  # (like if C99 was not enabled).
  # by calling these third-party software headers with the SYSTEM attribute in the
  # include_directories() function, we skip these warnings.
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

endif() # if (${USE_OPEN_OCD})

if (${USE_LIBFTDISPI})
	set (CFLAGS "${CFLAGS} -DUSE_LIBFTDISPI")
	set	(LIBFTDISPI_CFLAGS "libftdispi")
	include_directories(SYSTEM ../libftdispi/include/)
endif() # if (${USE_LIBFTDISPI})

if (WIN32)
  set (CFLAGS "${CFLAGS} -D__LITTLE_ENDIAN=1 -D__BIG_ENDIAN=2 -D__BYTE_ORDER=1")
elseif(${USE_OPEN_OCD})
  set (CFLAGS "${CFLAGS}")
endif()


file(GLOB Mast_Core_SOURCES "src/*")
add_library						(Mast_Core SHARED ${Mast_Core_SOURCES})
set_target_properties	(Mast_Core PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
	target_link_libraries(Mast_Core Logger DbgHelp)
elseif(${USE_OPEN_OCD})
	target_link_libraries(Mast_Core Logger pthread ${LIBFTDISPI_CFLAGS} ${LIBUSB_CFLAGS} ${HIDAPI_CFLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/../openocd/lib/libopenocd.so dl) #${CMAKE_CURRENT_SOURCE_DIR}/../openocd/openocd/jimtcl/libjim.a dl)
else ()
	target_link_libraries(Mast_Core Logger pthread ${LIBFTDISPI_CFLAGS}) # TODO call LIBFTDICLAGS here too.
endif ()
