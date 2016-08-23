TARGET   = SIT_Reader_Lib
TEMPLATE = lib

CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3


!exists( SIT_lexer.yy.cc ) {
    error("Lexer has not been generated")
}

!exists( SIT_parser.tab.cc ) {
    error("Parser has not been generated")
}

INCLUDEPATH += $$PWD/../Lib/include            \
               $$PWD/../Lib/public_include     \
               $$PWD/../Logger

DEPENDPATH += $$PWD/../Lib/include        \
              $$PWD/../Lib/public_include \
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
    position.hh \
    stack.hh


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/SIT_reader/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/debug)   -lLib -lLogger
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/SIT_reader/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/release) -lLib -lLogger
}

unix {
    LIBS += -lpthread
}
