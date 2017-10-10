# Readers_UT: for testing SIT & ICL readers

project (Readers_UT CXX)
message("")
message("================================================================================")

set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

message(STATUS "Readers_UT: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")

file(GLOB UT_SIT_READER_HEADERS "SIT/UT_*.hpp")
file(GLOB UT_SIT_READER_FILES   "SIT/*")
file(GLOB UT_ICL_READER_HEADERS "ICL/UT_*.hpp")
file(GLOB UT_ICL_READER_FILES   "ICL/*")

set(GENERATED_RUNNER            "Generated/Runner.cpp")

set(UT_HEADERS  ${UT_SIT_READER_HEADERS}
                ${UT_ICL_READER_HEADERS}
   )

set(UT_SOURCES  ${UT_SIT_READER_FILES}
                ${UT_ICL_READER_FILES}
                ${GENERATED_RUNNER}
   )

# ---------------- Generate Runner.cpp
#
set(RUNNER_PATH          ${UT_RootPath}/${GENERATED_RUNNER})
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "Readers_UT: UT_HEADERS:                 ${UT_HEADERS}")
include(../Tests_Common/MakeRunner.cmake)
MakeRunner(${CXXTEST_PATH} ${RUNNER_PATH} "${EXTRA_RUNNER_DEPENDS}" "${UT_HEADERS}")

message("================================================================================")
message("")

if (WIN32)
  set(PLATFORM_LIBS DbgHelp)
else()
  set(PLATFORM_LIBS pthread)
endif()

# ---------------- Runner application
#
add_executable             (Readers_UT ${UT_SOURCES})
target_compile_options     (Readers_UT PUBLIC  -fexceptions -std=c++14)
target_compile_definitions (Readers_UT PUBLIC  CXXTEST_HAVE_EH CXXTEST_HAVE_STD CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)
target_link_libraries      (Readers_UT ${PLATFORM_LIBS} Tests_Common ICL_Reader SIT_Reader)


install(DIRECTORY UT_TestFiles DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Readers)

include(CTest)
add_test(NAME Readers_UT COMMAND Readers_UT)
