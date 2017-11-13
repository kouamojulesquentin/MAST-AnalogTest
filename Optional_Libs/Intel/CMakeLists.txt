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
  set(LIBINTEL_INCLUDE_DIRS  ${LIBINTEL_INCLUDE_DIRS} PARENT_SCOPE)

  file(GLOB Intel_SOURCES "src/*.cpp")
  include_directories(${LIBINTEL_INCLUDE_DIRS})
  include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
  include_directories(${LIBLOGGER_INCLUDE_DIRS})

  add_library           (Intel_Exp SHARED                   ${Intel_SOURCES})
  set_target_properties (Intel_Exp PROPERTIES COMPILE_FLAGS "${CFLAGS}")

  #This emulation backend is based on Linux Sockets

  add_executable (NW_Region       test/NW_Region.cpp)
  add_executable (NW_Test_IF      test/NW_Test_IF.cpp)
  add_executable (NW_Test_IF_end  test/NW_Test_IF_end.cpp)
  add_executable (NW_Test_IF_MAST test/NW_Test_IF_MAST.cpp)

  set_target_properties (NW_Region       PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")
  set_target_properties (NW_Test_IF      PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")
  set_target_properties (NW_Test_IF_end  PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")
  set_target_properties (NW_Test_IF_MAST PROPERTIES COMPILE_FLAGS "${CFLAGS} -std=c++14")

  target_link_libraries (NW_Region       Mast_Core SIT_Reader Intel_Exp pthread dl)
  target_link_libraries (NW_Test_IF      Mast_Core SIT_Reader Intel_Exp pthread dl)
  target_link_libraries (NW_Test_IF_end  Mast_Core SIT_Reader Intel_Exp pthread dl)
  target_link_libraries (NW_Test_IF_MAST Mast_Core SIT_Reader Intel_Exp pthread dl)
endif()

message("================================================================================")
message("")


