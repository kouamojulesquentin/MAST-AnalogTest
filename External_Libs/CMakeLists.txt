
if (NOT XmlRpcInstall)
  message(STATUS "External_Libs: Setting XmlRpcInstall")
  set(XmlRpcInstall ${CMAKE_CURRENT_LIST_DIR}/xmlrpc-c)
endif()

message("")
message("================================================================================")
message(STATUS "External_Libs: XmlRpcInstall:            ${XmlRpcInstall}")
message("================================================================================")
message("")

