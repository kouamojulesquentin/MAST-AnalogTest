#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T21:12:04
#
#-------------------------------------------------

QT -= core gui

TARGET   = Mast_Core
TEMPLATE = lib

DEFINES += LIB_LIBRARY
CONFIG  += c++14
QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH += include        \
               $$PWD/../Logger

DEPENDPATH += include        \
               $$PWD/../Logger


SOURCES +=  \
    src/AccessInterface.cpp                         \
    src/LoopbackAccessInterfaceProtocol.cpp         \
    src/BinaryVector.cpp                            \
    src/BrocadeSelector.cpp                         \
    src/Chain.cpp                                   \
    src/Checker.cpp                                 \
    src/ConfigureVisitor.cpp                        \
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
    src/MismatchesCollector.cpp                     \
    src/NamesChecker.cpp                            \
    src/NodePathResolver.cpp                        \
    src/OpenOCDProtocol.cpp                         \
    src/ParentNode.cpp                              \
    src/PrettyPrinter.cpp                           \
    src/PropagatePendingVisitor.cpp                 \
    src/Register.cpp                                \
    src/Session.cpp                                 \
    src/SPI_Player.cpp                              \
    src/SPI_Protocol.cpp                            \
    src/Startup.cpp                                 \
    src/SVF_Player.cpp                              \
    src/SVF_EmulationProtocol.cpp                   \
    src/SVF_SimulationProtocol.cpp                  \
    src/SVFVector.cpp                               \
    src/SystemModel.cpp                             \
    src/SystemModelBuilder.cpp                      \
    src/SystemModelManager.cpp                      \
    src/SystemModelManagerMonitor.cpp               \
    src/SystemModelChecker.cpp                      \
    src/SystemModelCheckResult.cpp                  \
    src/SystemModelNode.cpp                         \
    src/SystemModelReseter.cpp                      \
    src/SystemModelVisitor.cpp                      \
    src/ToSutVisitor.cpp                            \
    src/Utility.cpp

HEADERS += \
    include/AccessInterface.hpp                         \
    include/AccessInterfaceProtocol.hpp                 \
    include/AppFunctionAndNode.hpp                      \
    include/LoopbackAccessInterfaceProtocol.hpp         \
    include/BinaryVector.hpp                            \
    include/BrocadeSelector.hpp                         \
    include/Chain.hpp                                   \
    include/Checker.hpp                                 \
    include/ConfigurationAlgorithm.hpp                  \
    include/ConfigureVisitor.hpp                        \
    include/DefaultBinaryPathSelector.hpp               \
    include/ConfigureAlgorithm_Last_Lazy.hpp            \
    include/ConfigureAlgorithm_LastOrDefault.hpp        \
    include/ConfigureAlgorithm_LastOrDefault_Greedy.hpp \
    include/DefaultTableBasedPathSelector.hpp           \
    include/DefaultOneHotPathSelector.hpp               \
    include/DefaultNHotPathSelector.hpp                 \
    include/GenericAccessInterfaceProtocol.hpp          \
    include/GmlPrinter.hpp                              \
    include/FromSutUpdater.hpp                          \
    include/I2C_EmulationProtocol.hpp                   \
    include/I2C_Player.hpp                              \
    include/Linker.hpp                                  \
    include/MismatchesCollector.hpp                     \
    include/NamesChecker.hpp                            \
    include/NodePathResolver.hpp                        \
    include/OpenOCDProtocol.hpp                         \
    include/ParentNode.hpp                              \
    include/PathSelector.hpp                            \
    include/PrettyPrinter.hpp                           \
    include/PropagatePendingVisitor.hpp                 \
    include/Register.hpp                                \
    include/SPI_Player.hpp                              \
    include/SPI_Protocol.hpp                            \
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
    include/SystemModelReseter.hpp                      \
    include/SystemModelVisitor.hpp                      \
    include/ToSutVisitor.hpp                            \
    include/Utility.hpp                                 \
    include/Platform.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32 {
  DEFINES += WINDOWS BUILD_DLL
  DEFINES += __LITTLE_ENDIAN=1
  DEFINES += __BIG_ENDIAN=2
  DEFINES += __BYTE_ORDER=1
}


CONFIG(debug,   debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_Core/debug)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug) -lLogger
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_Core/release)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lLogger
}
