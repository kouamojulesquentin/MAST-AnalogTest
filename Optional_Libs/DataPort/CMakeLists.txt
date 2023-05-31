cmake_minimum_required(VERSION 2.6)
project(DataPort)

include_directories(${LIBLOGGER_INCLUDE_DIRS})

message("")
message("================================================================================")
message(STATUS "DataPort: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "DataPort: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")


set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

set(Sources  "${CMAKE_CURRENT_SOURCE_DIR}/DataPort_Protocol.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/DataPort_ProtocolFactory.cpp")
                      
  
message("================================================================================")
message("")

add_library                (DataPort_Protocol SHARED     "${Sources}")
set_target_properties      (DataPort_Protocol PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (DataPort_Protocol PUBLIC  ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications



