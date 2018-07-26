cmake_minimum_required(VERSION 2.6)
project(MastRpcTranslator)

message("")
message("================================================================================")
message(STATUS "MastRpcTranslator: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "MastRpcTranslator: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")

if (WIN32)
  message(FATAL_ERROR "MastRpcTranslator: WIN32 is not supported for RPC")
endif()


set(LIBMASTRPC_TRANS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/." PARENT_SCOPE)

include_directories(${LIBMAST_CORE_INTERNAL_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

file(GLOB Sources *.cpp)

message(STATUS "MastRpcTranslator: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MastRpcTranslator: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")

add_library                (Mast_RPCTranslator SHARED     "${Sources}")
set_target_properties      (Mast_RPCTranslator PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (Mast_RPCTranslator PUBLIC  ${LIBMASTRPC_INCLUDE_DIRS}
                                             ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications
target_link_libraries      (Mast_RPCTranslator Mast_Core Logger)


