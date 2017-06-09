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
file(GLOB Mast_UT_HEADER      "./*/UT_*.hpp")
add_custom_command (OUTPUT 		${UT_RootPath}/Generated/Runner.cpp
										DEPENDS 	${UT_RootPath}/Runner.mak ${Mast_UT_HEADER}
										COMMAND 	make -C ${UT_RootPath} -f Runner.mak
										)

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${UT_RootPath}/CxxTest_Traits)
include_directories(${UT_RootPath}/UT_Helpers/)
include_directories(${UT_RootPath}/Yaml/)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBMAST_CORE_INTERNAL_DIRS})
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBMAST_C_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBSIT_READER_INCLUDE_DIRS})
include_directories(${LIBTCLAP_INCLUDE_DIRS})
include_directories(${LIBYAML_INCLUDE_DIRS})


#add_compile_options(${UT_CFLAGS})

file(GLOB Mast_UT_C_API        "API_C/*")
file(GLOB Mast_UT_CPP_API      "API_CPP/*")
file(GLOB Mast_UT_UTILITY      "Utility/*")
file(GLOB Mast_UT_SYSTEM_MODEL "SystemModel/*")
file(GLOB Mast_UT_YAML         "Yaml/*")
file(GLOB Mast_UT_CORE         "Core/*")
file(GLOB Mast_UT_PLUGINS      "Plugins/UT_*")
file(GLOB Mast_UT_HELPERS      "UT_Helpers/*")

#+set(Mast_UT_PLUGINS ${Mast_UT_PLUGINS} "Plugins/PluginsRegistry.hpp" "Plugins/PluginsRegistry.cpp")

if (WIN32)
  set(PLATFORM_LIBS DbgHelp)
else()
  set(PLATFORM_LIBS pthread)
endif()

# ---------------- Helpers DLL
#
add_library           (Mast_UT_Helpers SHARED     "Plugins/PluginsRegistry.cpp")
set_target_properties (Mast_UT_Helpers PROPERTIES COMPILE_FLAGS "${WARNINGS} -std=c++14")
target_link_libraries (Mast_UT_Helpers ${PLATFORM_LIBS})

#+set(EXTRA_LIBS ${EXTRA_LIBS} Mast_UT_Helpers)


set(Mast_UT_GENERATED "Generated/Runner.cpp")

set (Mast_UT_SOURCES ${Mast_UT_C_API}
                     ${Mast_UT_CPP_API}
                     ${Mast_UT_UTILITY}
                     ${Mast_UT_SYSTEM_MODEL}
                     ${Mast_UT_YAML}
                     ${Mast_UT_CORE}
                     ${Mast_UT_PLUGINS}
                     ${Mast_UT_HELPERS}
                     ${Mast_UT_GENERATED})

set_source_files_properties(${Mast_UT_GENERATED} GENERATED)


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable 				(Mast_UT ${Mast_UT_SOURCES})
set_target_properties	(Mast_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )

target_link_libraries(Mast_UT Mast_UT_Helpers Mast_API_CPP Mast_API_C Logger ${PLATFORM_LIBS})

install(DIRECTORY UT_TestFiles DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Mast_Core)

# ---------------- Fake plugins
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_DIR})  # For Windows
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGINS_DIR})  # For Linux

add_library           (Fake_Plugin_1 SHARED     "Plugins/Fake_Plugin_1.cpp")
set_target_properties (Fake_Plugin_1 PROPERTIES COMPILE_FLAGS "${WARNINGS} -std=c++14")
target_link_libraries (Fake_Plugin_1 ${PLATFORM_LIBS} Mast_UT_Helpers)


include(CTest)
add_test(NAME Mast_UT COMMAND Mast_UT)
