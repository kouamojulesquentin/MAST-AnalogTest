# LIB Unit tests:

project (mast_UT)

add_definitions(${UT_WARNINGS})
set (UT_WARNINGS ${WARNINGS})

add_definitions(${UT_CFLAGS})
set (UT_CFLAGS ${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)

add_compile_options(${UT_CFLAGS})

    file(GLOB Lib_UT_SOURCES 	   "Utility/*")
    file(GLOB Lib_UT_TMP_SOURCES  "SystemModel/*")
    set (Lib_UT_SOURCES ${Lib_UT_SOURCES} ${Lib_UT_TMP_SOURCES})
    file(GLOB Lib_UT_TMP_SOURCES 	"UT_Helpers/*")
    set (Lib_UT_SOURCES ${Lib_UT_SOURCES} ${Lib_UT_TMP_SOURCES})
    add_library(Lib_UT SHARED ${Lib_UT_SOURCES})

#########################################

set (CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest")
set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest/bin/cxxtestgen_cmake.py")
set (CXXTEST_TESTGEN_EXECUTABLE ${CXXTEST_PYTHON_TESTGEN_EXECUTABLE})
set (CXXTEST_USE_PYTHON 1)
find_package(CxxTest)

set (CXXTEST_TESTGEN_INTERPRETER python3)

#if(CXXTEST_FOUND)
set (UT_RootPath  "${CMAKE_CURRENT_SOURCE_DIR}/")

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${UT_RootPath}//CxxTest_Traits/)
include_directories(${UT_RootPath}/UT_Helpers/)
