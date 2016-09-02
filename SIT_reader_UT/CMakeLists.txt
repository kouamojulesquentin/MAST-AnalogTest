# SIT_Reader:

project (SIT_reader_UT)

add_definitions(${UT_WARNINGS})
set (UT_WARNINGS ${WARNINGS})

add_definitions(${UT_CFLAGS})
set (UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")


#if(CXXTEST_FOUND)
set (UT_RootPath  					"${CMAKE_CURRENT_SOURCE_DIR}/")
set (CXXTEST_INCLUDE_DIR 		"${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest")

ADD_CUSTOM_TARGET(SIT_Runner echo "Building SIT_Reader_UT runner")
file(GLOB SIT_Reader_UT_HEADER      "./*/UT_*.hpp")
add_custom_command (OUTPUT 		${UT_RootPath}/Generated/Runner.cpp
										DEPENDS 	${UT_RootPath}/Runner.mak ${SIT_Reader_UT_HEADER}
										COMMAND 	make -C ${UT_RootPath} -f Runner.mak
										)

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${UT_RootPath}/../Mast_UT/CxxTest_Traits)
include_directories(${UT_RootPath}/UT_Helpers/)
include_directories(../Mast_Core/include/)
include_directories(../Mast_Core/public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)
include_directories(../SIT_reader/)
if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()


#add_compile_options(${UT_CFLAGS})

file(GLOB SIT_Reader_UT_C_API        "Use_Cases/*")
file(GLOB SIT_Reader_UT_HELPERS      "UT_Helpers/*")
set(SIT_Reader_UT_GENERATED 		"Generated/Runner.cpp")

set (SIT_Reader_UT_SOURCES ${SIT_Reader_UT_C_API} ${SIT_Reader_UT_UTILITY} ${SIT_Reader_UT_SYSTEM_MODEL} ${SIT_Reader_UT_HELPERS} ${SIT_Reader_UT_GENERATED})



set_source_files_properties(${SIT_Reader_UT_GENERATED} GENERATED)

add_executable (SIT_Reader_UT ${SIT_Reader_UT_SOURCES})
set_target_properties(SIT_Reader_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )

if (WIN32)
	target_link_libraries(SIT_Reader_UT  Mast_Core Logger  SIT_Reader_Lib DbgHelp)
else ()
	target_link_libraries(SIT_Reader_UT  Mast_Core Logger  SIT_Reader_Lib pthread)
endif ()

