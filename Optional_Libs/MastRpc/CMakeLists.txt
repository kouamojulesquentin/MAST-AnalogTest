cmake_minimum_required(VERSION 2.6)
project(MastRpc)

message("")
message("================================================================================")
message(STATUS "MastRpc: CMAKE_PREFIX_PATH:       ${CMAKE_PREFIX_PATH}")
message(STATUS "MastRpc: CMAKE_CXX_COMPILER_ID:   ${CMAKE_CXX_COMPILER_ID}")

if (WIN32)
  message(FATAL_ERROR "MastRpc: WIN32 is not supported for RPC")
endif()

set(BaseProjectPath "../..")
include_directories(${BaseProjectPath}/Mast_Core/include/)
include_directories(.)

set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-std=c++03 -fPIC -g ${WARNINGS}")

file(GLOB Sources *.hpp *.cpp)

message(STATUS "MastRpc: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MastRpc: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")


add_library           (Mast_RPC SHARED     "${Sources}")
set_target_properties (Mast_RPC PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_link_libraries (Mast_RPC)
