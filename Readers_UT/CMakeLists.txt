# Readers_UT: for testing SIT & ICL readers

project (Readers_UT CXX)
message("")
message("================================================================================")

set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

set(TRAITS_HEADERS        "${CMAKE_CURRENT_LIST_DIR}/Common/Readers_Common_Enums_Traits.hpp")

set(UT_COMMON_HEADERS     "${CMAKE_CURRENT_LIST_DIR}/Common/UT_AST_PrettyPrinter.hpp"
                          "${CMAKE_CURRENT_LIST_DIR}/Common/UT_AST_IntegerExpr.hpp"
                          "${CMAKE_CURRENT_LIST_DIR}/Common/UT_AST_Checker.hpp")

set(UT_COMMON_SOURCES     "${CMAKE_CURRENT_LIST_DIR}/Common/UT_AST_PrettyPrinter.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/Common/UT_AST_IntegerExpr.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/Common/UT_AST_Checker.cpp")

set(UT_SIT_READER_HEADERS "${CMAKE_CURRENT_LIST_DIR}/SIT/UT_SIT_Printer.hpp"
                          "${CMAKE_CURRENT_LIST_DIR}/SIT/UT_SIT_Reader.hpp")

set(UT_SIT_READER_SOURCES "${CMAKE_CURRENT_LIST_DIR}/SIT/UT_SIT_Printer.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/SIT/UT_SIT_Reader.cpp")

set(UT_ICL_READER_HEADERS "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_BSDL_Reader.hpp"
                          "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_ICL_Reader.hpp"
                          "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_Fragments.hpp"
                          "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_IEEE_P1687.hpp"
                          )

set(UT_ICL_READER_SOURCES "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_BSDL_Reader.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_ICL_Reader.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_Fragments.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/ICL/UT_IEEE_P1687.cpp")

set(GENERATED_RUNNER      "${CMAKE_CURRENT_LIST_DIR}/Generated/Runner.cpp")

set(UT_HEADERS  ${UT_COMMON_HEADERS}
                ${UT_ICL_READER_HEADERS}
                ${UT_SIT_READER_HEADERS})

set(UT_SOURCES  ${UT_COMMON_SOURCES}
                ${UT_ICL_READER_SOURCES}
                ${UT_SIT_READER_SOURCES}
                ${GENERATED_RUNNER})

# ---------------- Generate Runner.cpp
#
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "Readers_UT: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")
include(../Tests_Common/MakeRunner.cmake)
MakeRunner(${CXXTEST_PATH} ${GENERATED_RUNNER} "${EXTRA_RUNNER_DEPENDS}" "${UT_HEADERS}")

message("================================================================================")
message("")

if (WIN32)
  set(PLATFORM_LIBS DbgHelp)
else()
  set(PLATFORM_LIBS pthread)
endif()

# ---------------- Runner application
#
add_executable             (Readers_UT ${UT_SOURCES} ${UT_HEADERS} ${TRAITS_HEADERS})
target_compile_options     (Readers_UT PUBLIC  -fexceptions -std=c++14)
target_compile_definitions (Readers_UT PUBLIC  CXXTEST_HAVE_EH CXXTEST_HAVE_STD CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)
target_link_libraries      (Readers_UT ${PLATFORM_LIBS} Tests_Common ICL_Reader SIT_Reader)


install(DIRECTORY UT_TestFiles DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Readers)

include(CTest)
add_test(NAME Readers_UT COMMAND Readers_UT)
