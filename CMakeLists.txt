if (WIN32)
	if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
		message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
	endif()
  set (CMAKE_CXX_COMPILER "g++")
  set (CMAKE_C_COMPILER   "gcc")
else()
  set (CMAKE_CXX_COMPILER "g++-4.9")
endif()

cmake_minimum_required(VERSION 3.5)


project(mast)


add_definitions(${WARNINGS})
set (WARNINGS -Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default)
add_definitions(${CFLAGS})
set (CFLAGS ${WARNINGS} -std=c++14)

if (WIN32)
#+  set (CFLAGS ${CFLAGS} -DBUILD_DLL)
  set (CFLAGS ${CFLAGS})
else()
  set (CFLAGS ${CFLAGS} -fPIC)
endif()

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

add_compile_options(${CFLAGS})
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")
#set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CFLAGS}")

#Bring the headers, such as Student.h into the project
include_directories(Lib/include/)
include_directories(Lib/public_include/)
include_directories(Logger/)
include_directories(Logger/g3log/)
include_directories(Logger/Unix)
include_directories(Logger/Windows)

# LOGGER:
file(GLOB Logger_SOURCES 			 "Logger/*.cpp")
file(GLOB Logger_SOURCES_Linux "Logger/Unix/*.cpp")
file(GLOB Logger_SOURCES_Win32 "Logger/Windows/*.cpp")

if (WIN32)
	set (Logger_SOURCES ${Logger_SOURCES} ${Logger_SOURCES_Win32})
else ()
	set (Logger_SOURCES ${Logger_SOURCES} ${Logger_SOURCES_Linux})
endif ()

add_library(Logger SHARED ${Logger_SOURCES})
if (WIN32)
	target_link_libraries(Logger DbgHelp)
else ()
	target_link_libraries(Logger pthread)
endif ()

# Lib:
file(GLOB Lib_SOURCES "Lib/src/*")
#add_library(Lib OBJECT ${Lib_SOURCES})
add_library(Lib SHARED ${Lib_SOURCES})
if (WIN32)
	target_link_libraries(Lib Logger DbgHelp)
else ()
	target_link_libraries(Lib Logger pthread)
endif ()


# LIB Unit tests:
add_definitions(${UT_WARNINGS})
set (UT_WARNINGS ${WARNINGS})

add_definitions(${UT_CFLAGS})
set (UT_CFLAGS ${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)

add_compile_options(${UT_CFLAGS})

#########################################

set (CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cxxtest")
set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/cxxtest/bin/cxxtestgen_cmake.py")
set (CXXTEST_TESTGEN_EXECUTABLE ${CXXTEST_PYTHON_TESTGEN_EXECUTABLE})
set (CXXTEST_USE_PYTHON 1)
find_package(CxxTest)

set (CXXTEST_TESTGEN_INTERPRETER python3)

#if(CXXTEST_FOUND)
set (UT_RootPath  "${CMAKE_CURRENT_SOURCE_DIR}/Lib_UT/")

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${UT_RootPath}/CxxTest_Traits/)
include_directories(${UT_RootPath}/UT_Helpers/)

add_definitions(${Runner_UT_SOURCES})
add_definitions(${MAST_CXX_AUTOMATIC_HEADER_SEARCH})
set (MAST_CXX_AUTOMATIC_HEADER_SEARCH 0)
MESSAGE("MAST_CXX_AUTOMATIC_HEADER_SEARCH : ${MAST_CXX_AUTOMATIC_HEADER_SEARCH}")

if (${MAST_CXX_AUTOMATIC_HEADER_SEARCH})
    file(GLOB Runner_UT_SOURCES "Lib_UT/Utility/*.hpp")
    file(GLOB Runner_UT_TMP_SOURCES "Lib_UT/SystemModel/*.hpp")
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${Runner_UT_TMP_SOURCES})
    file(GLOB Runner_UT_TMP_SOURCES "Lib_UT/UT_Helpers/*.hpp")
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
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_SystemModel.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_SystemModelCheckerVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_PrettyPrinterVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/SystemModel/UT_GmlPrinterVisitor.hpp)
    set (Runner_UT_SOURCES ${Runner_UT_SOURCES} ${UT_RootPath}/UT_Helpers/UT_SystemModelBuilder.hpp)
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

    file(GLOB Lib_UT_SOURCES 			"Lib_UT/Utility/*")
    file(GLOB Lib_UT_TMP_SOURCES  "Lib_UT/SystemModel/*")
    set (Lib_UT_SOURCES ${Lib_UT_SOURCES} ${Lib_UT_TMP_SOURCES})
    file(GLOB Lib_UT_TMP_SOURCES 	"Lib_UT/UT_Helpers/*")
    set (Lib_UT_SOURCES ${Lib_UT_SOURCES} ${Lib_UT_TMP_SOURCES})
    add_library(Lib_UT SHARED ${Lib_UT_SOURCES})

    enable_testing()


if (WIN32)
	target_link_libraries(Lib_UT_Runner Lib_UT Lib Logger DbgHelp )
else ()
	target_link_libraries(Lib_UT_Runner Lib_UT Lib Logger pthread)
endif ()
