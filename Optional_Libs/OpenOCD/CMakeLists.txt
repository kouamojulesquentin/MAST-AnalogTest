cmake_minimum_required(VERSION 2.6)
project(MastOpenOCD)

message("")
message("================================================================================")
message(STATUS "MastOpenOCD: LIBOPEN_OCD_INCLUDE_DIRS:  ${LIBOPEN_OCD_INCLUDE_DIRS}")
message(STATUS "MastOpenOCD: CFLAGS:                    ${CFLAGS}")
message("================================================================================")
message("")


set(LIBMASTOPEN_OCD_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(LIBMASTOPEN_OCD_LIBRARIES    Mast_OpenOCD                          PARENT_SCOPE)

file(GLOB Sources src/*.cpp)

set(CPP_FLAGS "-Wall -Wextra -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs") # not -Wpedantic on purpose (because of included header)
set(CPP_FLAGS "${CPP_FLAGS} -Wno-variadic-macros") # GCC: ISO C++ does not permit named variadic macros [-Wvariadic-macros]
set(CPP_FLAGS "${CPP_FLAGS} -std=c++14")


add_library                (Mast_OpenOCD SHARED     "${Sources}")
set_target_properties      (Mast_OpenOCD PROPERTIES COMPILE_FLAGS ${CPP_FLAGS})
target_include_directories (Mast_OpenOCD PUBLIC     "${CMAKE_CURRENT_SOURCE_DIR}/include"
                                         PRIVATE    "${LIBOPEN_OCD_INCLUDE_DIRS}"
                                                    "${LIBMAST_CORE_INCLUDE_DIRS}") # Necessary because we do not link to Mast_Core
target_link_libraries      (Mast_OpenOCD ${OPEN_OCD_LIBRARIES} Logger)


