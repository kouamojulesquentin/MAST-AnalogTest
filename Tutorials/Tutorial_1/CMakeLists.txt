cmake_minimum_required(VERSION 3.5)
project(Mast_Tutorial_1)

message("")
message("================================================================================")
message(STATUS "Mast_Tutorial_1: LIBMAST_CORE_INCLUDE_DIRS:       ${LIBMAST_CORE_INCLUDE_DIRS}")
message("================================================================================")
message("")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Tutorial_1")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library           (Tutorial_1 SHARED Tutorial_1.cpp)
set_target_properties (Tutorial_1 PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (Tutorial_1 Mast_API_CPP ${EXTRA_LIBS})

install(FILES Tutorial_1.sit DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(CTest)
add_test(NAME Smoke:Tutorial_1 COMMAND Mast --log --log_kind=only_errors_on_cerr --log_level=error --plugin_dir=Tutorial_1 --sit=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Tutorial_1.sit)


