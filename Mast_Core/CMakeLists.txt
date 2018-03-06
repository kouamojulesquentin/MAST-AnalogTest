project(Mast_Core CXX)

include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBTCLAP_INCLUDE_DIRS})
include_directories(${LIBYAML_INCLUDE_DIRS})

set(LIBMAST_CORE_INCLUDE_DIRS  "${CMAKE_CURRENT_SOURCE_DIR}/include/"  PARENT_SCOPE)  # For libs that only use headers (without linking with Mast_Core)
set(LIBMAST_CORE_INTERNAL_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/internal/" PARENT_SCOPE)  # For other lib of mast project

if (CODE_COVERAGE)
  set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS}")
endif()

if (WIN32)
  set(Mast_Core_Platform_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Windows/Dll.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/src/Windows/FileSystem.cpp")
elseif(UNIX)
  set(Mast_Core_Platform_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Linux/Dll.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/src/Linux/FileSystem.cpp")
else()
  set(Mast_Core_Platform_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Dummy/Dll.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/src/Dummy/FileSystem.cpp")
endif()

set(Mast_Core_SOURCES ${Mast_Core_Platform_SOURCES}
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/AccessInterface.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/AccessInterfaceProtocolFactory.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/AccessInterfaceTranslator.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/AliasRepository.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/BinaryVector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/BrocadeSelector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Chain.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/CheckResult.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Checker.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigureAlgorithmFactory.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigureAlgorithm_LastOrDefault.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigureAlgorithm_LastOrDefault_Greedy.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigureAlgorithm_Last_Lazy.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigureVisitor.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/DataCycleVisitor.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/DefaultBinaryPathSelector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/DefaultNHotPathSelector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/DefaultOneHotPathSelector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/DefaultTableBasedPathSelector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Dll.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/FromSutUpdater.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/GenericAccessInterfaceProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/GmlPrinter.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/I2C_EmulationProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/I2C_Player.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Linker.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/LoopbackAccessInterfaceProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/MastConfiguration.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/MismatchesCollector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/NamesChecker.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/NodePathResolver.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/OfflineProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/PDL_AlgorithmsRepository.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/ParentNode.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/PathSelector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/PathSelectorFactory.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Plugins.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/PrettyPrinter.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/PropagatePendingVisitor.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Register.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/RegistersAlias.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/RemoteProtocolFactory.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Remote_Protocol_Proxy.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/STIL_EmulationProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/STIL_Player.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SVFVector.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SVF_EmulationProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SVF_Player.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SVF_SimulationProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Session.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Startup.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModel.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelBuilder.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelChecker.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelCheckerBase.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelFactory.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelManager.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelManagerMonitor.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelManager_impl.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelNode.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelReseter.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SystemModelVisitor.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/ToSutVisitor.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Utility.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/VirtualRegister.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/YamlFile.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/YamlNodesCache.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/AccessInterfaceTranslatorProtocolFactory.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/Emulation_TranslationProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/I2C_RawPlayer.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/JTAG_to_I2C_TranslatorProtocol.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SVF_RawPlayer.cpp"
                      )

set(Public_Headers "${CMAKE_CURRENT_SOURCE_DIR}/include/AccessInterface.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/AccessInterfaceProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/AccessInterfaceProtocolFactory.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/AccessInterfaceTranslator.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/AppFunctionAndNode.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/AppFunctionNameAndNode.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/AliasRepository.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/BinaryVector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/BitsOrdering.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/BrocadeSelector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/CallbackRequest.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Chain.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/CheckResult.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Checker.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Conditioners.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/ConfigurationAlgorithm.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/ConfigureAlgorithmFactory.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/ConfigureAlgorithm_LastOrDefault.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/ConfigureAlgorithm_LastOrDefault_Greedy.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/ConfigureAlgorithm_Last_Lazy.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/ConfigureVisitor.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/DataCycleVisitor.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultBinaryPathSelector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultNHotPathSelector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultOneHotPathSelector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultTableBasedPathSelector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/EnumsUtility.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Factory.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/GenericAccessInterfaceProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/GmlPrinter.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/I2C_EmulationProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/I2C_Player.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/IndexedRange.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Linker.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/LoopbackAccessInterfaceProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/MTQueue.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Mast_Core_export.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/NodePathResolver.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/OfflineProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/PDL_AlgorithmsRepository.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/ParentNode.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/PathSelector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/PathSelectorFactory.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Plugins.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/PrettyPrinter.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Register.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/RegisterInterface.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/RegistersAlias.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/RemoteProtocolFactory.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Remote_Protocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Remote_Protocol_Client.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Remote_Protocol_Proxy.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Remote_Protocol_Server.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/STIL_EmulationProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/STIL_Player.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SVFVector.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SVF_EmulationProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SVF_Player.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SVF_SimulationProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Session.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Startup.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SubModelPlaceHolder.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModel.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelBuilder.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelChecker.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelCheckerBase.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelFactory.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelManager.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelManagerMonitor.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelNode.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelNodes.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SystemModelVisitor.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Utility.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/VirtualRegister.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/YamlFile.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/Emulation_TranslatorProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/I2C_RawPlayer.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/JTAG_to_I2C_TranslatorProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/SVF_RawPlayer.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/T_2_E_TranslatorProtocol.hpp"
                   "${CMAKE_CURRENT_SOURCE_DIR}/include/T_2_T_TranslatorProtocol.hpp"
                  )

set(Private_Headers "${CMAKE_CURRENT_SOURCE_DIR}/internal/Dll.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/FileSystem.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/FromSutUpdater.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/MastConfiguration.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/MismatchesCollector.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/NamesChecker.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/PropagatePendingVisitor.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/SystemModelManager_impl.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/SystemModelReseter.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/ToSutVisitor.hpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/internal/YamlNodesCache.hpp")

set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/internal/MastConfig.hpp" GENERATED)

add_library                (Mast_Core SHARED ${Mast_Core_SOURCES} ${Private_Headers} ${Public_Headers})
target_include_directories (Mast_Core PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include/"
                                      PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/internal/")
set_target_properties      (Mast_Core PROPERTIES COMPILE_FLAGS "${CFLAGS}")
target_compile_definitions (Mast_Core PRIVATE G3_DYNAMIC_LOGGING
                                              __LITTLE_ENDIAN=1
                                              __BIG_ENDIAN=2
                                              __BYTE_ORDER=1)

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)  # dl is for loading dlls
endif()

message("")
message("================================================================================")
message(STATUS "Mast_Core:  CODE_COVERAGE:              ${CODE_COVERAGE}")
message(STATUS "Mast_Core:  USE_OPEN_OCD:               ${USE_OPEN_OCD}")
message(STATUS "Mast_Core:  USE_LIBFTDISPI:             ${USE_LIBFTDISPI}")
message(STATUS "Mast_Core:  CMAKE_CXX_FLAGS:            ${CMAKE_CXX_FLAGS}")
message("================================================================================")
message("")

target_link_libraries(Mast_Core Logger ${YAML_LIBRARIES} ${EXTRA_LIBS})
