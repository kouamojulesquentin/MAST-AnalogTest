

message("")
message("================================================================================")


if (USE_XML_RPC)
  message(STATUS "Optional_Libs: Using XML_RPC")
  message(STATUS "Optional_Libs: XmlRpcInstall:   ${XmlRpcInstall}")
  if (${XmlRpcInstall} STREQUAL "")
    message(FATAL_ERROR "Optional_Libs: XmlRpcInstall must be defined")
  endif()
  message("================================================================================")
  message("")
  add_subdirectory(MastXmlRpc)
endif()

