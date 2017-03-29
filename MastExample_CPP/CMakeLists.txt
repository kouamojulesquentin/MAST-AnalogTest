# Mast usage example using C++ API

cmake_minimum_required(VERSION 3.5)
project(MastExample_CPP)

message("")
message("================================================================================")
message(STATUS "MastExample_CPP: LIBMAST_CORE_INCLUDE_DIRS:       ${LIBMAST_CORE_INCLUDE_DIRS}")
message(STATUS "MastExample_CPP: INTEL_EXPERIMENT                 ${INTEL_EXPERIMENT}")
message(STATUS "MastExample_CPP: KISSFFT:                         ${KISSFFT}")
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
include_directories(${LIBSIT_READER_INCLUDE_DIRS})

if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()


add_executable        (MastExample_CPP main.cpp DefaultMastMain.cpp EmptyInit.cpp Example.cpp)
set_target_properties	(MastExample_CPP PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

add_executable        (Tutorial main.cpp   DefaultMastMain.cpp EmptyInit.cpp Tutorial.cpp)
set_target_properties (Tutorial PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
endif()

if (WIN32) # Package png not installed on Windows ==> Cannot use kiss_fft (as is)
  target_link_libraries(MastExample_CPP Mast Mast_Core Logger SIT_Reader_Lib DbgHelp )
  target_link_libraries(Tutorial        Mast Mast_Core Logger SIT_Reader_Lib DbgHelp )
else ()
  if (${KISSFFT})
    include_directories(${LIBKISSFFT_INCLUDE_DIRS})
    set(EXTRA_LIBS ${EXTRA_LIBS} ${KISS_FFT_LIBRARIES})

    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})

    add_executable        (ML505 main.cpp   DefaultMastMain.cpp EmptyInit.cpp ml505_demo.cpp)
    set_target_properties (ML505 PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

    add_executable        (ETS17 main.cpp   DefaultMastMain.cpp EmptyInit.cpp ETS17.cpp)
    set_target_properties (ETS17 PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

    target_link_libraries(ML505 Mast Mast_Core SIT_Reader_Lib ${EXTRA_LIBS})
    target_link_libraries(ETS17 Mast Mast_Core SIT_Reader_Lib ${PNG_LIBRARY} ${EXTRA_LIBS})
  endif()

  add_executable        (Intel main.cpp   DefaultMastMain.cpp EmptyInit.cpp Intel.cpp)
  set_target_properties (Intel PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

  add_executable        (ML505SPI main.cpp DefaultMastMain.cpp EmptyInit.cpp ml505_spi.cpp)
  set_target_properties (ML505SPI PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

  if (USE_XML_RPC)
    include_directories(${LIBMASTRPC_INCLUDE_DIRS})
    include_directories(${LIBMASTXMLRPC_INCLUDE_DIRS})

    add_executable        (Example_XmlRpc main.cpp DefaultMastMain.cpp Examples_Utils.cpp Example_XmlRpc.cpp)
    set_target_properties (Example_XmlRpc PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
    target_link_libraries (Example_XmlRpc Mast Mast_Core Mast_RPC_Client SIT_Reader_Lib ${EXTRA_LIBS})
  endif()

  target_link_libraries(MastExample_CPP Mast Mast_Core SIT_Reader_Lib ${EXTRA_LIBS})
  target_link_libraries(Tutorial        Mast Mast_Core SIT_Reader_Lib ${EXTRA_LIBS})
  target_link_libraries(Intel           Mast Mast_Core SIT_Reader_Lib ${EXTRA_LIBS})
  target_link_libraries(ML505SPI        Mast Mast_Core SIT_Reader_Lib ${EXTRA_LIBS})
endif ()

message("")
message("================================================================================")
message(STATUS "MastExample_CPP: LIBMAST_CORE_INCLUDE_DIRS:       ${LIBMAST_CORE_INCLUDE_DIRS}")
message(STATUS "MastExample_CPP: LIBMAST_CPP_API_INCLUDE_DIRS:    ${LIBMAST_CPP_API_INCLUDE_DIRS}")
message(STATUS "MastExample_CPP: LIBKISSFFT_INCLUDE_DIRS:         ${LIBKISSFFT_INCLUDE_DIRS}")
message("================================================================================")
message("")

install(DIRECTORY SIT DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMPONENT SIT_Files)
