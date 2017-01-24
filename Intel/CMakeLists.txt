project(Intel_Experimental)


if (WIN32)
  message("\n>>>>>>>>>>>>>>> Lib 'Intel' is not build on Windows !!!\n")
  set (CFLAGS "${CFLAGS} -D__LITTLE_ENDIAN=1 -D__BIG_ENDIAN=2 -D__BYTE_ORDER=1")
elseif(${USE_OPEN_OCD})
  set (CFLAGS "${CFLAGS}")

  file(GLOB Intel_SOURCES "src/*")
  include_directories(./src/)
  include_directories(../Mast_Core/include/)
  include_directories(../Logger/)
  include_directories(../Logger/g3log/)

  add_library (Intel_Exp STATIC ${Intel_SOURCES})
  set_target_properties (Intel_Exp PROPERTIES COMPILE_FLAGS "${CFLAGS}")

  #This emulation backend is based on Linux Sockets

  add_executable (NW_Region NW_Region.cpp)
  add_executable (NW_Test_IF NW_Test_IF.cpp)
  add_executable (NW_Test_IF_end NW_Test_IF_end.cpp)
  set_target_properties (NW_Region PROPERTIES COMPILE_FLAGS "${CFLAGS}")
  set_target_properties (NW_Test_IF PROPERTIES COMPILE_FLAGS "${CFLAGS}")
  set_target_properties (NW_Test_IF_end PROPERTIES COMPILE_FLAGS "${CFLAGS}")

  add_executable (NW_Test_IF_MAST NW_Test_IF_MAST.cpp)

  target_link_libraries (NW_Region Intel_Exp)
  target_link_libraries (NW_Test_IF Intel_Exp)
  target_link_libraries (NW_Test_IF_end Intel_Exp)

  target_link_libraries (NW_Test_IF_MAST Intel_Exp)
endif()

