#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T21:12:04
#
#-------------------------------------------------

QT -= core gui

TARGET   = Mast
TEMPLATE = lib

DEFINES += LIB_LIBRARY
CONFIG  += c++14
QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH += C_API                      \
               CPP_API                    \
               $$PWD/../SIT_reader        \
               $$PWD/../Mast_Core/include \
               $$PWD/../Logger

DEPENDPATH  += C_API                      \
               CPP_API                    \
               $$PWD/../SIT_reader        \
               $$PWD/../Mast_Core/include \
               $$PWD/../Logger


SOURCES += \
           CPP_API/PDL_Adapter_CPP.cpp  \
           C_API/PDL_Adapter.cpp        \
           C_API/SystemModelAdapter.cpp

HEADERS += \
    CPP_API/PDL_Adapter_CPP.h  \
    C_API/C_API_Commons.hpp    \
    C_API/PDL_Adapter.h        \
    C_API/SystemModelAdapter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


CONFIG(debug,   debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast/debug)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug) -lSIT_reader_Lib -lLogger -lMast_Core
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast/release)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lSIT_reader_Lib -lLogger -lMast_Core
}
