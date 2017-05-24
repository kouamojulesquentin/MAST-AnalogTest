project(Mast_Core)

include_directories(include/)
include_directories(internal/)
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(LIBUSB_CFLAGS              "")
set(HIDAPI_CFLAGS              "")
set(LIBFTDISPI_CFLAGS          "")
set(LIBMAST_CORE_INCLUDE_DIRS  "${CMAKE_CURRENT_SOURCE_DIR}/include/"  PARENT_SCOPE)
set(LIBMAST_CORE_INTERNAL_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/internal/" PARENT_SCOPE)



if (WIN32)
  set (CFLAGS "${CFLAGS} -DMAST_CORE_EXPORTS -D__LITTLE_ENDIAN=1 -D__BIG_ENDIAN=2 -D__BYTE_ORDER=1")
endif()

if (CODE_COVERAGE)
  set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS}")
endif()


file(GLOB Mast_Core_SOURCES "src/*")
add_library           (Mast_Core SHARED ${Mast_Core_SOURCES})
set_target_properties (Mast_Core PROPERTIES COMPILE_FLAGS "${CFLAGS}")

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
endif()

message("")
message("================================================================================")
message(STATUS "Mast_Core:  CODE_COVERAGE:              ${CODE_COVERAGE}")
message(STATUS "Mast_Core:  USE_OPEN_OCD:               ${USE_OPEN_OCD}")
message(STATUS "Mast_Core:  USE_LIBFTDISPI:             ${USE_LIBFTDISPI}")
message(STATUS "Mast_Core:  CMAKE_CXX_FLAGS:            ${CMAKE_CXX_FLAGS}")
message("================================================================================")
message("")


target_link_libraries(Mast_Core Logger ${EXTRA_LIBS})
