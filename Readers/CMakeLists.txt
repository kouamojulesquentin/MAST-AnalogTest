cmake_minimum_required(VERSION 2.6)
project(Readers)

# ---------------- SIT_Reader
#
add_subdirectory(SIT_Reader)
set(LIBSIT_READER_INCLUDE_DIRS ${LIBSIT_READER_INCLUDE_DIRS} PARENT_SCOPE)
message("")
message("================================================================================")
message(STATUS "Readers: LIBSIT_READER_INCLUDE_DIRS:    ${LIBSIT_READER_INCLUDE_DIRS}")
message("================================================================================")
message("")

# ---------------- ICL_Reader
#
add_subdirectory(ICL_Reader)
set(LIBICL_READER_INCLUDE_DIRS ${LIBICL_READER_INCLUDE_DIRS} PARENT_SCOPE)
message("")
message("================================================================================")
message(STATUS "Readers: LIBICL_READER_INCLUDE_DIRS:    ${LIBICL_READER_INCLUDE_DIRS}")
message("================================================================================")
message("")

