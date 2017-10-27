# Mast_Core Unit tests:

project (Mast_UT)

add_definitions(${UT_WARNINGS})
set (UT_WARNINGS ${WARNINGS})

add_definitions(${UT_CFLAGS})
#+set (UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")


#if(CXXTEST_FOUND)
set (UT_RootPath  					"${CMAKE_CURRENT_SOURCE_DIR}/")
set (CXXTEST_INCLUDE_DIR 		"${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest")

ADD_CUSTOM_TARGET(Runner echo "Building Mast_UT runner")
file(GLOB Mast_UT_HEADER      "./*/UT_*.hpp")
add_custom_command (OUTPUT 		${UT_RootPath}/Generated/Runner.cpp
										DEPENDS 	${UT_RootPath}/Runner.mak ${Mast_UT_HEADER}
										COMMAND 	make -C ${UT_RootPath} -f Runner.mak
										)


set(Mast_UT_HELPERS      UT_Helpers/Fake_SVF_Simulator.cpp
                         UT_Helpers/Fake_SVF_Simulator.hpp
                         UT_Helpers/Pusher.hpp
                         UT_Helpers/SpiedProtocolsCommands.hpp
                         UT_Helpers/Spy_AccessInterfaceProtocols.cpp
                         UT_Helpers/Spy_AccessInterfaceProtocols.hpp
                         UT_Helpers/Spy_I2C_Protocol.cpp
                         UT_Helpers/Spy_I2C_Protocol.hpp
                         UT_Helpers/Spy_STIL_Protocol.cpp
                         UT_Helpers/Spy_STIL_Protocol.hpp
                         UT_Helpers/Spy_SVF_Protocol.cpp
                         UT_Helpers/Spy_SVF_Protocol.hpp
                         UT_Helpers/StaticTestSpy.cpp
                         UT_Helpers/StaticTestSpy.hpp
                         UT_Helpers/TestStrategies.hpp)

set(Mast_UT_CPP_API      API_CPP/UT_MastEnvironment.cpp
                         API_CPP/UT_MastEnvironment.hpp
                         API_CPP/UT_ModelBuildDriver.cpp
                         API_CPP/UT_ModelBuildDriver.hpp
                         API_CPP/UT_PDL_Adapter_CPP.cpp
                         API_CPP/UT_PDL_Adapter_CPP.hpp)

set(Mast_UT_C_API        API_C/UT_PDL_Adapter_C.cpp
                         API_C/UT_PDL_Adapter_C.hpp)

set(Mast_UT_CORE         Core/UT_PDL_AlgorithmsRepository.cpp
                         Core/UT_PDL_AlgorithmsRepository.hpp)

set(Mast_UT_PLUGINS      Plugins/UT_Plugins.cpp
                         Plugins/UT_Plugins.hpp)

set(Mast_UT_YAML         Yaml/UT_YamlFile.cpp
                         Yaml/UT_YamlFile.hpp)

