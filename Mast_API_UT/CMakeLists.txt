# Mast_Core Unit tests:

project (Mast_API_UT CXX)
message("")
message("================================================================================")

set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

set(TRAITS           "${CMAKE_CURRENT_LIST_DIR}/CxxTest_Traits/CPP_API_Traits.hpp"
                     "${CMAKE_CURRENT_LIST_DIR}/CxxTest_Traits/C_API_Traits.hpp")

set(CPP_API_HEADERS  "${CMAKE_CURRENT_LIST_DIR}/API_CPP/UT_MastEnvironment.hpp"
                     "${CMAKE_CURRENT_LIST_DIR}/API_CPP/UT_ModelBuildDriver.hpp"
                     "${CMAKE_CURRENT_LIST_DIR}/API_CPP/UT_PDL_Adapter_CPP.hpp")

set(CPP_API_SOURCES  "${CMAKE_CURRENT_LIST_DIR}/API_CPP/UT_MastEnvironment.cpp"
                     "${CMAKE_CURRENT_LIST_DIR}/API_CPP/UT_ModelBuildDriver.cpp"
                     "${CMAKE_CURRENT_LIST_DIR}/API_CPP/UT_PDL_Adapter_CPP.cpp")


set(C_API_HEADERS    "${CMAKE_CURRENT_LIST_DIR}/API_C/UT_PDL_Adapter_C.hpp")
set(C_API_SOURCES    "${CMAKE_CURRENT_LIST_DIR}/API_C/UT_PDL_Adapter_C.cpp")


set(GENERATED_RUNNER "${CMAKE_CURRENT_LIST_DIR}/Generated/Runner.cpp")

set(UT_HEADERS ${CPP_API_HEADERS}
               ${C_API_HEADERS})

set(UT_SOURCES ${CPP_API_SOURCES}
               ${C_API_SOURCES}
               ${GENERATED_RUNNER})


# ---------------- Generate Runner.cpp
#
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "Mast_API_UT: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")
include(../Tests_Common/MakeRunner.cmake)
MakeRunner(${CXXTEST_PATH} ${GENERATED_RUNNER} "${EXTRA_RUNNER_DEPENDS}" "${UT_HEADERS}")

message("================================================================================")
message("")

if (WIN32)
  set(PLATFORM_LIBS DbgHelp)
else()
  set(PLATFORM_LIBS pthread)
endif()



set_source_files_properties(${GENERATED_RUNNER} GENERATED)


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable             (Mast_API_UT ${UT_SOURCES} ${UT_HEADERS} ${TRAITS})
target_compile_options     (Mast_API_UT PUBLIC  -fexceptions -std=c++14)
target_compile_definitions (Mast_API_UT PUBLIC  CXXTEST_HAVE_EH CXXTEST_HAVE_STD CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)
target_link_libraries      (Mast_API_UT ${PLATFORM_LIBS} Tests_Common Mast_API_CPP Mast_API_C)
target_include_directories (Mast_API_UT PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/API_CPP
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/API_C
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/CxxTest_Traits
                                                 ${LIBMAST_CPP_API_INTERNAL_DIRS}
                                                 ${LIBMAST_C_API_INTERNAL_DIRS}
                                                 )

install(DIRECTORY UT_TestFiles DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Mast_API)

include(CTest)
add_test(NAME Mast_API_UT COMMAND Mast_API_UT)
