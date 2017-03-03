# Mast usage example using C++ API

cmake_minimum_required(VERSION 3.5)
project(MastExample_CPP)

message("MastExample_CPP.CMake:   INTEL_EXPERIMENT: ${INTEL_EXPERIMENT}")
message("MastExample_CPP.CMake:   KISSFFT: ${KISSFFT}")

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


#+set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")

add_executable        (MastExample_CPP main.cpp DefaultMastMain.cpp EmptyInit.cpp Example.cpp)
set_target_properties	(MastExample_CPP PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

add_executable        (Tutorial main.cpp   DefaultMastMain.cpp EmptyInit.cpp Tutorial.cpp)
set_target_properties (Tutorial PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32) # Package png not installed on Windows ==> Cannot use kiss_fft (as is)
  target_link_libraries(MastExample_CPP Mast Mast_Core Logger SIT_Reader_Lib DbgHelp )
  target_link_libraries(Tutorial        Mast Mast_Core Logger SIT_Reader_Lib DbgHelp )
else ()
  if (${KISSFFT})

  include_directories(kissfft/)
  include_directories(kissfft/tools)

  find_package(PNG REQUIRED)
  include_directories(${PNG_INCLUDE_DIR})

  set(KissFFT_Files # main FFT implementation
                    kissfft/kiss_fft.c
                    # from the tools/ directory
                    kissfft/tools/kfc.c
                    kissfft/tools/kiss_fftnd.c
                    kissfft/tools/kiss_fftndr.c
                    kissfft/tools/kiss_fftr.c
  )
  add_library           (kiss_fft SHARED     ${KissFFT_Files})
  set_target_properties (kiss_fft PROPERTIES COMPILE_FLAGS "-Wall" )

  add_executable        (ML505 main.cpp    DefaultMastMain.cpp EmptyInit.cpp ml505_demo.cpp)
  set_target_properties (ML505 PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

  add_executable        (ETS17 main.cpp    DefaultMastMain.cpp EmptyInit.cpp ETS17.cpp)
  set_target_properties (ETS17 PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

  target_link_libraries(ML505           Mast Mast_Core SIT_Reader_Lib pthread dl kiss_fft)
  target_link_libraries(ETS17           Mast Mast_Core SIT_Reader_Lib pthread dl kiss_fft ${PNG_LIBRARY})

  endif()

  add_executable        (Intel main.cpp   DefaultMastMain.cpp EmptyInit.cpp Intel.cpp)
  set_target_properties (Intel PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

  add_executable        (ML505SPI main.cpp DefaultMastMain.cpp EmptyInit.cpp ml505_spi.cpp)
  set_target_properties (ML505SPI PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

  if (USE_XML_RPC)
    include_directories(${LIBMASTRPC_INCLUDE_DIRS})
    include_directories(${LIBMASTXMLRPC_INCLUDE_DIRS})

    add_executable        (Example_RmlRpc main.cpp DefaultMastMain.cpp Examples_Utils.cpp Example_XmlRpc.cpp)
    set_target_properties (Example_RmlRpc PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
    target_link_libraries (Example_RmlRpc Mast Mast_Core Mast_RPC_Client SIT_Reader_Lib pthread dl)
  endif()

  target_link_libraries(MastExample_CPP Mast Mast_Core SIT_Reader_Lib pthread dl)
  target_link_libraries(Tutorial        Mast Mast_Core SIT_Reader_Lib pthread dl)
  target_link_libraries(Intel           Mast Mast_Core SIT_Reader_Lib pthread dl)
  target_link_libraries(ML505SPI        Mast Mast_Core SIT_Reader_Lib pthread dl)


endif ()

message("")
message("================================================================================")
message(STATUS "MastExample_CPP: LIBMAST_CORE_INCLUDE_DIRS:       ${LIBMAST_CORE_INCLUDE_DIRS}")
message(STATUS "MastExample_CPP: LIBMAST_CPP_API_INCLUDE_DIRS:    ${LIBMAST_CPP_API_INCLUDE_DIRS}")
message(STATUS "MastExample_CPP: CMAKE_RUNTIME_OUTPUT_DIRECTORY:  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("================================================================================")
message("")

install(DIRECTORY SIT DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
