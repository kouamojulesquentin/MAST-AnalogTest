# Mast_Core Unit tests:

project (Mast_Core_UT CXX)
message("")
message("================================================================================")

set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")


set(HELPERS_HEADERS  "${CMAKE_CURRENT_LIST_DIR}/UT_Helpers/Fake_SVF_Simulator.hpp"
                     "${CMAKE_CURRENT_LIST_DIR}/UT_Helpers/Pusher.hpp"
                     "${CMAKE_CURRENT_LIST_DIR}/UT_Helpers/StaticTestSpy.hpp"
                     "${CMAKE_CURRENT_LIST_DIR}/UT_Helpers/TestStrategies.hpp")

set(HELPERS_SOURCES  "${CMAKE_CURRENT_LIST_DIR}/UT_Helpers/Fake_SVF_Simulator.cpp"
                     "${CMAKE_CURRENT_LIST_DIR}/UT_Helpers/StaticTestSpy.cpp")


set(SYSTEM_MODEL_HEADERS "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_AccessInterfaceProtocolFactory.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_AccessInterfaceProtocolFactory.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_GenericAccessInterfaceProtocol.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelNode.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_Register.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_VirtualRegister.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_RegistersAlias.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_AliasRepository.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ParentNode.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ConfigureAlgorithmFactory.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ConfigureVisitor.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultBinaryPathSelector.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultNHotPathSelector.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultOneHotPathSelector.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultTableBasedPathSelector.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_FromSutUpdater.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_NamesChecker.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_PrettyPrinter.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_GmlPrinter.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_NodePathResolver.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_PathSelectorFactory.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_PropagatePendingVisitor.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_I2C_EmulationProtocol.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_STIL_EmulationProtocol.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SVF_SimulationProtocol.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_Remote_Protocol_Proxy.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DataCycle.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ToSutVisitor.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModel.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelBuilder.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelChecker.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelFactory.hpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelManager.hpp"
                         )

set(SYSTEM_MODEL_SOURCES "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_AccessInterfaceProtocolFactory.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_AliasRepository.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ConfigureAlgorithmFactory.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ConfigureVisitor.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DataCycle.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultBinaryPathSelector.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultNHotPathSelector.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultOneHotPathSelector.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_DefaultTableBasedPathSelector.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_FromSutUpdater.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_GenericAccessInterfaceProtocol.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_GmlPrinter.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_I2C_EmulationProtocol.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_NamesChecker.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_NodePathResolver.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ParentNode.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_PathSelectorFactory.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_PrettyPrinter.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_PropagatePendingVisitor.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_Register.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_RegistersAlias.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_Remote_Protocol_Proxy.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_STIL_EmulationProtocol.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SVF_SimulationProtocol.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModel.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelBuilder.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelChecker.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelFactory.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelManager.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_SystemModelNode.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_ToSutVisitor.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_VirtualRegister.cpp"
                         "${CMAKE_CURRENT_LIST_DIR}/SystemModel/UT_VirtualRegister.hpp")

set(UTILITY_HEADERS "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_BinaryVector.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_BinaryVector.hpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_FileSystem.hpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_IndexedRange.hpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_MastConfiguration.hpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_MTQueue.hpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_SVFVector.hpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_Utility.hpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_EnumsUtility.hpp")

set(UTILITY_SOURCES "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_BinaryVector.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_FileSystem.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_IndexedRange.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_MastConfiguration.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_MTQueue.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_SVFVector.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_Utility.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_EnumsUtility.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Utility/UT_EnumsUtility.hpp")

set(PLUGINS_HEADERS "${CMAKE_CURRENT_LIST_DIR}/Plugins/UT_Plugins.hpp")
set(PLUGINS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/Plugins/UT_Plugins.cpp")

set(YAML_HEADERS    "${CMAKE_CURRENT_LIST_DIR}/Yaml/UT_YamlFile.hpp")
set(YAML_SOURCES    "${CMAKE_CURRENT_LIST_DIR}/Yaml/UT_YamlFile.cpp")

set(CORE_HEADERS    "${CMAKE_CURRENT_LIST_DIR}/Core/UT_PDL_AlgorithmsRepository.hpp")
set(CORE_SOURCES    "${CMAKE_CURRENT_LIST_DIR}/Core/UT_PDL_AlgorithmsRepository.cpp")

set(GENERATED_RUNNER "${CMAKE_CURRENT_LIST_DIR}/Generated/Runner.cpp")


set(UT_HEADERS ${UT_COMMON_HEADERS}
               ${UTILITY_HEADERS}
               ${YAML_HEADERS}
               ${PLUGINS_HEADERS}
               ${CORE_HEADERS}
               ${SYSTEM_MODEL_HEADERS}
               )

set(UT_SOURCES ${HELPERS_SOURCES}
               ${UTILITY_SOURCES}
               ${YAML_SOURCES}
               ${PLUGINS_SOURCES}
               ${CORE_SOURCES}
               ${SYSTEM_MODEL_SOURCES}
               ${GENERATED_RUNNER})


# ---------------- Generate Runner.cpp
#
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "Mast_Core_UT: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")
include(../Tests_Common/MakeRunner.cmake)
MakeRunner(${CXXTEST_PATH} ${GENERATED_RUNNER} "${EXTRA_RUNNER_DEPENDS}" "${UT_HEADERS}")

message("================================================================================")
message("")

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

# ---------------- Runner application
#
add_executable             (Mast_Core_UT ${UT_SOURCES} ${UT_HEADERS} ${HELPERS_HEADERS})
target_compile_options     (Mast_Core_UT PUBLIC  -fexceptions -std=c++14)
target_compile_definitions (Mast_Core_UT PUBLIC  CXXTEST_HAVE_EH CXXTEST_HAVE_STD CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)
target_link_libraries      (Mast_Core_UT ${PLATFORM_LIBS} Mast_UT_Helpers Tests_Common Mast_Core )
target_include_directories (Mast_Core_UT PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/UT_Helpers
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/Utility
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/Yaml
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/Plugins
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/Core
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/SystemModel
                                                 ${LIBMAST_CORE_INTERNAL_DIRS}
                                                 ${LIBTCLAP_INCLUDE_DIRS}
                                                 ${LIBYAML_INCLUDE_DIRS})

install(DIRECTORY UT_TestFiles DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Mast_Core)

# ---------------- Fake plugins
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_DIR})  # For Windows
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGINS_DIR})  # For Linux

add_library           (Fake_Plugin_1 SHARED     "Plugins/Fake_Plugin_1.cpp")
set_target_properties (Fake_Plugin_1 PROPERTIES COMPILE_FLAGS "${WARNINGS} -std=c++14")
target_link_libraries (Fake_Plugin_1 ${PLATFORM_LIBS} Mast_UT_Helpers)


include(CTest)
add_test(NAME Mast_Core_UT COMMAND Mast_Core_UT)
