TEMPLATE = subdirs
CONFIG  += ordered
CONFIG  -= qml_debug

SUBDIRS += Yaml

unix {
  SUBDIRS +=    \
      FTDI_SPI
}


