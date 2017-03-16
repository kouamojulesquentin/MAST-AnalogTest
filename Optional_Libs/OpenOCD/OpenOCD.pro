TARGET   = OpenOCD
TEMPLATE = lib

CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += warn_off

QMAKE_CXXFLAGS += -Wnarrowing -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum
QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH += include \
               $$PWD/../../Mast_Core/include \
               $$PWD/../../Logger            \
               $$PWD/../../External_Libs/OpenOCD/include

DEPENDPATH  += include \
               $$PWD/../../Mast_Core/include  \
               $$PWD/../../Logger             \
               $$PWD/../../External_Libs/OpenOCD/include

SOURCES += \
    src/OpenOCDProtocol.cpp


HEADERS += \
    include/OpenOCDProtocol.hpp


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/OpenOCD/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug)
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/OpenOCD/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release)
}

LIBS += -L$$clean_path($$PWD/../External_Libs/OpenOCD/lib -llibopenocd

unix {
    LIBS += -lpthread
}
