cmake_minimum_required(VERSION 2.6)
project(MastOpenOCD)

message("")
message("================================================================================")
message(STATUS "MastOpenOCD: LIBMAST_CORE_INCLUDE_DIRS: ${LIBMAST_CORE_INCLUDE_DIRS}")
message(STATUS "MastOpenOCD: LIBOPEN_OCD_INCLUDE_DIRS:  ${LIBOPEN_OCD_INCLUDE_DIRS}")
message(STATUS "MastOpenOCD: CFLAGS:                    ${CFLAGS}")
message("================================================================================")
message("")


include_directories(./include)
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBOPEN_OCD_INCLUDE_DIRS})

set(LIBMASTOPEN_OCD_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(LIBMASTOPEN_OCD_LIBRARIES    Mast_OpenOCD                          PARENT_SCOPE)

file(GLOB Sources src/*.cpp)


add_library           (Mast_OpenOCD SHARED     "${Sources}")
set_target_properties (Mast_OpenOCD PROPERTIES COMPILE_FLAGS "${CFLAGS}" )
target_link_libraries (Mast_OpenOCD ${OPEN_OCD_LIBRARIES})


