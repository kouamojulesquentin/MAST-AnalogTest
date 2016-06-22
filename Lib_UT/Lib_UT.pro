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

INCLUDEPATH += $$PWD/../Lib/include \
               $$PWD/../Lib/public_include \
               $$PWD/../cxxtest CxxTest_Traits \
               UT_Helpers

DEPENDPATH += $$PWD/../Lib/include \
              $$PWD/../Lib/public_include \
              $$PWD/../cxxtest CxxTest_Traits \
              UT_Helpers


SOURCES += \
    Generated/Runner.cpp                         \
    SystemModel/Fake_SVF_Simulator.cpp           \
    SystemModel/Spy_AccessInterfaceProtocols.cpp \
    SystemModel/Spy_I2C_Protocol.cpp             \
    SystemModel/Spy_SVF_Protocol.cpp             \
    SystemModel/UT_ConfigureVisitor.cpp          \
    SystemModel/UT_DataCycle.cpp                 \
    SystemModel/UT_DefaultBinaryPathSelector.cpp \
    SystemModel/UT_FromSutUpdater.cpp            \
    SystemModel/UT_GmlPrinterVisitor.cpp         \
    SystemModel/UT_PrettyPrinterVisitor.cpp      \
    SystemModel/UT_PropagatePendingVisitor.cpp   \
    SystemModel/UT_Register.cpp                  \
    SystemModel/UT_SVF_SimulationProtocol.cpp    \
    SystemModel/UT_SystemModel.cpp               \
    SystemModel/UT_SystemModelCheckerVisitor.cpp \
    SystemModel/UT_ToSutVisitor.cpp              \
    SystemModel/UT_SystemModelManager.cpp        \
    SystemModel/UT_SystemModelNode.cpp           \
    UT_Helpers/SystemModelBuilder.cpp            \
    UT_Helpers/UT_SystemModelBuilder.cpp         \
    Utility/UT_BinaryVector.cpp                  \
    Utility/UT_SVFVector.cpp                     \
    Utility/UT_Utility.cpp


HEADERS += \
    CxxTest_Traits/BinaryVector_Traits.hpp           \
    CxxTest_Traits/SystemModelCheckResult_Traits.hpp \
    SystemModel/Fake_SVF_Simulator.hpp               \
    SystemModel/Spy_AccessInterfaceProtocols.hpp     \
    SystemModel/Spy_I2C_Protocol.hpp                 \
    SystemModel/Spy_SVF_Protocol.hpp                 \
    SystemModel/UT_ConfigureVisitor.hpp              \
    SystemModel/UT_DataCycle.hpp                     \
    SystemModel/UT_DefaultBinaryPathSelector.hpp     \
    SystemModel/UT_FromSutUpdater.hpp                \
    SystemModel/UT_GmlPrinterVisitor.hpp             \
    SystemModel/UT_PrettyPrinterVisitor.hpp          \
    SystemModel/UT_PropagatePendingVisitor.hpp       \
    SystemModel/UT_Register.hpp                      \
    SystemModel/UT_SVF_SimulationProtocol.hpp        \
    SystemModel/UT_SystemModel.hpp                   \
    SystemModel/UT_SystemModelCheckerVisitor.hpp     \
    SystemModel/UT_ToSutVisitor.hpp                  \
    SystemModel/UT_SystemModelManager.hpp            \
    SystemModel/UT_SystemModelNode.hpp               \
    UT_Helpers/SystemModelBuilder.hpp                \
    UT_Helpers/UT_SystemModelBuilder.hpp             \
    Utility/UT_BinaryVector.hpp                      \
    Utility/UT_SVFVector.hpp                         \
    Utility/UT_Utility.hpp


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib_UT/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/debug)   -lLib
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Lib_UT/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/bin/release) -lLib
}

unix {
    LIBS += -lpthread
}
