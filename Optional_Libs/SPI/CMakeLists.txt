cmake_minimum_required(VERSION 2.6)
project(MastSpi)

message("")
message("================================================================================")
message(STATUS "MastSpi: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "MastSpi: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "MastSpi: LIBMAST_CORE_INCLUDE_DIRS: ${LIBMAST_CORE_INCLUDE_DIRS}")

if (WIN32)
  message(FATAL_ERROR "MastSpi: WIN32 is not supported for SPI")
endif()


include_directories(./include)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBFTDI_SPI_INCLUDE_DIRS})

set(LIBMASTSPI_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

file(GLOB Sources src/*.cpp)

message(STATUS "MastSpi: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MastSpi: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")

add_library           (Mast_SPI SHARED     "${Sources}")
set_target_properties (Mast_SPI PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_link_libraries (Mast_SPI)


