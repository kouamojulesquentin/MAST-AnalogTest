cmake_minimum_required(VERSION 3.5)
project(Examples)

message("")
message("================================================================================")
message(STATUS "Examples: ")
message("================================================================================")
message("")

set(EXAMPLE_DIRECTORY         "Examples")
set(SIT_DIRECTORY         "SIT")
set(ICL_DIRECTORY         "ICL")
set(PDL_DIRECTORY         	"PDL")
set(CONCURRENT_DIRECTORY         	"Concurrent")

message("================================================================================")
message(" Copying Examples from directory ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_DIRECTORY} file in install directory ${CMAKE_INSTALL_PREFIX}")
message("================================================================================")
message("")

#Install SIT Example files
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SIT_DIRECTORY} DESTINATION ${CMAKE_INSTALL_PREFIX}/${EXAMPLE_DIRECTORY})
#Install ICL Example files
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${ICL_DIRECTORY} DESTINATION ${CMAKE_INSTALL_PREFIX}/${EXAMPLE_DIRECTORY})
#Install Concurrent Example files
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${CONCURRENT_DIRECTORY} DESTINATION ${CMAKE_INSTALL_PREFIX}/${EXAMPLE_DIRECTORY})
#Install Example configuration file
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/Examples.yml ${CMAKE_CURRENT_SOURCE_DIR}/ICL_Leds.yml ${CMAKE_CURRENT_SOURCE_DIR}/Concurrent.yml ${CMAKE_CURRENT_SOURCE_DIR}/README.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${EXAMPLE_DIRECTORY})

#Install link to Mast executable for easier usage
INSTALL(CODE "execute_process( \
    COMMAND ${CMAKE_COMMAND} -E create_symlink \
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Mast \
    ${CMAKE_INSTALL_PREFIX}/${EXAMPLE_DIRECTORY}/Mast   \
    )"
)
add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${EXAMPLE_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${EXAMPLE_DIRECTORY})

add_library           (Examples SHARED ${PDL_DIRECTORY}/Examples.cpp)
set_target_properties (Examples PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (Examples Mast_API_CPP ${EXTRA_LIBS})

add_library           (ICL_Leds SHARED ${PDL_DIRECTORY}/ICL_Leds.cpp)
set_target_properties (ICL_Leds PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (ICL_Leds Mast_API_CPP ${EXTRA_LIBS})

add_library           (Random SHARED ${PDL_DIRECTORY}/Random.cpp)
set_target_properties (Random PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (Random Mast_API_CPP ${EXTRA_LIBS})

add_library           (LDO SHARED ${PDL_DIRECTORY}/LDO.cpp)
set_target_properties (LDO PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (LDO Mast_API_CPP ${EXTRA_LIBS})
