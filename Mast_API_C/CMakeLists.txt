project(Mast_API_C)


include_directories(./include)
include_directories(./internal)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBMAST_CORE_INTERNAL_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(LIBMAST_C_API_INCLUDE_DIRS   "${CMAKE_CURRENT_SOURCE_DIR}/include"   PARENT_SCOPE)
set(CFLAGS "${CFLAGS} -DC_API_EXPORTS")

if (CODE_COVERAGE)
  set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS} ")
endif()


file(GLOB Mast_SOURCES_C_API "src/*")

set (Mast_SOURCES ${Mast_SOURCES_C_API})

add_library           (Mast_API_C SHARED   ${Mast_SOURCES})
set_target_properties (Mast_API_C PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else ()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
endif ()

message("")
message("================================================================================")
message(STATUS "Mast_API_C:   CODE_COVERAGE     ${CODE_COVERAGE}")
message(STATUS "Mast_API_C:   CFLAGS:           ${CFLAGS}")
message(STATUS "Mast_API_C:   EXTRA_LIBS:       ${EXTRA_LIBS}")
message(STATUS "Mast_API_C:   CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
message("================================================================================")
message("")

target_link_libraries(Mast_API_C SIT_Reader Mast_Core Logger ${EXTRA_LIBS})
