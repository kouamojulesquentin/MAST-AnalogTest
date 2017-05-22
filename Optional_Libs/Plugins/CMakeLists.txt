cmake_minimum_required(VERSION 3.5)
project(Plugins)


set (WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set (CFLAGS "${WARNINGS} -std=c++14 -g -DPLUGINS_EXPORTS")

include_directories(include)
include_directories(internal)
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})

set(LIBPLUGINS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

file(GLOB Sources "src/*.cpp")
if (WIN32)
  file(GLOB Platform_Sources "Windows/*.cpp")
elseif(UNIX)
  file(GLOB Platform_Sources "Linux/*.cpp")
else()
  message(FATAL_ERROR "Plugins: There is no plugin support for target platform: " ${CMAKE_SYSTEM_NAME})
endif()

set(Sources ${Sources} ${Platform_Sources})

message("")
message("================================================================================")
message(STATUS "Plugins: CMAKE_CXX_COMPILER_ID:   ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Plugins: CMAKE_LINK_LIBRARY_FLAG: ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "Plugins: CFLAGS:                  ${CFLAGS}")
message(STATUS "Plugins: Sources:                 ${Sources}")
message("================================================================================")
message("")

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
endif()

add_library           (Plugins SHARED     "${Sources}")
set_target_properties (Plugins PROPERTIES COMPILE_FLAGS "${CFLAGS}" )
target_link_libraries (Plugins Mast_Core Logger ${EXTRA_LIBS})


