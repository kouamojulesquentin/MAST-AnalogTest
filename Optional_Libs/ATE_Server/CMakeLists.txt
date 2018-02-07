cmake_minimum_required(VERSION 2.6)

message("")
message("================================================================================")
message(STATUS "ATE_Server: USE_XML_RPC:                      ${USE_XML_RPC}")

if (USE_XML_RPC)
  if (WIN32)
    message(FATAL_ERROR "ATE_Server: WIN32 is not supported")
  endif()

  if (NOT XmlRpcInstall)
    message(FATAL_ERROR "ATE_Server: Missing definition for XmlRpcInstall")
  endif()
  message(STATUS "ATE_Server: XmlRpcInstall:                    ${XmlRpcInstall}")
  message("================================================================================")
  message("")
  find_package(LibXmlRpc  REQUIRED)
  include_directories(${LIBXMLRPC_INCLUDE_DIRS})
  include_directories(${LIBMASTXMLRPC_INCLUDE_DIRS})
endif()

message(STATUS "ATE_Server: CMAKE_PREFIX_PATH:                ${CMAKE_PREFIX_PATH}")
message(STATUS "ATE_Server: CMAKE_CXX_COMPILER_ID:            ${CMAKE_CXX_COMPILER_ID}")

include_directories(${LIBMASTRPC_INCLUDE_DIRS})

set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(WARNINGS "-Wall -Wextra -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(FLAGS    "-fPIC -g ${WARNINGS}")

set(ATE_LOOPBACK_SERVER_SOURCES   "ATE_Loopback_Server.cpp")

set(ADVANTEST_TRIVIAL_POC_SOURCES "Advantest_SmartRDI_Protocol.cpp"
                                  "Advantest_TrivialPoc.cpp"
                                  "Advantest_JTAG.cpp"
                                )

message(STATUS "ATE_Server: ATE_LOOPBACK_SERVER_SOURCES:    ${ATE_LOOPBACK_SERVER_SOURCES}")
message(STATUS "ATE_Server: ADVANTEST_TRIVIAL_POC_SOURCES:  ${ADVANTEST_TRIVIAL_POC_SOURCES}")

add_executable        (ATE_Loopback_Server "${ATE_LOOPBACK_SERVER_SOURCES}")
set_target_properties (ATE_Loopback_Server PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_link_libraries (ATE_Loopback_Server Mast_RPC Mast_RPC_Server pthread)

add_executable        (Advantest_Trivial_Poc "${ADVANTEST_TRIVIAL_POC_SOURCES}")
set_target_properties (Advantest_Trivial_Poc PROPERTIES COMPILE_FLAGS "${FLAGS}" )
target_link_libraries (Advantest_Trivial_Poc Mast_RPC Mast_RPC_Server pthread)

message("================================================================================")
message("")

