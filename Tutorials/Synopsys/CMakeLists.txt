cmake_minimum_required(VERSION 3.5)
project(Synopsys)

message("")
message("================================================================================")
message(STATUS "Synopsys: ")
message("================================================================================")
message("")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Synopsys")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library           (Synopsys SHARED Synopsys.cpp)
set_target_properties (Synopsys PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (Synopsys Mast_API_CPP ${EXTRA_LIBS})

install(FILES Synopsys.yml    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES Synopsys.lst    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES SReg.icl              DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES xlbist_des_unit.icl      DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES Tap_and_des_unit.icl DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES TOP.bsd               DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(CTest)
add_test(NAME Smoke:Synopsys COMMAND Mast
                                           --check
                                           --plugin_dir=Synopsys
                                           --conf=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Synopsys.yml
                                           --icl_list=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Synopsys.lst)
