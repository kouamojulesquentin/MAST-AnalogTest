# Readers_UT:

project (Readers_UT)
message("")
message("================================================================================")

set (UT_WARNINGS ${WARNINGS})
set (UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")

set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${UT_RootPath}/../Mast_UT/CxxTest_Traits)
include_directories(${UT_RootPath}/UT_Helpers)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBSIT_READER_INCLUDE_DIRS})
include_directories(${LIBICL_READER_INCLUDE_DIRS})


set(EXTRA_LIBS Logger)
message(STATUS "Readers_UT: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")
message(STATUS "Readers_UT: LIBSIT_READER_INCLUDE_DIRS  ${LIBSIT_READER_INCLUDE_DIRS}")
message(STATUS "Readers_UT: LIBICL_READER_INCLUDE_DIRS  ${LIBICL_READER_INCLUDE_DIRS}")

file(GLOB UT_SIT_READER_HEADERS "SIT/UT_*.hpp")
file(GLOB UT_SIT_READER_FILES   "SIT/*")
file(GLOB UT_ICL_READER_HEADERS "ICL/UT_*.hpp")
file(GLOB UT_ICL_READER_FILES   "ICL/*")
file(GLOB UT_HELPERS_FILES      "UT_Helpers/*")

set(GENERATED_RUNNER            "Generated/Runner.cpp")

set(UT_HEADERS  ${UT_SIT_READER_HEADERS}
                ${UT_ICL_READER_HEADERS}
   )

set(UT_SOURCES  ${UT_SIT_READER_FILES}
                ${UT_ICL_READER_FILES}
                ${UT_HELPERS_FILES}
                ${GENERATED_RUNNER}
   )

set(EXTRA_LIBS ${EXTRA_LIBS} ICL_Reader SIT_Reader_Lib)

# ---------------- Runner
#
set(RUNNER_PATH          ${UT_RootPath}/${GENERATED_RUNNER})
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "Readers_UT: UT_HEADERS:                             ${UT_HEADERS}")
include(MakeRunner.cmake)
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
add_executable        (Readers_UT ${UT_SOURCES})
set_target_properties (Readers_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )
target_link_libraries (Readers_UT ${PLATFORM_LIBS} ${EXTRA_LIBS})


install(DIRECTORY UT_TestFiles DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Readers)

include(CTest)
add_test(NAME Readers_UT COMMAND Readers_UT)
