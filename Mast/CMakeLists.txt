project(mast)

include_directories(CPP_API/)
include_directories(C_API/)
include_directories(../Mast_Core/include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)
include_directories(../SIT_reader/)

set (CFLAGS "${CFLAGS}")

file(GLOB Mast_SOURCES_CPP_API "CPP_API/*")
file(GLOB Mast_SOURCES_C_API   "C_API/*")

set (Mast_SOURCES ${Mast_SOURCES_CPP_API} ${Mast_SOURCES_C_API})

add_library						(Mast SHARED 	${Mast_SOURCES})
set_target_properties	(Mast PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
	target_link_libraries(Mast SIT_Reader_Lib Mast_Core Logger DbgHelp)
else ()
	target_link_libraries(Mast SIT_reader_Lib Mast_Core Logger pthread openocd ${LIBUSB_CFLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/../openocd/openocd/jimtcl/libjim.a dl)
endif ()
