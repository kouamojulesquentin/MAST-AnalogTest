cmake_minimum_required(VERSION 2.6)
project(MastXmlRpc)

message(STATUS "")
message("================================================================================")
message(STATUS "MastXmlRpc: CMAKE_MODULE_PATH:     ${CMAKE_MODULE_PATH}")
message(STATUS "MastXmlRpc: CMAKE_PREFIX_PATH:     ${CMAKE_PREFIX_PATH}")
message(STATUS "MastXmlRpc: CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

if (WIN32)
  message(FATAL_ERROR STATUS "MastXmlRpc: WIN32 is not supported for XplRPC")
endif()

#+set (CMAKE_CXX_COMPILER "g++")
#+set (CMAKE_C_COMPILER   "gcc")



set_property(GLOBAL PROPERTY CXX_STANDARD 98)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

#+    xmlrpc
#+    xmlrpc_abyss
#+    xmlrpc_abyss++
#+    xmlrpc_client
#+    xmlrpc_cpp
#+    xmlrpc_packetsocket
#+    xmlrpc_server
#+    xmlrpc_server_abyss
#+    xmlrpc_server_pstream+  +
#+    xmlrpc_util
#+    xmlrpc_xmlparse
#+    xmlrpc_xmltok

if (NOT XmlRpcInstall)
  message(FATAL_ERROR STATUS "MastXmlRpc: Missing definition for XmlRpcInstall")
endif()
message(STATUS "MastXmlRpc: XmlRpcInstall:         ${XmlRpcInstall}")
message("================================================================================")
message("")


find_package(LibXmlRpc  REQUIRED)
include_directories(${LibXmlRpc_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LibXmlRpc_LIBRARIES})

set(ClientSources XmlRpc_Protocol_Client.cpp)

add_library           (Mast_RPC_Client SHARED     ${ClientSources})
set_target_properties (Mast_RPC_Client PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (Mast_RPC_Client ${LIBXMLRPC_LIBRARIES})

