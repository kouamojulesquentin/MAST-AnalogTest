TEMPLATE = app

CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum
QMAKE_CXXFLAGS += -fmax-errors=3


INCLUDEPATH += $$PWD/../Lib/include            \
               $$PWD/../Lib/public_include     \
               $$PWD/../Logger                 \
               $$PWD/../SIT_reader

DEPENDPATH += $$PWD/../Lib/include        \
              $$PWD/../Lib/public_include \
              $$PWD/../Logger             \
              $$PWD/../SIT_reader

win32 {
  INCLUDEPATH +=  "$$(FLEX_DIR)"
}

SOURCES += \
    main.cpp \
    Options.cpp


HEADERS += \
    Options.hpp


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/TestCasesApp/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/debug)   -lLib -lLogger -lSIT_Reader_Lib
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/TestCasesApp/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/release) -lLib -lLogger -lSIT_Reader_Lib
}

unix {
    LIBS += -lpthread
}
