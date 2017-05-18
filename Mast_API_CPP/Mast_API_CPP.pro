QT -= core gui

TARGET   = Mast_API_CPP
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


SOURCES += src/PDL_Adapter_CPP.cpp        \
           src/SystemModelAdapter_CPP.cpp

HEADERS += \
    include/AppFunctionAndName_CPP.hpp     \
    include/AppFunctionAndNodePath_CPP.hpp \
    include/PDL_Adapter_CPP.hpp            \
    include/SystemModelAdapter_CPP.hpp

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}


CONFIG(debug,   debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_API_CPP/debug)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug) -lSIT_reader_Lib -lLogger -lMast_Core
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR =   $$clean_path($$PWD/../Build_Qt/Mast_API_CPP/release)
  DESTDIR     =   $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lSIT_reader_Lib -lLogger -lMast_Core
}
