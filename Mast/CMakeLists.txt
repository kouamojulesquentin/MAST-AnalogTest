project(mast)

include_directories(CPP_API/)
include_directories(C_API/)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBSIT_READER_INCLUDE_DIRS})

set(LIBMAST_CPP_API_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/CPP_API" PARENT_SCOPE)
set(LIBMAST_C_API_INCLUDE_DIRS   "${CMAKE_CURRENT_SOURCE_DIR}/C_API"   PARENT_SCOPE)

set (CFLAGS "${CFLAGS}")

file(GLOB Mast_SOURCES_CPP_API "CPP_API/*")
file(GLOB Mast_SOURCES_C_API   "C_API/*")

set (Mast_SOURCES ${Mast_SOURCES_CPP_API} ${Mast_SOURCES_C_API})

add_library						(Mast SHARED 	${Mast_SOURCES})
set_target_properties	(Mast PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else ()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
endif ()

if (${USE_OPEN_OCD})
  set(EXTRA_LIBS ${EXTRA_LIBS} ${LIBMASTOPEN_OCD_LIBRARIES} ${LIBUSB_CFLAGS})
endif()

target_link_libraries(Mast SIT_Reader_Lib Mast_Core Logger ${EXTRA_LIBS})
