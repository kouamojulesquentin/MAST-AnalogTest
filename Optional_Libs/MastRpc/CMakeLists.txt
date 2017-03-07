cmake_minimum_required(VERSION 2.6)
project(MastRpc)

message("")
message("================================================================================")
message(STATUS "MastRpc: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "MastRpc: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "MastRpc: LIBMAST_CORE_INCLUDE_DIRS: ${LIBMAST_CORE_INCLUDE_DIRS}")

if (WIN32)
  message(FATAL_ERROR "MastRpc: WIN32 is not supported for RPC")
endif()


include_directories(.)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})

set(LIBMASTRPC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/." PARENT_SCOPE)

set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

file(GLOB Sources *.cpp)

message(STATUS "MastRpc: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MastRpc: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")

add_library           (Mast_RPC SHARED     "${Sources}")
set_target_properties (Mast_RPC PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_link_libraries (Mast_RPC)


