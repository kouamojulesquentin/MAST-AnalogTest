project(mast_lib)

include_directories(include/)
include_directories(public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)

if (WIN32)
  set (CFLAGS "${CFLAGS} -D__LITTLE_ENDIAN=1 -D__BIG_ENDIAN=2 -D__BYTE_ORDER=1")
else()
endif()


file(GLOB Lib_SOURCES "src/*")
add_library						(Lib SHARED ${Lib_SOURCES})
set_target_properties	(Lib PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
	target_link_libraries(Lib Logger DbgHelp)
else ()
	target_link_libraries(Lib Logger pthread)
endif ()
