TARGET   = FTDI_SPI
TEMPLATE = lib

CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += warn_off

QMAKE_CXXFLAGS += -Wnarrowing -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum
QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH += include
DEPENDPATH  += include
SOURCES     += src/ftdispi.c
HEADERS     += include/ftdispi.h


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/FTDI_SPI/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug -lftdi)
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/FTDI_SPI/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release -lftdi)
}

