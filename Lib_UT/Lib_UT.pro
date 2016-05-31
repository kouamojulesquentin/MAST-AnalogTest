TEMPLATE = app

DEFINES += CXXTEST_HAVE_EH CXXTEST_HAVE_STD CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default


!exists( Generated/Runner.cpp ) {
    error("Generated/Runner.cpp has not been generated")
}

INCLUDEPATH += $$PWD/../Lib/include \
               $$PWD/../Lib/public_include \
               $$PWD/../cxxtest CxxTest_Traits \
               UT_Helpers

DEPENDPATH += $$PWD/../Lib/include \
              $$PWD/../Lib/public_include \
              $$PWD/../cxxtest CxxTest_Traits \
              UT_Helpers


SOURCES += \
    SystemModel/UT_DefaultBinaryPathSelector.cpp \
    SystemModel/UT_GmlPrinterVisitor.cpp \
    SystemModel/UT_PrettyPrinterVisitor.cpp \
    SystemModel/UT_Register.cpp \
    SystemModel/UT_SystemModel.cpp \
    SystemModel/UT_SystemModelCheckerVisitor.cpp \
    UT_Helpers/SystemModelBuilder.cpp \
    UT_Helpers/UT_SystemModelBuilder.cpp \
    Utility/UT_BinaryVector.cpp \
    Utility/UT_SVFVector.cpp \
    Utility/UT_Utility.cpp \
    Generated/Runner.cpp \
    SystemModel/UT_SystemModelNode.cpp

HEADERS += \
    SystemModel/UT_DefaultBinaryPathSelector.hpp \
    SystemModel/UT_GmlPrinterVisitor.hpp \
    SystemModel/UT_PrettyPrinterVisitor.hpp \
    SystemModel/UT_Register.hpp \
    SystemModel/UT_SystemModel.hpp \
    SystemModel/UT_SystemModelCheckerVisitor.hpp \
    UT_Helpers/SystemModelBuilder.hpp \
    UT_Helpers/UT_SystemModelBuilder.hpp \
    Utility/UT_BinaryVector.hpp \
    Utility/UT_SVFVector.hpp \
    Utility/UT_Utility.hpp \
    CxxTest_Traits/BinaryVector_Traits.hpp \
    CxxTest_Traits/SystemModelCheckResult_Traits.hpp \
    SystemModel/UT_SystemModelNode.hpp

#+SUBDIRS += \
#+    ../Lib/Lib.pro

CONFIG(debug,   debug|release): {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib_UT/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/debug)   -lLib
}
else: CONFIG(release, debug|release): {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib_UT/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/release) -lLib
}

#+CONFIG(release, debug|release):    LIBS += -L$$clean_path($$OUT_PWD/../Lib/release) -lLib
#+else:CONFIG(debug, debug|release): LIBS += -L$$clean_path($$OUT_PWD/../Lib/debug)   -lLib

#+win32:CONFIG(release, debug|release):    LIBS += -L$$clean_path($$OUT_PWD/../Lib/release) -lLib
#+else:win32:CONFIG(debug, debug|release): LIBS += -L$$clean_path($$OUT_PWD/../Lib/debug)   -lLib
#+else:unix:CONFIG(debug, debug|release):  LIBS += -L$$clean_path($$OUT_PWD/../Lib)         -lLib


