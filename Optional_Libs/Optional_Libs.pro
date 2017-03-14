TEMPLATE = subdirs
CONFIG  += ordered
CONFIG  -= qml_debug

unix {
  SUBDIRS += \
      Spi
}

