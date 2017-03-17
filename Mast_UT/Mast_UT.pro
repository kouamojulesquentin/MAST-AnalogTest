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

INCLUDEPATH += $$PWD/../Mast_Core/include \
               $$PWD/../Mast/CPP_API      \
               $$PWD/../Mast/C_API        \
               $$PWD/../Logger            \
               $$PWD/../cxxtest           \
               CxxTest_Traits             \
               UT_Helpers

DEPENDPATH += $$PWD/../Mast_Core/include \
              $$PWD/../Mast/CPP_API      \
              $$PWD/../Mast/C_API        \
              $$PWD/../cxxtest           \
              CxxTest_Traits             \
              UT_Helpers


SOURCES += \
    Generated/Runner.cpp                                \
    API_C/UT_PDL_Adapter_C.cpp                          \
    API_CPP/UT_PDL_Adapter_CPP.cpp                      \
    SystemModel/UT_AccessInterfaceProtocolFactory.cpp   \
    SystemModel/UT_ConfigureAlgorithmFactory.cpp        \
    SystemModel/UT_ConfigureVisitor.cpp                 \
    SystemModel/UT_DataCycle.cpp                        \
    SystemModel/UT_DefaultBinaryPathSelector.cpp        \
    SystemModel/UT_DefaultNHotPathSelector.cpp          \
    SystemModel/UT_DefaultOneHotPathSelector.cpp        \
    SystemModel/UT_FromSutUpdater.cpp                   \
    SystemModel/UT_GenericAccessInterfaceProtocol.cpp   \
    SystemModel/UT_GmlPrinter.cpp                       \
    SystemModel/UT_NamesChecker.cpp                     \
    SystemModel/UT_NodePathResolver.cpp                 \
    SystemModel/UT_ParentNode.cpp                       \
    SystemModel/UT_PrettyPrinter.cpp                    \
    SystemModel/UT_PropagatePendingVisitor.cpp          \
    SystemModel/UT_Remote_Protocol_Proxy.cpp            \
    SystemModel/UT_Register.cpp                         \
    SystemModel/UT_I2C_EmulationProtocol.cpp            \
    SystemModel/UT_STIL_EmulationProtocol.cpp           \
    SystemModel/UT_SVF_SimulationProtocol.cpp           \
    SystemModel/UT_SystemModel.cpp                      \
    SystemModel/UT_SystemModelBuilder.cpp               \
    SystemModel/UT_SystemModelChecker.cpp               \
    SystemModel/UT_ToSutVisitor.cpp                     \
    SystemModel/UT_SystemModelManager.cpp               \
    SystemModel/UT_SystemModelNode.cpp                  \
    UT_Helpers/Fake_SVF_Simulator.cpp                   \
    UT_Helpers/Spy_AccessInterfaceProtocols.cpp         \
    UT_Helpers/Spy_I2C_Protocol.cpp                     \
    UT_Helpers/Spy_STIL_Protocol.cpp                    \
    UT_Helpers/Spy_SVF_Protocol.cpp                     \
    UT_Helpers/TestModelBuilder.cpp                     \
    UT_Helpers/UT_TestModelBuilder.cpp                  \
    Utility/UT_BinaryVector.cpp                         \
    Utility/UT_SVFVector.cpp                            \
    Utility/UT_Utility.cpp


HEADERS += \
    CxxTest_Traits/BinaryVector_Traits.hpp            \
    CxxTest_Traits/SystemModelCheckResult_Traits.hpp  \
    CxxTest_Traits/Cpp_11_Traits.hpp                  \
    CxxTest_Traits/CPP_API_Traits.hpp                 \
    CxxTest_Traits/C_API_Traits.hpp                   \
    API_C/UT_PDL_Adapter_C.hpp                        \
    API_CPP/UT_PDL_Adapter_CPP.hpp                    \
    SystemModel/UT_AccessInterfaceProtocolFactory.h   \
    SystemModel/UT_ConfigureAlgorithmFactory.hpp      \
    SystemModel/UT_ConfigureVisitor.hpp               \
    SystemModel/UT_DataCycle.hpp                      \
    SystemModel/UT_DefaultBinaryPathSelector.hpp      \
    SystemModel/UT_DefaultNHotPathSelector.hpp        \
    SystemModel/UT_DefaultOneHotPathSelector.hpp      \
    SystemModel/UT_FromSutUpdater.hpp                 \
    SystemModel/UT_GenericAccessInterfaceProtocol.hpp \
    SystemModel/UT_GmlPrinter.hpp                     \
    SystemModel/UT_NamesChecker.hpp                   \
    SystemModel/UT_NodePathResolver.hpp               \
    SystemModel/UT_ParentNode.hpp                     \
    SystemModel/UT_PrettyPrinter.hpp                  \
    SystemModel/UT_PropagatePendingVisitor.hpp        \
    SystemModel/UT_Remote_Protocol_Proxy.hpp          \
    SystemModel/UT_Register.hpp                       \
    SystemModel/UT_I2C_EmulationProtocol.hpp          \
    SystemModel/UT_STIL_EmulationProtocol.hpp         \
    SystemModel/UT_SVF_SimulationProtocol.hpp         \
    SystemModel/UT_SystemModel.hpp                    \
    SystemModel/UT_SystemModelBuilder.hpp             \
    SystemModel/UT_SystemModelChecker.hpp             \
    SystemModel/UT_ToSutVisitor.hpp                   \
    SystemModel/UT_SystemModelManager.hpp             \
    SystemModel/UT_SystemModelNode.hpp                \
    UT_Helpers/Fake_SVF_Simulator.hpp                 \
    UT_Helpers/SpiedProtocolsCommands.hpp             \
    UT_Helpers/Spy_AccessInterfaceProtocols.hpp       \
    UT_Helpers/Spy_I2C_Protocol.hpp                   \
    UT_Helpers/Spy_STIL_Protocol.hpp                  \
    UT_Helpers/Spy_SVF_Protocol.hpp                   \
    UT_Helpers/TestModelBuilder.hpp                   \
    UT_Helpers/UT_TestModelBuilder.hpp                \
    Utility/UT_BinaryVector.hpp                       \
    Utility/UT_SVFVector.hpp                          \
    Utility/UT_Utility.hpp


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Mast_UT/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/debug)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/debug)   -lMast -lMast_Core -lLogger -lSIT_reader_Lib
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Mast_UT/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/Bin/release)
  LIBS       += -L$$clean_path($$PWD/../Build_Qt/Bin/release) -lMast -lMast_Core -lLogger -lSIT_reader_Lib
}

unix {
    LIBS += -lpthread
}
