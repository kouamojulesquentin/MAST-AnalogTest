TEMPLATE = app

DEFINES += CXXTEST_HAVE_EH CXXTEST_HAVE_STD CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
QMAKE_CXXFLAGS += -fmax-errors=3


!exists( Generated/Runner.cpp ) {
    error("Generated/Runner.cpp has not been generated")
}

INCLUDEPATH += $$PWD/../Mast_Core/include        \
               $$PWD/../SIT_reader               \
               $$PWD/../Logger                   \
               $$PWD/../cxxtest                  \
               UT_Helpers

DEPENDPATH +=  $$PWD/../Mast_Core/include        \
               $$PWD/../SIT_reader               \
               $$PWD/../Logger                   \
               $$PWD/../cxxtest                  \
               UT_Helpers

win32 {
  Flex = "$$clean_path($$(FLEX_DIR))"
  INCLUDEPATH +=  $$Flex
}

SOURCES += \
    Generated/Runner.cpp    \
    Use_Cases/UT_reader.cpp \
    UT_Helpers/UT_reader_wrapper.cpp


HEADERS += \
    Use_Cases/UT_reader.hpp \
    UT_Helpers/UT_reader_wrapper.hpp


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/SIT_reader_UT/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug)   -lMast_Core -lLogger -lSIT_reader_Lib
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/SIT_reader_UT/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lMast_Core -lLogger -lSIT_rader_Lib
}

unix {
    LIBS += -lpthread
}
