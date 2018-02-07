cmake_minimum_required(VERSION 2.6)
project(MastSpi)

message("")
message("================================================================================")
message(STATUS "MastSpi: LIBFTDI_SPI_INCLUDE_DIRS:  ${LIBFTDI_SPI_INCLUDE_DIRS}")

if (WIN32)
  message(FATAL_ERROR "MastSpi: WIN32 is not supported for SPI")
endif()


set(LIBMASTSPI_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(LIBMASTSPI_LIBRARIES    Mast_SPI PARENT_SCOPE)

file(GLOB Sources src/*.cpp)

message(STATUS "MastSpi: CMAKE_LINK_LIBRARY_FLAG:   ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MastSpi: CFLAGS:                    ${CFLAGS}")
message("================================================================================")
message("")

add_library                (Mast_SPI SHARED     "${Sources}")
set_target_properties      (Mast_SPI PROPERTIES COMPILE_FLAGS "${CFLAGS}" )
target_include_directories (Mast_SPI PUBLIC     "${CMAKE_CURRENT_SOURCE_DIR}/include"
                                     PRIVATE     ${LIBMAST_CORE_INCLUDE_DIRS}) # Necessary because we do not link to Mast_Core

target_link_libraries      (Mast_SPI ftdi_spi Logger)


