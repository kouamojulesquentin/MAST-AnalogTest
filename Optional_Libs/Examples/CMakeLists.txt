cmake_minimum_required(VERSION 2.6)
project(OptionalExamples)

message("")
message("================================================================================")
message(STATUS "Optionals_Examples: USE_XML_RPC:              ${USE_XML_RPC}")

if (USE_XML_RPC)
  if (WIN32)
    message(FATAL_ERROR "Optionals_Examples: WIN32 is not supported for RPC")
  endif()

  if (NOT XmlRpcInstall)
    message(FATAL_ERROR "Optionals_Examples: Missing definition for XmlRpcInstall")
  endif()
  message(STATUS "Optionals_Examples: XmlRpcInstall:          ${XmlRpcInstall}")
  message("================================================================================")
  message("")
  find_package(LibXmlRpc  REQUIRED)

  message(STATUS "Optionals_Examples: CMAKE_PREFIX_PATH:      ${CMAKE_PREFIX_PATH}")
  message(STATUS "Optionals_Examples: CMAKE_CXX_COMPILER_ID:  ${CMAKE_CXX_COMPILER_ID}")

  include_directories(${LIBXMLRPC_INCLUDE_DIRS})
  include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
  include_directories(${LIBMASTRPC_INCLUDE_DIRS})
  include_directories(${LIBMASTXMLRPC_INCLUDE_DIRS})

  set_property(GLOBAL PROPERTY CXX_STANDARD          03)
  set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

#+  set(WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
#+  set(FLAGS    "-std=c++03 -fPIC -g ${WARNINGS}")

  set(XML_RPC_LOOPBACK_SERVER_SOURCES  "./XmlRpc_Loopback_Server.cpp")
  message(STATUS "Optionals_Examples: XML_RPC_LOOPBACK_SERVER_SOURCES: ${XML_RPC_LOOPBACK_SERVER_SOURCES}")

  add_executable        (XmlRpc_Loopback_Server "${XML_RPC_LOOPBACK_SERVER_SOURCES}")
  set_target_properties (XmlRpc_Loopback_Server PROPERTIES COMPILE_FLAGS "${FLAGS}" )
  target_link_libraries (XmlRpc_Loopback_Server Mast_RPC Mast_RPC_Server pthread)
endif()

message("================================================================================")
message("")

