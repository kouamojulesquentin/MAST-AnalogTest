
add_subdirectory(MastRpc)
set(LIBMASTRPC_INCLUDE_DIRS "${LIBMASTRPC_INCLUDE_DIRS}" PARENT_SCOPE)

message("")
message("================================================================================")
message(STATUS "Optional_Libs: USE_XML_RPC:       ${USE_XML_RPC}")
message(STATUS "Optional_Libs: BUILD_ATE_SERVER   ${BUILD_ATE_SERVER}")

if (USE_XML_RPC)
  message(STATUS "Optional_Libs: XmlRpcInstall:     ${XmlRpcInstall}")
  if (${XmlRpcInstall} STREQUAL "")
    message(FATAL_ERROR "Optional_Libs: XmlRpcInstall must be defined")
  endif()

  message("================================================================================")
  message("")
  add_subdirectory(MastXmlRpc)
  set(LIBMASTXMLRPC_INCLUDE_DIRS "${LIBMASTXMLRPC_INCLUDE_DIRS}" PARENT_SCOPE)

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ATE_Server AND DEFINED BUILD_ATE_SERVER AND (${BUILD_ATE_SERVER} MATCHES "TRUE|ON"))
    add_subdirectory(ATE_Server)
  endif()
endif()


if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Examples)
  add_subdirectory(Examples)
endif()


