# Tests_Common: Regroup parts that are potentially used by multiple unit tests projects
# It depends on MAST core to provide facilities to build a model of SUT

project (Tests_Common CXX)

set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

message("")
message("================================================================================")
message(STATUS "Tests_Common: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")


set(TESTS_COMMON_LIBRARIES "Tests_Common" PARENT_SCOPE)

if (WIN32)
  set(PLATFORM_LIBS DbgHelp)
else()
  set(PLATFORM_LIBS pthread)
endif()

# ---------------- Tests Commons Library
#
add_library           (Tests_Common  SHARED TestUtilities.cpp
                                            TestModelBuilder.cpp
                      )
target_compile_options      (Tests_Common PUBLIC  -fexceptions -std=c++14)
target_compile_definitions  (Tests_Common PUBLIC  CXXTEST_HAVE_EH CXXTEST_HAVE_STD CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)
target_include_directories  (Tests_Common PUBLIC  ${CXXTEST_INCLUDE_DIR}
                                                  ${CMAKE_CURRENT_SOURCE_DIR}
                                                  ${CMAKE_CURRENT_SOURCE_DIR}/CxxTest_Traits
                          )

target_link_libraries (Tests_Common  ${PLATFORM_LIBS} Mast_Core)
#+set_target_properties (Tests_Common  PROPERTIES       COMPILE_FLAGS "${UT_CFLAGS}" )


# ---------------- Build Runner.cpp
#
set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(UT_HEADERS           ${UT_RootPath}/UT_TestModelBuilder.hpp)
set(UT_SOURCES           UT_TestModelBuilder.cpp)
set(GENERATED_RUNNER     "Generated/Runner.cpp")
set(RUNNER_PATH          ${UT_RootPath}/${GENERATED_RUNNER})
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "Tests_Common: UT_RootPath:                ${UT_RootPath}")
message(STATUS "Tests_Common: UT_HEADERS:                 ${UT_HEADERS}")
include(MakeRunner.cmake)
MakeRunner(${CXXTEST_PATH} ${RUNNER_PATH} "${EXTRA_RUNNER_DEPENDS}" "${UT_HEADERS}")

message("================================================================================")
message("")


# ---------------- Runner application (to check provided common utilities)
#
add_executable        (Tests_Common_UT ${UT_SOURCES} ${RUNNER_PATH})
target_link_libraries (Tests_Common_UT ${PLATFORM_LIBS} Tests_Common Logger)

include(CTest)
add_test(NAME Tests_Common_UT COMMAND Tests_Common_UT)
