TARGET   = SIT_reader_Lib
TEMPLATE = lib

CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += warn_off

QMAKE_CXXFLAGS += -Wnarrowing -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum
QMAKE_CXXFLAGS += -fmax-errors=3


!exists( SIT_lexer.yy.cc ) {
    error("Lexer has not been generated")
}

!exists( SIT_parser.tab.cc ) {
    error("Parser has not been generated")
}

INCLUDEPATH += $$PWD/../Mast_Core/include            \
               $$PWD/../Mast_Core/public_include     \
               $$PWD/../Logger

DEPENDPATH += $$PWD/../Mast_Core/include        \
              $$PWD/../Mast_Core/public_include \
              $$PWD/../Logger

win32 {
  Flex = "$$clean_path($$(FLEX_DIR))"
  INCLUDEPATH +=  $$Flex
}


SOURCES += \
    SIT_parser.tab.cc \
    SIT_lexer.yy.cc   \
    SIT_reader.cpp


HEADERS += \
    location.hh \
    position.hh \
    SIT_reader.hpp \
    stack.hh


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/SIT_reader/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug)   -lMast_Core -lLogger
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/SIT_reader/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lMast_Core -lLogger
}

unix {
    LIBS += -lpthread
}
