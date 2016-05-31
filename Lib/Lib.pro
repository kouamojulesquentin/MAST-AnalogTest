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

INCLUDEPATH = include public_include

SOURCES +=  \
    src/AccessInterface.cpp \
    src/AccessInterfaceProtocol_1149_1.cpp \
    src/BinaryVector.cpp \
    src/Chain.cpp \
    src/ConfigureVisitor.cpp \
    src/DefaultBinaryPathSelector.cpp \
    src/GmlPrinterVisitor.cpp \
    src/Linker.cpp \
    src/ParentNode.cpp \
    src/PrettyPrinterVisitor.cpp \
    src/Register.cpp \
    src/SVFVector.cpp \
    src/SystemModel.cpp \
    src/SystemModelCheckerVisitor.cpp \
    src/SystemModelCheckResult.cpp \
    src/SystemModelNode.cpp \
    src/Utility.cpp

HEADERS += \
    include/AccessInterface.hpp \
    include/AccessInterfaceProtocol.hpp \
    include/AccessInterfaceProtocol_1149_1.hpp \
    include/BinaryVector.hpp \
    include/Chain.hpp \
    include/ConfigureVisitor.hpp \
    include/DataCycleReadVisitor.hpp \
    include/DataCycleWriteVisitor.hpp \
    include/DefaultBinaryPathSelector.hpp \
    include/GmlPrinterVisitor.hpp \
    include/Linker.hpp \
    include/ParentNode.hpp \
    include/PathSelector.hpp \
    include/PostConditionVisitor.hpp \
    include/PreconditionVisitor.hpp \
    include/PrettyPrinterVisitor.hpp \
    include/Register.hpp \
    include/SVFVector.hpp \
    include/SystemModel.hpp \
    include/SystemModelCheckerVisitor.hpp \
    include/SystemModelCheckResult.hpp \
    include/SystemModelNode.hpp \
    include/SystemModelNodes.hpp \
    include/SystemModelVisitor.hpp \
    include/TableBasedPathSelector.hpp \
    include/Utility.hpp \
    public_include/PDLAdapter.h \
    public_include/Platform.hpp \
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

CONFIG(debug,   debug|release): {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
}
else: CONFIG(release, debug|release): {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
}
