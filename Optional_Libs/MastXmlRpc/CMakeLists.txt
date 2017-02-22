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

set (CMAKE_CXX_COMPILER "g++")
#+set (CMAKE_C_COMPILER   "gcc")

set(BaseProjectPath "../..")
include_directories(${BaseProjectPath}/Mast_Core/include/)
#+include_directories(${BaseProjectPath}/Logger/)
#+include_directories(${BaseProjectPath}/Logger/g3log/)

#+set_property(GLOBAL PROPERTY CXX_STANDARD 98)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)


if (NOT XmlRpcInstall)
  message(FATAL_ERROR STATUS "MastXmlRpc: Missing definition for XmlRpcInstall")
endif()
message(STATUS "MastXmlRpc: XmlRpcInstall:         ${XmlRpcInstall}")
message("================================================================================")
message("")


find_package(LibXmlRpc  REQUIRED)
include_directories(${LibXmlRpc_INCLUDE_DIRS})

set(WARNINGS      "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
set(CLIENT_FLAGS  "-std=c++03 -fPIC -g ${WARNINGS}")
file(GLOB ClientSources "*.cpp")

#+set(ClientSources XmlRpc_Protocol_Client.cpp)

message("")
message("================================================================================")
message(STATUS "MastXmlRpc: CMAKE_LINK_LIBRARY_FLAG:  ${CMAKE_LINK_LIBRARY_FLAG}")
message(STATUS "MastXmlRpc: CLIENT_FLAGS:             ${CLIENT_FLAGS}")
message(STATUS "MastXmlRpc: ClientSources:            ${ClientSources}")
message(STATUS "MastXmlRpc: LIBXMLRPC_LIBRARIES:      ${LIBXMLRPC_LIBRARIES}")
message("================================================================================")
message("")


add_library           (Mast_RPC_Client SHARED     "${ClientSources}")
set_target_properties (Mast_RPC_Client PROPERTIES COMPILE_FLAGS "${CLIENT_FLAGS}" )
target_link_libraries (Mast_RPC_Client Mast_Core ${LIBXMLRPC_LIBRARIES})

