cmake_minimum_required(VERSION 3.5)

#+add_subdirectory(openocd-builder)

set(LIBOPEN_OCD_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
                             "${CMAKE_CURRENT_SOURCE_DIR}/include/src"
                             "${CMAKE_CURRENT_SOURCE_DIR}/include/jimtcl"
                             PARENT_SCOPE)

set(OPEN_OCD_LIBRARIES       "${CMAKE_CURRENT_SOURCE_DIR}/lib/libopenocd.so" PARENT_SCOPE)



