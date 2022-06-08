cmake_minimum_required(VERSION 2.6)
project(MSPG_Translator)

include_directories(${LIBLOGGER_INCLUDE_DIRS})

message("")
message("================================================================================")
message(STATUS "MSPG_Translator: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "MSPG_Translator: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")


set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

set(Sources  "${CMAKE_CURRENT_SOURCE_DIR}/MSPG_TranslatorProtocol.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/MSPG_ProtocolFactory.cpp")
                      

message(STATUS "MSPG: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MSPG: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")

add_library                (MSPGTranslator SHARED     "${Sources}")
set_target_properties      (MSPGTranslator PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (MSPGTranslator PUBLIC  ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications



