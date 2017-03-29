cmake_minimum_required(VERSION 2.6)
project(External_Libs)
include(ExternalProject)

if (NOT XmlRpcInstall)
  message(STATUS "External_Libs: Setting XmlRpcInstall")
  set(XmlRpcInstall ${CMAKE_CURRENT_SOURCE_DIR}/xmlrpc-c)
endif()

message("")
message("================================================================================")
message(STATUS "External_Libs: XmlRpcInstall:            ${XmlRpcInstall}")
message("================================================================================")
message("")

if(${USE_LIBFTDISPI} MATCHES "ON")
  add_subdirectory (FTDI_SPI)
  set(LIBFTDI_SPI_INCLUDE_DIRS "${LIBFTDI_SPI_INCLUDE_DIRS}" PARENT_SCOPE)

  message("")
  message("================================================================================")
  message(STATUS "External_Libs: LIBFTDI_SPI_INCLUDE_DIRS:  ${LIBFTDI_SPI_INCLUDE_DIRS}")
  message("================================================================================")
  message("")
endif()

if (${USE_OPEN_OCD} MATCHES "ON")
  add_subdirectory(OpenOCD)
  message("")
  message("================================================================================")
  message(STATUS "External_Libs: OPEN_OCD_LIBRARIES:        ${OPEN_OCD_LIBRARIES}")
  message(STATUS "External_Libs: LIBOPEN_OCD_INCLUDE_DIRS:  ${LIBOPEN_OCD_INCLUDE_DIRS}")
  message("================================================================================")
  message("")
  set(OPEN_OCD_LIBRARIES       ${OPEN_OCD_LIBRARIES}       PARENT_SCOPE)
  set(LIBOPEN_OCD_INCLUDE_DIRS ${LIBOPEN_OCD_INCLUDE_DIRS} PARENT_SCOPE)
endif()

# ---------------- Kiss FFT library
#
if (${KISSFFT} MATCHES "ON")
  add_subdirectory(kissfft)
  message("")
  message("================================================================================")
  message(STATUS "External_Libs: KISS_FFT_LIBRARIES:        ${KISS_FFT_LIBRARIES}")
  message(STATUS "External_Libs: LIBKISSFFT_INCLUDE_DIRS:   ${LIBKISSFFT_INCLUDE_DIRS}")
  message("================================================================================")
  message("")
  set(LIBKISSFFT_INCLUDE_DIRS ${LIBKISSFFT_INCLUDE_DIRS} PARENT_SCOPE)
  set(KISS_FFT_LIBRARIES      ${KISS_FFT_LIBRARIES}      PARENT_SCOPE)
endif()

# Unused: external libraries are built separately
#ExternalProject_Add( project_xmlprc
#  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/xmlrpc-c"
#  PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/xmlrpc-c"
#  CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/xmlrpc-c/configure  --prefix=${CMAKE_CURRENT_BINARY_DIR}/xmlrpc-c/
#  BUILD_COMMAND make
#  INSTALL_COMMAND make install
#  BUILD_IN_SOURCE 1
#)
#
#ExternalProject_Get_Property(project_xmlprc install_dir)
#
#
#add_library(xmlprc SHARED IMPORTED)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc++.so)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc_packetsocket.so)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc_client.so)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc_client++.so)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc_server++.so)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc_server_abyss++.so)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc_util.so)
#set_property(TARGET xmlprc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libxmlrpc_util++.so)
#add_dependencies(xmlprc project_xmlprc)
#
#add_definitions(${XMLRPC_CMAKE_INSTALL})
#set (XMLRPC_CMAKE_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/xmlrpc-c/)
#add_definitions(${XMLRPC_CMAKE_INCLUDE_DIRS})
#set (XMLRPC_CMAKE_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/xmlrpc-c/include/)
#message("================================================================================")
#message(STATUS "External_Libs: XMLRPC_CMAKE_INSTALL:            ${XMLRPC_CMAKE_INSTALL}")
#message(STATUS "External_Libs: XMLRPC_CMAKE_INCLUDE_DIRS:       ${XMLRPC_CMAKE_INCLUDE_DIRS}")
#message("================================================================================")
#
