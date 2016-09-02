TEMPLATE = subdirs
CONFIG  += ordered
CONFIG  -= qml_debug

SUBDIRS += \
    Logger        \
    Mast_Core     \
    Mast          \
    Mast_UT       \
    SIT_reader    \
    SIT_reader_UT \
    TestCasesApp
