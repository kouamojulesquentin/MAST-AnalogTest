#-------------------------------------------------
QT -= core gui

TARGET   = Mast_API_C
TEMPLATE = lib

DEFINES += LIB_LIBRARY
CONFIG  += c++14
QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH += include                    \
               $$PWD/../SIT_reader        \
               $$PWD/../Mast_Core/include \
               $$PWD/../Logger

DEPENDPATH  += include                    \
               $$PWD/../SIT_reader        \
               $$PWD/../Mast_Core/include \
               $$PWD/../Logger


SOURCES += src/PDL_Adapter_C.cpp            \
           src/SystemModelAdapter_C.cpp

HEADERS += \
    include/Commons_C.hpp                    \
    include/ErrorCode_C.h                    \
    include/PDL_Adapter_C.h                  \
    include/SystemModelAdapter_C.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}


CONFIG(debug,   debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_API_C/debug)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug) -lSIT_reader_Lib -lLogger -lMast_Core
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_API_C/release)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lSIT_reader_Lib -lLogger -lMast_Core
}
