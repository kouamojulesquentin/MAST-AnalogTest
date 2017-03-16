TARGET   = Spi
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
               $$PWD/../../External_Libs/FTDI_SPI/include

DEPENDPATH  += include \
               $$PWD/../../Mast_Core/include  \
               $$PWD/../../Logger             \
               $$PWD/../../External_Libs/FTDI_SPI/include

SOURCES += \
    src/SPI_Player.cpp \
    src/SPI_Protocol.cpp


HEADERS += \
    include/SPI_Player.hpp \
    include/SPI_Protocol.hpp


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Spi/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug)   -lftdi_spi
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Spi/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lftdi_spi
}

unix {
    LIBS += -lpthread
}
