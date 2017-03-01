cmake_minimum_required(VERSION 2.6)
project(MastXmlRpc)

message(STATUS "")
message("================================================================================")
message(STATUS "MastXmlRpc: CMAKE_MODULE_PATH:     ${CMAKE_MODULE_PATH}")
message(STATUS "MastXmlRpc: CMAKE_PREFIX_PATH:     ${CMAKE_PREFIX_PATH}")
message(STATUS "MastXmlRpc: CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

if (WIN32)
  message(FATAL_ERROR "MastXmlRpc: WIN32 is not supported yet for XmlRpc")
endif()

set (CMAKE_CXX_COMPILER "g++")

set(BaseProjectPath "../..")


set_property(GLOBAL PROPERTY CXX_STANDARD          03)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)


if (NOT XmlRpcInstall)
  message(FATAL_ERROR "MastXmlRpc: Missing definition for XmlRpcInstall")
endif()
message(STATUS "MastXmlRpc: XmlRpcInstall:         ${XmlRpcInstall}")
message("================================================================================")
message("")
find_package(LibXmlRpc  REQUIRED)


set(WARNINGS     "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(CLIENT_FLAGS "-std=c++03 -fPIC -g ${WARNINGS} -Wno-long-long")
set(SERVER_FLAGS "${CLIENT_FLAGS}")


set(ClientSources XmlRpc_Protocol_Common.hpp
                  XmlRpc_Protocol_Client.cpp
                  XmlRpc_Protocol_Client.hpp)

set(ServerSources XmlRpc_Protocol_Common.hpp
                  XmlRpc_Protocol_Server.cpp
                  XmlRpc_Protocol_Server.hpp)

include_directories(${LIBXMLRPC_INCLUDE_DIRS})
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})

set(LIBMASTXMLRPC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/." PARENT_SCOPE)
message("")
message("================================================================================")
message(STATUS "MastXmlRpc: CMAKE_LINK_LIBRARY_FLAG:  ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MastXmlRpc: CLIENT_FLAGS:             ${CLIENT_FLAGS}")
message(STATUS "MastXmlRpc: ClientSources:            ${ClientSources}")
message(STATUS "MastXmlRpc: SERVER_FLAGS:             ${SERVER_FLAGS}")
message(STATUS "MastXmlRpc: ServerSources:            ${ServerSources}")
message(STATUS "MastXmlRpc: ServerSources:            ${ServerSources}")
message(STATUS "MastXmlRpc: LIBXMLRPC_INCLUDE_DIRS:   ${LIBXMLRPC_INCLUDE_DIRS}")
message("================================================================================")
message("")


add_library           (Mast_RPC_Client SHARED     "${ClientSources}")
set_target_properties (Mast_RPC_Client PROPERTIES COMPILE_FLAGS "${CLIENT_FLAGS}" )
target_link_libraries (Mast_RPC_Client ${LIBXMLRPC_LIBRARIES})

add_library           (Mast_RPC_Server SHARED     "${ServerSources}")
set_target_properties (Mast_RPC_Server PROPERTIES COMPILE_FLAGS "${SERVER_FLAGS}" )
target_link_libraries (Mast_RPC_Server ${LIBXMLRPC_LIBRARIES})
