cmake_minimum_required(VERSION 2.6)
project(PDL_Player_Translator)

include_directories(${LIBLOGGER_INCLUDE_DIRS})

message("")
message("================================================================================")
message(STATUS "PDL_Player_Translator: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "PDL_Player_Translator: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")


set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

set(Sources  "${CMAKE_CURRENT_SOURCE_DIR}/PDL_Player_TranslatorProtocol.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/PDL_Player_ProtocolFactory.cpp")
                      

message(STATUS "PDL_Player: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "PDL_Player: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")

add_library                (PDL_PlayerTranslator SHARED     "${Sources}")
set_target_properties      (PDL_PlayerTranslator PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (PDL_PlayerTranslator PUBLIC  ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications



