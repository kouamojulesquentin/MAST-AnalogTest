cmake_minimum_required(VERSION 2.6)
project(Readers)

# ---------------- ICL_Reader
#
add_subdirectory(ICL_Reader)
set(LIBICL_READER_INCLUDE_DIRS ${LIBICL_READER_INCLUDE_DIRS} PARENT_SCOPE)
message("")
message("================================================================================")
message(STATUS "Readers: LIBICL_READER_INCLUDE_DIRS:    ${LIBICL_READER_INCLUDE_DIRS}")
message("================================================================================")
message("")



#+add_subdirectory(ICL_Reader_UT)
