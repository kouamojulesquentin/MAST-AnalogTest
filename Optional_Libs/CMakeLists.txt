

message("")
message("================================================================================")
message(STATUS "Optional_Libs: USE_LIBFTDISPI:    ${USE_LIBFTDISPI}")
message(STATUS "Optional_Libs: USE_XML_RPC:       ${USE_XML_RPC}")
message(STATUS "Optional_Libs: USE_OPEN_OCD:      ${USE_OPEN_OCD}")
message(STATUS "Optional_Libs: INTEL_EXPERIMENT   ${INTEL_EXPERIMENT}")
message(STATUS "Optional_Libs: BUILD_ATE_SERVER   ${BUILD_ATE_SERVER}")

if (USE_XML_RPC)
  message(STATUS "Optional_Libs: XmlRpcInstall:     ${XmlRpcInstall}")
  if (${XmlRpcInstall} STREQUAL "")
    message(FATAL_ERROR "Optional_Libs: XmlRpcInstall must be defined")
  endif()

  message("================================================================================")
  message("")

  add_subdirectory(MastRpc)
  set(LIBMASTRPC_INCLUDE_DIRS "${LIBMASTRPC_INCLUDE_DIRS}" PARENT_SCOPE)

  add_subdirectory(MastXmlRpc)
  set(LIBMASTXMLRPC_INCLUDE_DIRS "${LIBMASTXMLRPC_INCLUDE_DIRS}" PARENT_SCOPE)

  add_subdirectory(MastRpcTranslator)
 set(LIBMASTRPC_TRANS_INCLUDE_DIRS "${LIBMASTRPC_TRANS_INCLUDE_DIRS}" PARENT_SCOPE)

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ATE_Server AND DEFINED BUILD_ATE_SERVER AND (${BUILD_ATE_SERVER} MATCHES "TRUE|ON"))
    add_subdirectory(ATE_Server)
  endif()
endif()

# ---------------- FTDI-SPI library
#
if(${USE_LIBFTDISPI})
  add_subdirectory (SPI)
  set(LIBMASTSPI_INCLUDE_DIRS ${LIBMASTSPI_INCLUDE_DIRS} PARENT_SCOPE)
  set(LIBMASTSPI_LIBRARIES    ${LIBMASTSPI_LIBRARIES}    PARENT_SCOPE)
endif()

if (${USE_OPEN_OCD})
  add_subdirectory (OpenOCD)
  set(LIBMASTOPEN_OCD_INCLUDE_DIRS ${LIBMASTOPEN_OCD_INCLUDE_DIRS} PARENT_SCOPE)
  set(LIBMASTOPEN_OCD_LIBRARIES    ${LIBMASTOPEN_OCD_LIBRARIES}    PARENT_SCOPE)
  message("================================================================================")
  message(STATUS "Optional_Libs: LIBMASTOPEN_OCD_LIBRARIES:     ${LIBMASTOPEN_OCD_LIBRARIES}")
  message(STATUS "Optional_Libs: LIBMASTOPEN_OCD_INCLUDE_DIRS:  ${LIBMASTOPEN_OCD_INCLUDE_DIRS}")
  message("================================================================================")
endif()

# ---------------- Intel library
#
if(${INTEL_EXPERIMENT})
  add_subdirectory (Intel)
  set(INTEL_EXP_LIBRARIES   ${INTEL_EXP_LIBRARIES}   PARENT_SCOPE)
  message("================================================================================")
  message(STATUS "Optional_Libs: INTEL_EXP_LIBRARIES:           ${INTEL_EXP_LIBRARIES}")
  message("================================================================================")
endif()


# ---------------- Examples
#
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Examples)
  add_subdirectory(Examples)
endif()


add_subdirectory(Trivium)
add_subdirectory(Empty_TranslatorProtocol)
add_subdirectory(Dummy_PathSelector)
add_subdirectory(SSAK)
add_subdirectory(DummyStreamerProtocol)
#add_subdirectory(NXP)
add_subdirectory(Low_Pin)

