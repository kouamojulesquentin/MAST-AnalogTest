project(libftdispi)

include_directories(include/)

if (WIN32)
  set (CFLAGS "${CFLAGS} -D__LITTLE_ENDIAN=1 -D__BIG_ENDIAN=2 -D__BYTE_ORDER=1")
#else(${USE_OPEN_OCD})
#  set (CFLAGS "${CFLAGS} -lusb-1.0")
endif()

find_path(FTDI_INCLUDE_DIR
          NAMES ftdi.h
          PATHS /usr/local/include
                /usr/include
                /usr/include/libftdi
                /usr/local/include/libftdi)

find_library(FTDI_LIBRARY
             NAMES ftdi
             PATHS /usr/lib
                   /usr/local/lib)

file(GLOB libftdispi_SOURCES "src/*")
add_library						(libftdispi SHARED ${libftdispi_SOURCES})
set_target_properties	(libftdispi PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
	target_link_libraries(libftdispi)
else ()
	target_link_libraries(libftdispi ftdi)
endif ()
