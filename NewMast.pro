TEMPLATE = subdirs
CONFIG  += ordered
CONFIG  -= qml_debug

SUBDIRS += \
    Logger        \
    Mast_Core     \
    SIT_reader    \
    Mast          \
    Mast_UT       \
    SIT_reader_UT \
    TestCasesApp
