TEMPLATE = subdirs
CONFIG  += ordered
CONFIG  -= qml_debug

SUBDIRS += \
    Logger        \
    External_Libs \
    Mast_Core     \
    SIT_reader    \
    Mast_API_CPP  \
    Mast_API_C    \
    Mast_UT       \
    SIT_reader_UT \
    Optional_Libs \
    TestCasesApp  \
    MastExample_CPP
