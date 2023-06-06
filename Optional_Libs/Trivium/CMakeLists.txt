cmake_minimum_required(VERSION 2.6)
project(Trivium)

include_directories(${LIBLOGGER_INCLUDE_DIRS})

message("")
message("================================================================================")
message(STATUS "Trivium: CMAKE_PREFIX_PATH:         ${CMAKE_PREFIX_PATH}")
message(STATUS "Trivium: CMAKE_CXX_COMPILER_ID:     ${CMAKE_CXX_COMPILER_ID}")


set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

set(Sources  "${CMAKE_CURRENT_SOURCE_DIR}/Trivium_TranslatorProtocol.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/Trivium_ProtocolFactory.cpp")
                      

set(Streamer_Sources  "${CMAKE_CURRENT_SOURCE_DIR}/TriviumStreamerProtocol.cpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/TriviumStreamerProtocolFactory.cpp")

set (Python_ADDITIONAL_VERSIONS "2.7.12") 
find_package(PythonLibs 2.7 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
message("PYTHONLIBS_VERSION_STRING: ${PYTHONLIBS_VERSION_STRING}")

message(STATUS "Trivium: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "Trivium: FLAGS:                   ${FLAGS}")
message("================================================================================")
message("")

add_library                (Trivium SHARED     "${Sources}")
set_target_properties      (Trivium PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (Trivium PUBLIC  ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications
target_link_libraries      (Trivium ${PYTHON_LIBRARIES})

install(FILES trivium.py setupTrivium.sh DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_executable           (Trivium_proto Trivium.cpp )
set_target_properties (Trivium_proto PROPERTIES COMPILE_FLAGS "${P_CFLAGS} -g" )
target_link_libraries(Trivium_proto  ${PYTHON_LIBRARIES} Mast_Core pthread)

add_library                (TriviumStreamerProtocol SHARED     "${Streamer_Sources}")
set_target_properties      (TriviumStreamerProtocol PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_include_directories (TriviumStreamerProtocol PUBLIC  ${LIBMAST_CORE_INCLUDE_DIRS})   # Necessary because we do not link to Mast_Core - it is public for usage by dependent applications
target_link_libraries      (TriviumStreamerProtocol ${PYTHON_LIBRARIES})

