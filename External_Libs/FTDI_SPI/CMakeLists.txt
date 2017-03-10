project(ftdi_spi)

if (WIN32)
  message(STATUS "FTDI_SPI: Lib FTDI for SPI is not build on Windows !!!")
else()
  include_directories(include/)

  find_path(LIBFTDI_SPI_INCLUDE_DIRS
            NAMES ftdi.h
            PATHS /usr/local/include
                  /usr/include
                  /usr/include/libftdi
                  /usr/local/include/libftdi)

  set(LIBFTDI_SPI_INCLUDE_DIRS "${LIBFTDI_SPI_INCLUDE_DIRS}" 
                               "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                               PARENT_SCOPE)

  find_library(FTDI_LIBRARY
               NAMES ftdi
               PATHS /usr/lib
                     /usr/local/lib)

  file(GLOB FTDI_SPI_SOURCES "src/*")
  add_library           (ftdi_spi SHARED ${FTDI_SPI_SOURCES})
  set_target_properties (ftdi_spi PROPERTIES COMPILE_FLAGS "-Wall")

  target_link_libraries(ftdi_spi ftdi)
endif()
