# External_Libs

project (External_Libs_UT)
message("")
message("================================================================================")

set(UT_WARNINGS ${WARNINGS})
set(UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")

set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${LIBTCLAP_INCLUDE_DIRS})
include_directories(${LIBYAML_INCLUDE_DIRS})

set(UT_GENERATED_RUNNER   "Generated/Runner.cpp")

file(GLOB UT_YAML_HEADERS  "UT_Yaml/UT_*.hpp")
file(GLOB UT_YAML_SOURCES  "UT_Yaml/UT_*.cpp")

file(GLOB UT_TCLAP_HEADERS  "UT_TClap/UT_*.hpp")
file(GLOB UT_TCLAP_SOURCES  "UT_TClap/UT_*.cpp")

set(UT_HEADERS ${UT_HEADERS}
               ${UT_TCLAP_HEADERS}
               ${UT_YAML_HEADERS})

set(UT_SOURCES ${UT_GENERATED_RUNNER}
               ${UT_TCLAP_SOURCES}
               ${UT_YAML_SOURCES})

set(EXTRA_LIBS ${YAML_LIBRARIES})

message(STATUS "External_Libs_UT: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")
message(STATUS "External_Libs_UT: LIBTCLAP_INCLUDE_DIRS       ${LIBTCLAP_INCLUDE_DIRS}")
message(STATUS "External_Libs_UT: LIBYAML_INCLUDE_DIRS        ${LIBYAML_INCLUDE_DIRS}")


# ---------------- Runner
#
set(RUNNER_PATH          ${UT_RootPath}/Generated/Runner.cpp)
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "External_Libs_UT: UT_HEADERS:                 ${UT_HEADERS}")
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
add_executable        (Externals_UT ${UT_SOURCES})
set_target_properties (Externals_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )
target_link_libraries (Externals_UT ${PLATFORM_LIBS} ${EXTRA_LIBS})

include(CTest)
add_test(NAME Externals_UT COMMAND Externals_UT)
