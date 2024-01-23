cmake_minimum_required(VERSION 2.6)
project(SSAK)

include_directories(${LIBLOGGER_INCLUDE_DIRS})

message("")
message("================================================================================")
message(STATUS "SSAK_Translator: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "SSAK_Translator: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")


set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

set(Sources  "${CMAKE_CURRENT_SOURCE_DIR}/SHA256.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/SSAKplugin.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/aes.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/cryptoProc.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/cryptoProc.h"
             "${CMAKE_CURRENT_SOURCE_DIR}/example.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/microAES.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/usefullFunc.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/usefullFunc.h"
             "${CMAKE_CURRENT_SOURCE_DIR}/SSAK_PathSelector.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/SSAK_PathSelectorFactory.cpp")
	     
set(S2IBSources  "${CMAKE_CURRENT_SOURCE_DIR}/SHA256.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/SSAKplugin.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/aes.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/cryptoProc.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/cryptoProc.h"
             "${CMAKE_CURRENT_SOURCE_DIR}/example.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/microAES.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/usefullFunc.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/usefullFunc.h"
             "${CMAKE_CURRENT_SOURCE_DIR}/SSAK_PathSelector.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/DefaultS2IBPathSelector.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/S2IB_PathSelectorFactory.cpp")

message(STATUS "SSAK: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "SSAK: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")

add_library                (SSAK SHARED     "${Sources}")
set_target_properties      (SSAK PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (SSAK PUBLIC  ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications

add_library                (S2IB SHARED     "${S2IBSources}")
set_target_properties      (S2IB PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (S2IB PUBLIC  ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications



