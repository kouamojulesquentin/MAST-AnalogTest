
add_subdirectory(MastRpc)
set(LIBMASTRPC_INCLUDE_DIRS "${LIBMASTRPC_INCLUDE_DIRS}" PARENT_SCOPE)

message("")
message("================================================================================")
message(STATUS "Optional_Libs: USE_XML_RPC:     ${USE_XML_RPC}")

if (USE_XML_RPC)
  message(STATUS "Optional_Libs: XmlRpcInstall:   ${XmlRpcInstall}")
  if (${XmlRpcInstall} STREQUAL "")
    message(FATAL_ERROR "Optional_Libs: XmlRpcInstall must be defined")
  endif()

  message("================================================================================")
  message("")
  add_subdirectory(MastXmlRpc)
  set(LIBMASTXMLRPC_INCLUDE_DIRS "${LIBMASTXMLRPC_INCLUDE_DIRS}" PARENT_SCOPE)
endif()

add_subdirectory(Examples)


