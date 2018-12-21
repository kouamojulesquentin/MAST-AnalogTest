cmake_minimum_required(VERSION 3.5)
project(Mast_Graphcore)

message("")
message("================================================================================")
message(STATUS "Mast_Graphcore: ")
message("================================================================================")
message("")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Graphcore")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library           (Graphcore SHARED Graphcore.cpp)
set_target_properties (Graphcore PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (Graphcore Mast_API_CPP ${EXTRA_LIBS})

install(FILES Graphcore.sit Graphcore_hierarchical.sit Graphcore_horizontal.sit Graphcore_horizontal_stress.sit Module_8.sit Replica_100x8.sit Graphcore_hierarchical_deep.sit Graphcore_hierarchical.sit Graphcore_hierarchical_stress.sit DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES Graphcore.yml DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


