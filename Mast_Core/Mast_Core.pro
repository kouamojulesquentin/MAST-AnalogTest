#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T21:12:04
#
#-------------------------------------------------

QT -= core gui

TARGET   = Mast_Core
TEMPLATE = lib

CONFIG  += c++14
QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH += include         \
               internal        \
               $$PWD/../Logger \
               $$PWD/../External_Libs/tclap/include
               $$PWD/../External_Libs/Yaml/include

DEPENDPATH += include         \
              internal        \
              $$PWD/../Logger \
              $$PWD/../External_Libs/tclap/include
              $$PWD/../External_Libs/Yaml/include


SOURCES +=  \
    src/AccessInterface.cpp                         \
    src/AccessInterfaceProtocolFactory.cpp          \
    src/LoopbackAccessInterfaceProtocol.cpp         \
    src/BinaryVector.cpp                            \
    src/BrocadeSelector.cpp                         \
    src/Chain.cpp                                   \
    src/Checker.cpp                                 \
    src/ConfigureVisitor.cpp                        \
    src/ConfigureAlgorithmFactory.cpp               \
    src/ConfigureAlgorithm_Last_Lazy.cpp            \
    src/ConfigureAlgorithm_LastOrDefault.cpp        \
    src/ConfigureAlgorithm_LastOrDefault_Greedy.cpp \
    src/DefaultBinaryPathSelector.cpp               \
    src/DefaultTableBasedPathSelector.cpp           \
    src/DefaultOneHotPathSelector.cpp               \
    src/DefaultNHotPathSelector.cpp                 \
    src/FromSutUpdater.cpp                          \
    src/GenericAccessInterfaceProtocol.cpp          \
    src/GmlPrinter.cpp                              \
    src/I2C_EmulationProtocol.cpp                   \
    src/I2C_Player.cpp                              \
    src/Linker.cpp                                  \
    src/MastConfiguration.cpp                       \
    src/MismatchesCollector.cpp                     \
    src/NamesChecker.cpp                            \
    src/NodePathResolver.cpp                        \
    src/OfflineProtocol.cpp                         \
    src/ParentNode.cpp                              \
    src/PathSelector.cpp                            \
    src/PathSelectorFactory.cpp                     \
    src/PrettyPrinter.cpp                           \
    src/PropagatePendingVisitor.cpp                 \
    src/Register.cpp                                \
    src/Remote_Protocol_Proxy.cpp                   \
    src/RemoteProtocolFactory.cpp                   \
    src/Session.cpp                                 \
    src/Startup.cpp                                 \
    src/STIL_Player.cpp                             \
    src/STIL_EmulationProtocol.cpp                  \
    src/SVF_Player.cpp                              \
    src/SVF_EmulationProtocol.cpp                   \
    src/SVF_SimulationProtocol.cpp                  \
    src/SVFVector.cpp                               \
    src/SystemModel.cpp                             \
    src/SystemModelBuilder.cpp                      \
    src/SystemModelManager.cpp                      \
    src/SystemModelManager_impl.cpp                 \
    src/SystemModelManagerMonitor.cpp               \
    src/SystemModelChecker.cpp                      \
    src/SystemModelCheckResult.cpp                  \
    src/SystemModelNode.cpp                         \
    src/SystemModelReseter.cpp                      \
    src/SystemModelVisitor.cpp                      \
    src/ToSutVisitor.cpp                            \
    src/Utility.cpp                                 \
    src/YamlFile.cpp                                \
    src/YamlNodesCache.cpp

HEADERS += \
    include/AccessInterface.hpp                         \
    include/AccessInterfaceProtocol.hpp                 \
    include/AccessInterfaceProtocolFactory.hpp          \
    include/AppFunctionAndNode.hpp                      \
    include/LoopbackAccessInterfaceProtocol.hpp         \
    include/BinaryVector.hpp                            \
    include/BrocadeSelector.hpp                         \
    include/Chain.hpp                                   \
    include/Checker.hpp                                 \
    include/ConfigurationAlgorithm.hpp                  \
    include/ConfigureAlgorithmFactory.hpp               \
    include/ConfigureAlgorithm_Last_Lazy.hpp            \
    include/ConfigureAlgorithm_LastOrDefault.hpp        \
    include/ConfigureAlgorithm_LastOrDefault_Greedy.hpp \
    include/ConfigureVisitor.hpp                        \
    include/DefaultBinaryPathSelector.hpp               \
    include/DefaultTableBasedPathSelector.hpp           \
    include/DefaultOneHotPathSelector.hpp               \
    include/DefaultNHotPathSelector.hpp                 \
    include/Factory.hpp                                 \
    include/GenericAccessInterfaceProtocol.hpp          \
    include/GmlPrinter.hpp                              \
    include/I2C_EmulationProtocol.hpp                   \
    include/I2C_Player.hpp                              \
    include/Linker.hpp                                  \
    include/Mast_Core_export.hpp                        \
    include/NodePathResolver.hpp                        \
    include/OfflineProtocol.hpp                         \
    include/ParentNode.hpp                              \
    include/PathSelector.hpp                            \
    include/PathSelectorFactory.hpp                     \
    include/PrettyPrinter.hpp                           \
    include/Remote_Protocol.hpp                         \
    include/Remote_Protocol_Client.hpp                  \
    include/Remote_Protocol_Proxy.hpp                   \
    include/RemoteProtocolFactory.hpp                   \
    include/Register.hpp                                \
    include/STIL_Player.hpp                             \
    include/STIL_EmulationProtocol.hpp                  \
    include/SVF_Player.hpp                              \
    include/SVF_EmulationProtocol.hpp                   \
    include/SVF_SimulationProtocol.hpp                  \
    include/SVFVector.hpp                               \
    include/Session.hpp                                 \
    include/Startup.hpp                                 \
    include/SystemModel.hpp                             \
    include/SystemModelBuilder.hpp                      \
    include/SystemModelManager.hpp                      \
    include/SystemModelManagerMonitor.hpp               \
    include/SystemModelChecker.hpp                      \
    include/SystemModelCheckResult.hpp                  \
    include/SystemModelNode.hpp                         \
    include/SystemModelNodes.hpp                        \
    include/SystemModelVisitor.hpp                      \
    include/Utility.hpp                                 \
    include/YamlFile.hpp                                \
    internal/PropagatePendingVisitor.hpp                \
    internal/SystemModelReseter.hpp                     \
    internal/FromSutUpdater.hpp                         \
    internal/ToSutVisitor.hpp                           \
    internal/MastConfig.hpp                             \
    internal/MastConfiguration.hpp                      \
    internal/MismatchesCollector.hpp                    \
    internal/NamesChecker.hpp                           \
    internal/SystemModelManager_impl.hpp                \
    internal/YamlNodesCache.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32 {
  DEFINES += WINDOWS
  DEFINES += MAST_CORE_EXPORTS
  DEFINES += __LITTLE_ENDIAN=1
  DEFINES += __BIG_ENDIAN=2
  DEFINES += __BYTE_ORDER=1
}

DEFINES += HAS_EXPERIMENTAL_STRING_VIEW


CONFIG(debug,   debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_Core/debug)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug) -lLogger -lyaml-cpp
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_Core/release)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lLogger -lyaml-cpp
}
