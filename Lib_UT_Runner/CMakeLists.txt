# LIB Unit tests Runner:

Project (mast_UT_Runner)

add_definitions(${UT_WARNINGS})
set (UT_WARNINGS ${WARNINGS})

add_definitions(${UT_CFLAGS})
set (UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")

#add_compile_options(${UT_CFLAGS})

#########################################

set (CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest")
set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest/bin/cxxtestgen_cmake.py")
set (CXXTEST_TESTGEN_EXECUTABLE ${CXXTEST_PYTHON_TESTGEN_EXECUTABLE})
set (CXXTEST_USE_PYTHON 1)
find_package(CxxTest)

set (CXXTEST_TESTGEN_INTERPRETER python3)

#if(CXXTEST_FOUND)
set (UT_RootPath  "${CMAKE_CURRENT_SOURCE_DIR}/../Lib_UT/")

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${UT_RootPath}//CxxTest_Traits/)
include_directories(${UT_RootPath}/UT_Helpers/)

add_definitions(${Runner_UT_SOURCES})
add_definitions(${MAST_CXX_AUTOMATIC_HEADER_SEARCH})
set (MAST_CXX_AUTOMATIC_HEADER_SEARCH 0)
MESSAGE("MAST_CXX_AUTOMATIC_HEADER_SEARCH : ${MAST_CXX_AUTOMATIC_HEADER_SEARCH}")

if (${MAST_CXX_AUTOMATIC_HEADER_SEARCH})
    file(GLOB Runner_UT_SOURCES "../Lib_UT/Utility/*.hpp")
    file(GLOB Runner_UT_TMP_SOURCES "../Lib_UT/SystemModel/*.hpp")
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${Runner_UT_TMP_SOURCES})
    file(GLOB Runner_UT_TMP_SOURCES "../Lib_UT/UT_Helpers/*.hpp")
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${Runner_UT_TMP_SOURCES})

else()
    #Manual add in order of execution
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/Utility/UT_Utility.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/Utility/UT_BinaryVector.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/Utility/UT_SVFVector.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_SystemModelNode.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_Register.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_ParentNode.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_NodePathResolver.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_DefaultBinaryPathSelector.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_DefaultOneHotPathSelector.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_SystemModel.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_SystemModelCheckerVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_PrettyPrinterVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_GmlPrinterVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/UT_Helpers/UT_TestModelBuilder.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_PropagatePendingVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_ConfigureVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_FromSutUpdater.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_ToSutVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_DataCycle.hpp )
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_GenericAccessInterfaceProtocol.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_SVF_SimulationProtocol.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_SystemModelManager.hpp)
endif()

    CXXTEST_ADD_TEST(Lib_UT_Runner   	Runner.cpp  	${Runner_UT_SOURCES} )

    set_target_properties(Lib_UT_Runner PROPERTIES
 	 COMPILE_FLAGS "${UT_CFLAGS}"
	  )
    enable_testing()


if (WIN32)
	target_link_libraries(Lib_UT_Runner Lib_UT Lib Logger DbgHelp )
else ()
	target_link_libraries(Lib_UT_Runner Lib_UT Lib Logger pthread)
endif ()


