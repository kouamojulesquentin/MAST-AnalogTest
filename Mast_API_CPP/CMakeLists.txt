project(Mast_API_CPP)

include_directories(include)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBMAST_CORE_INTERNAL_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBSIT_READER_INCLUDE_DIRS})

set(LIBMAST_CPP_API_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

set (CFLAGS "${CFLAGS} -DCPP_API_EXPORTS")

file(GLOB Mast_SOURCES_CPP_API "src/*")

set (Mast_SOURCES ${Mast_SOURCES_CPP_API})

add_library           (Mast_API_CPP SHARED   ${Mast_SOURCES})
set_target_properties (Mast_API_CPP PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else ()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
endif ()

target_link_libraries(Mast_API_CPP Mast_Core SIT_Reader_Lib Logger ${EXTRA_LIBS})
