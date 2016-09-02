# Mast_Core Unit tests:

project (Mast_UT)

add_definitions(${UT_WARNINGS})
set (UT_WARNINGS ${WARNINGS})

add_definitions(${UT_CFLAGS})
set (UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")


#if(CXXTEST_FOUND)
set (UT_RootPath  					"${CMAKE_CURRENT_SOURCE_DIR}/")
set (CXXTEST_INCLUDE_DIR 		"${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest")

ADD_CUSTOM_TARGET(Runner echo "Building Mast_UT runner")
file(GLOB Lib_UT_HEADER      "./*/UT_*.hpp")
add_custom_command (OUTPUT 		${UT_RootPath}/Generated/Runner.cpp
										DEPENDS 	${UT_RootPath}/Runner.mak ${Lib_UT_HEADER}
										COMMAND 	make -C ${UT_RootPath} -f Runner.mak
										)

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${UT_RootPath}/CxxTest_Traits)
include_directories(${UT_RootPath}/UT_Helpers/)
include_directories(../Mast_Core/include/)
include_directories(../Mast_Core/public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)


#add_compile_options(${UT_CFLAGS})

file(GLOB Lib_UT_C_API        "C_API/*")
file(GLOB Lib_UT_UTILITY      "Utility/*")
file(GLOB Lib_UT_SYSTEM_MODEL "SystemModel/*")
file(GLOB Lib_UT_HELPERS      "UT_Helpers/*")
set(Lib_UT_GENERATED 		"Generated/Runner.cpp")

set (Lib_UT_SOURCES ${Lib_UT_C_API} ${Lib_UT_UTILITY} ${Lib_UT_SYSTEM_MODEL} ${Lib_UT_HELPERS} ${Lib_UT_GENERATED})



set_source_files_properties(${Lib_UT_GENERATED} GENERATED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable 				(Mast_UT ${Lib_UT_SOURCES})
set_target_properties	(Mast_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )

if (WIN32)
	target_link_libraries(Mast_UT Mast_Core Logger DbgHelp )
else ()
	target_link_libraries(Mast_UT Mast_Core Logger pthread)
endif ()