set(Mast_UT_SYSTEM_MODEL SystemModel/UT_AccessInterfaceProtocolFactory.cpp
                         SystemModel/UT_AccessInterfaceProtocolFactory.hpp
                         SystemModel/UT_ConfigureAlgorithmFactory.cpp
                         SystemModel/UT_ConfigureAlgorithmFactory.hpp
                         SystemModel/UT_ConfigureVisitor.cpp
                         SystemModel/UT_ConfigureVisitor.hpp
                         SystemModel/UT_DataCycle.cpp
                         SystemModel/UT_DataCycle.hpp
                         SystemModel/UT_DefaultBinaryPathSelector.cpp
                         SystemModel/UT_DefaultBinaryPathSelector.hpp
                         SystemModel/UT_DefaultNHotPathSelector.cpp
                         SystemModel/UT_DefaultNHotPathSelector.hpp
                         SystemModel/UT_DefaultOneHotPathSelector.cpp
                         SystemModel/UT_DefaultOneHotPathSelector.hpp
                         SystemModel/UT_DefaultTableBasedPathSelector.cpp
                         SystemModel/UT_DefaultTableBasedPathSelector.hpp
                         SystemModel/UT_FromSutUpdater.cpp
                         SystemModel/UT_FromSutUpdater.hpp
                         SystemModel/UT_GenericAccessInterfaceProtocol.cpp
                         SystemModel/UT_GenericAccessInterfaceProtocol.hpp
                         SystemModel/UT_GmlPrinter.cpp
                         SystemModel/UT_GmlPrinter.hpp
                         SystemModel/UT_I2C_EmulationProtocol.cpp
                         SystemModel/UT_I2C_EmulationProtocol.hpp
                         SystemModel/UT_NamesChecker.cpp
                         SystemModel/UT_NamesChecker.hpp
                         SystemModel/UT_NodePathResolver.cpp
                         SystemModel/UT_NodePathResolver.hpp
                         SystemModel/UT_ParentNode.cpp
                         SystemModel/UT_ParentNode.hpp
                         SystemModel/UT_PathSelectorFactory.cpp
                         SystemModel/UT_PathSelectorFactory.hpp
                         SystemModel/UT_PrettyPrinter.cpp
                         SystemModel/UT_PrettyPrinter.hpp
                         SystemModel/UT_PropagatePendingVisitor.cpp
                         SystemModel/UT_PropagatePendingVisitor.hpp
                         SystemModel/UT_Register.cpp
                         SystemModel/UT_Register.hpp
                         SystemModel/UT_Remote_Protocol_Proxy.cpp
                         SystemModel/UT_Remote_Protocol_Proxy.hpp
                         SystemModel/UT_STIL_EmulationProtocol.cpp
                         SystemModel/UT_STIL_EmulationProtocol.hpp
                         SystemModel/UT_SVF_SimulationProtocol.cpp
                         SystemModel/UT_SVF_SimulationProtocol.hpp
                         SystemModel/UT_SystemModel.cpp
                         SystemModel/UT_SystemModel.hpp
                         SystemModel/UT_SystemModelBuilder.cpp
                         SystemModel/UT_SystemModelBuilder.hpp
                         SystemModel/UT_SystemModelChecker.cpp
                         SystemModel/UT_SystemModelChecker.hpp
                         SystemModel/UT_SystemModelFactory.cpp
                         SystemModel/UT_SystemModelFactory.hpp
                         SystemModel/UT_SystemModelManager.cpp
                         SystemModel/UT_SystemModelManager.hpp
                         SystemModel/UT_SystemModelNode.cpp
                         SystemModel/UT_SystemModelNode.hpp
                         SystemModel/UT_ToSutVisitor.cpp
                         SystemModel/UT_ToSutVisitor.hpp
                         SystemModel/UT_VirtualRegister.cpp
                         SystemModel/UT_VirtualRegister.hpp)

set(Mast_UT_UTILITY      Utility/UT_BinaryVector.cpp
                         Utility/UT_BinaryVector.hpp
                         Utility/UT_FileSystem.cpp
                         Utility/UT_FileSystem.hpp
                         Utility/UT_IndexedRange.cpp
                         Utility/UT_IndexedRange.hpp
                         Utility/UT_MastConfiguration.cpp
                         Utility/UT_MastConfiguration.hpp
                         Utility/UT_MTQueue.cpp
                         Utility/UT_MTQueue.hpp
                         Utility/UT_SVFVector.cpp
                         Utility/UT_SVFVector.hpp
                         Utility/UT_Utility.cpp
                         Utility/UT_Utility.hpp
                         Utility/UT_EnumsUtility.cpp
                         Utility/UT_EnumsUtility.hpp
                         )

if (WIN32)
  set(PLATFORM_LIBS DbgHelp)
else()
  set(PLATFORM_LIBS pthread)
endif()

# ---------------- Helpers DLL
#
add_library           (Mast_UT_Helpers SHARED     "Plugins/PluginsRegistry.cpp")
set_target_properties (Mast_UT_Helpers PROPERTIES COMPILE_FLAGS "${WARNINGS} -std=c++14")
target_link_libraries (Mast_UT_Helpers Tests_Common ${PLATFORM_LIBS})

set(Mast_UT_GENERATED "Generated/Runner.cpp")

set(Mast_UT_SOURCES ${Mast_UT_C_API}
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
add_executable             (Mast_UT ${Mast_UT_SOURCES})
set_target_properties      (Mast_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )
target_compile_options     (Mast_UT PRIVATE -fexceptions -std=c++14)
target_compile_definitions (Mast_UT PRIVATE CXXTEST_HAVE_EH
                                            CXXTEST_HAVE_STD
                                            CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)
target_include_directories (Mast_UT PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../cxxtest
                                             ${UT_RootPath}/CxxTest_Traits
                                             ${UT_RootPath}/UT_Helpers/
                                             ${UT_RootPath}/Yaml/
                                             ${LIBMAST_CORE_INCLUDE_DIRS}
                                             ${LIBMAST_CORE_INTERNAL_DIRS}
                                             ${LIBMAST_CPP_API_INCLUDE_DIRS}
                                             ${LIBMAST_CPP_API_INTERNAL_DIRS}
                                             ${LIBMAST_C_API_INCLUDE_DIRS}
                                             ${LIBLOGGER_INCLUDE_DIRS}
                                             ${LIBTCLAP_INCLUDE_DIRS}
                                             ${LIBYAML_INCLUDE_DIRS})

target_link_libraries(Mast_UT Mast_UT_Helpers Tests_Common Mast_API_CPP Mast_API_C Logger ${PLATFORM_LIBS})

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
