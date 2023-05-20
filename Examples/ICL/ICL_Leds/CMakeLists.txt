cmake_minimum_required(VERSION 3.5)
project(Mast_ICL_Tutorial_2)

message("")
message("================================================================================")
message(STATUS "Mast_ICL_Tutorial_2: ")
message("================================================================================")
message("")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ICL_Tutorial_2")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library           (ICL_Tutorial_2 SHARED ICL_Tutorial_2.cpp)
set_target_properties (ICL_Tutorial_2 PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (ICL_Tutorial_2 Mast_API_CPP ${EXTRA_LIBS})

install(FILES ICL_Tutorial_2.yml    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES ICL_Tutorial_2.lst    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES Tap_and_Daisy_3WI.icl DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES Instrument.icl        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES Daisy_3WI.icl         DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES SReg.icl              DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES WrappedInstr.icl      DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(FILES TOP.bsd               DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(CTest)
add_test(NAME Smoke:ICL_Tutorial_2 COMMAND Mast
                                           --check
                                           --plugin_dir=ICL_Tutorial_2
                                           --conf=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ICL_Tutorial_2.yml
                                           --icl_list=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ICL_Tutorial_2.lst)


