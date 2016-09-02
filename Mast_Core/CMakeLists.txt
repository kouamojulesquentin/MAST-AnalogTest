project(mast_lib)

include_directories(include/)
include_directories(public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)

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
  set (CFLAGS "${CFLAGS} -D__LITTLE_ENDIAN=1 -D__BIG_ENDIAN=2 -D__BYTE_ORDER=1")
else()
  set (CFLAGS "${CFLAGS} -lusb-1.0")
endif()


file(GLOB Lib_SOURCES "src/*")
add_library						(Mast_Core SHARED ${Lib_SOURCES})
set_target_properties	(Mast_Core PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
	target_link_libraries(Mast_Core Logger DbgHelp)
else ()
	target_link_libraries(Mast_Core Logger pthread openocd ${LIBUSB_CFLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/../openocd/openocd/jimtcl/libjim.a dl)
endif ()
