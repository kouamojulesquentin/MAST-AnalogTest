project(Mast_Core)

include_directories(include/)
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(LIBUSB_CFLAGS             "")
set(HIDAPI_CFLAGS             "")
set(LIBFTDISPI_CFLAGS         "")
set(LIBMAST_CORE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/" PARENT_SCOPE)


message("")
message("================================================================================")
message(STATUS "Mast_Core:  USE_OPEN_OCD:               ${USE_OPEN_OCD}")
message(STATUS "Mast_Core:  USE_LIBFTDISPI:             ${USE_LIBFTDISPI}")
message("================================================================================")
message("")


#+if (${USE_OPEN_OCD})
#+  message("")
#+  message("================================================================================")
#+  message(STATUS "Mast_Core: OPEN_OCD_LIBRARIES:        ${OPEN_OCD_LIBRARIES}")
#+  message(STATUS "Mast_Core: LIBOPEN_OCD_INCLUDE_DIRS:  ${LIBOPEN_OCD_INCLUDE_DIRS}")
#+  message("================================================================================")
#+  message("")

#+  # OpenOCD has C header files but using gcc with -std=c++14 args creates warnings
#+  # (like if C99 was not enabled).
#+  # by calling these third-party software headers with the SYSTEM attribute in the
#+  # include_directories() function, we skip these warnings.
#+#+  include_directories(SYSTEM ${LIBOPEN_OCD_INCLUDE_DIRS} ${LIBMASTOPEN_OCD_INCLUDE_DIRS})

#+  # Trying to find libusb0
#+  find_path(LIBUSB0_INCLUDE_DIR
#+    NAMES usb.h
#+    PATH_SUFFIXES "include" "libusb")

#+  find_library(LIBUSB0_LIBRARY
#+    NAMES usb
#+    PATH_SUFFIXES "lib" "lib32" "lib64" )

#+  # Trying to find libusb1
#+  find_path(LIBUSB1_INCLUDE_DIR
#+    NAMES libusb.h
#+    PATH_SUFFIXES "include" "libusb-1.0")

#+  find_library(LIBUSB1_LIBRARY
#+    NAMES usb-1.0
#+    PATH_SUFFIXES "lib" "lib32" "lib64" )

#+  if(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)
#+      set(LIBUSB_CFLAGS "-lusb-1.0")
#+  else(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)

#+      if(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)
#+          set(LIBUSB_CFLAGS "-lusb")
#+      endif(LIBUSB0_INCLUDE_DIR AND LIBUSB0_LIBRARY)

#+  endif(LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARY)

#+  # Trying to find HIDAPI
#+  find_library(HIDAPI_LIBRARY
#+    NAMES hidapi hidapi-libusb)

#+  find_path(HIDAPI_INCLUDE_DIR
#+    NAMES hidapi.h
#+    PATH_SUFFIXES "hidapi")

#+  if(HIDAPI_INCLUDE_DIR AND HIDAPI_LIBRARY)
#+      set(HIDAPI_FOUND true)
#+      set(HIDAPI_CFLAGS "--Ihidapi/hidapi")
#+  endif(HIDAPI_INCLUDE_DIR AND HIDAPI_LIBRARY)

#+endif() # if (${USE_OPEN_OCD})

if (WIN32)
  set (CFLAGS "${CFLAGS} -D__LITTLE_ENDIAN=1 -D__BIG_ENDIAN=2 -D__BYTE_ORDER=1")
endif()


file(GLOB Mast_Core_SOURCES "src/*")
add_library						(Mast_Core SHARED ${Mast_Core_SOURCES})
set_target_properties	(Mast_Core PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
endif()

#+if (${USE_OPEN_OCD})
#+  set(EXTRA_LIBS ${EXTRA_LIBS} ${OPEN_OCD_LIBRARIES} ${LIBUSB_CFLAGS} ${HIDAPI_CFLAGS} dl)
#+endif()

target_link_libraries(Mast_Core Logger ${EXTRA_LIBS})
