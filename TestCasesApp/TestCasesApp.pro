TEMPLATE = app

CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3


INCLUDEPATH += $$PWD/../Lib/include            \
               $$PWD/../Lib/public_include     \
               $$PWD/../Logger

DEPENDPATH += $$PWD/../Lib/include \
              $$PWD/../Lib/public_include \
              $$PWD/../Logger


SOURCES += \
    main.cpp


#+HEADERS += \


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/TestCasesApp/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/debug)   -lLib -lLogger
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/TestCasesApp/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/release) -lLib -lLogger
}

unix {
    LIBS += -lpthread
}
