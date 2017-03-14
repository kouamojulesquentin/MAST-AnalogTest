TEMPLATE = subdirs
CONFIG  += ordered
CONFIG  -= qml_debug

unix {
  SUBDIRS += \
      FTDI_SPI
}


