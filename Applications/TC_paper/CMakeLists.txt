cmake_minimum_required(VERSION 3.5)
project(Mast_TC_paper)

message("")
message("================================================================================")
message(STATUS "Mast_TC_paper: ")
message("================================================================================")
message("")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TC_paper")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library           (TC_paper SHARED TC_paper.cpp)
set_target_properties (TC_paper PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (TC_paper Mast_API_CPP ${EXTRA_LIBS})

install(FILES TC_paper.sit DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES TC_paper.yml DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

