project(Intel_Experimental)

message("")
message("================================================================================")
message(STATUS "Intel: INTEL_EXPERIMENT:      ${INTEL_EXPERIMENT}")

if (WIN32)
  message(STATUS "Intel: Lib 'Intel' is not build on Windows !!!")
else()
  set (CFLAGS "${CFLAGS}")

  set(INTEL_EXP_LIBRARIES   "Intel_Exp"               PARENT_SCOPE)
  set(LIBINTEL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

  file(GLOB Intel_SOURCES "src/*.cpp")
  set(Intel_Headers "${CMAKE_CURRENT_SOURCE_DIR}/include/ClientSocket.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/DFT_Ring_NW.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/Intel_EmulationProtocol.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/Intel_Packet_Player.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/ServerSocket.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/Socket.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/SocketException.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/blockingqueue.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/threadsafe_queue.hpp")

  add_library                (Intel_Exp SHARED     ${Intel_SOURCES} ${Intel_Headers})
  set_target_properties      (Intel_Exp PROPERTIES COMPILE_FLAGS "${CFLAGS}")
  target_include_directories (Intel_Exp PUBLIC     ${LIBINTEL_INCLUDE_DIRS})
  target_link_libraries      (Intel_Exp Mast_Core SIT_Reader)

  #This emulation backend is based on Linux Sockets

  add_executable (NW_Region       test/NW_Region.cpp)
  add_executable (NW_Test_IF      test/NW_Test_IF.cpp)
  add_executable (NW_Test_IF_end  test/NW_Test_IF_end.cpp)
  add_executable (NW_Test_IF_MAST test/NW_Test_IF_MAST.cpp)

  set_target_properties (NW_Region       PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")
  set_target_properties (NW_Test_IF      PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")
  set_target_properties (NW_Test_IF_end  PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")
  set_target_properties (NW_Test_IF_MAST PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")

  target_include_directories(NW_Region       PUBLIC ${LIBINTEL_INCLUDE_DIRS})
  target_include_directories(NW_Test_IF      PUBLIC ${LIBINTEL_INCLUDE_DIRS})
  target_include_directories(NW_Test_IF_end  PUBLIC ${LIBINTEL_INCLUDE_DIRS})
  target_include_directories(NW_Test_IF_MAST PUBLIC ${LIBINTEL_INCLUDE_DIRS})

  target_link_libraries (NW_Region       Intel_Exp pthread dl)
  target_link_libraries (NW_Test_IF      Intel_Exp pthread dl)
  target_link_libraries (NW_Test_IF_end  Intel_Exp pthread dl)
  target_link_libraries (NW_Test_IF_MAST Intel_Exp pthread dl)
endif()

message("================================================================================")
message("")


