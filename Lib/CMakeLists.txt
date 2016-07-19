project(mast_lib)

include_directories(include/)
include_directories(public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)

file(GLOB Lib_SOURCES "src/*")
add_library						(Lib SHARED ${Lib_SOURCES})
set_target_properties	(Lib PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
	target_link_libraries(Lib Logger DbgHelp)
else ()
	target_link_libraries(Lib Logger pthread)
endif ()
