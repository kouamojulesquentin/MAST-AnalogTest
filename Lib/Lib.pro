#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T21:12:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = Lib
TEMPLATE = lib

DEFINES += LIB_LIBRARY
CONFIG  += c++14
QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH = include public_include

SOURCES +=  \
    src/AccessInterface.cpp                \
    src/LoopbackAccessInterfaceProtocol.cpp \
    src/BinaryVector.cpp                   \
    src/Chain.cpp                          \
    src/ConfigureVisitor.cpp               \
    src/ConfigureAlgorithm_LastOrDefault.cpp  \
    src/DefaultBinaryPathSelector.cpp      \
    src/DefaultTableBasedPathSelector.cpp  \
    src/FromSutUpdater.cpp                 \
    src/GmlPrinterVisitor.cpp              \
    src/Linker.cpp                         \
    src/ParentNode.cpp                     \
    src/PrettyPrinterVisitor.cpp           \
    src/PropagatePendingVisitor.cpp        \
    src/Register.cpp                       \
    src/SVF_Player.cpp                     \
    src/SVF_SimulationProtocol.cpp         \
    src/SVFVector.cpp                      \
    src/SystemModel.cpp                    \
    src/SystemModelManager.cpp             \
    src/SystemModelCheckerVisitor.cpp      \
    src/SystemModelCheckResult.cpp         \
    src/SystemModelVisitor.cpp             \
    src/ToSutVisitor.cpp                   \
    src/SystemModelNode.cpp                \
    src/Utility.cpp

HEADERS += \
    include/AccessInterface.hpp                 \
    include/AccessInterfaceProtocol.hpp         \
    include/LoopbackAccessInterfaceProtocol.hpp \
    include/BinaryVector.hpp                    \
    include/Chain.hpp                           \
    include/ConfigurationAlgorithm.hpp          \
    include/ConfigureVisitor.hpp                \
    include/DefaultBinaryPathSelector.hpp       \
    include/ConfigureAlgorithm_LastOrDefault.hpp   \
    include/DefaultTableBasedPathSelector.hpp   \
    include/GmlPrinterVisitor.hpp               \
    include/FromSutUpdater.hpp                  \
    include/Linker.hpp                          \
    include/ParentNode.hpp                      \
    include/PathSelector.hpp                    \
    include/PostConditionVisitor.hpp            \
    include/PreconditionVisitor.hpp             \
    include/PrettyPrinterVisitor.hpp            \
    include/PropagatePendingVisitor.hpp         \
    include/Register.hpp                        \
    include/SVF_Player.hpp                      \
    include/SVF_SimulationProtocol.hpp          \
    include/SVFVector.hpp                       \
    include/SystemModel.hpp                     \
    include/SystemModelManager.hpp              \
    include/SystemModelCheckerVisitor.hpp       \
    include/SystemModelCheckResult.hpp          \
    include/SystemModelNode.hpp                 \
    include/SystemModelNodes.hpp                \
    include/SystemModelVisitor.hpp              \
    include/ToSutVisitor.hpp                    \
    include/Utility.hpp                         \
    public_include/PDLAdapter.h                 \
    public_include/Platform.hpp                 \
    public_include/SystemModelAdapter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32 {
  DEFINES += WINDOWS BUILD_DLL
}


#+CONFIG(debug,   debug|release): OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib/debug)
#+else: CONFIG(release, debug|release): OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib/release)

CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
}
