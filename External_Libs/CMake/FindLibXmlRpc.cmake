# - Try to find LibXmlRpc
# Once done this will define
#  LIBXMLRPC_FOUND        - System has LibXmlRpc
#  LIBXMLRPC_INCLUDE_DIRS - The LibXmlRpc include directories
#  LIBXMLRPC_LIBRARIES    - The libraries needed to use LibXmlRpc

set(LIBXMLRPC_FOUND FALSE)

message("")
message("================================================================================")
message(STATUS "FindLibXmlRpc: XmlRpcInstall:           ${XmlRpcInstall}")

find_path(LIBXMLRPC_INCLUDE_DIR xmlrpc.h HINTS ${XmlRpcInstall}/include )

find_library(LIBXMLRPC_LIBRARY                        NAMES xmlrpc                libxmlrpc                HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_PLUS_PLUS_LIBRARY              NAMES xmlrpc++              libxmlrpc++              HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_PACKET_SOCKET_LIBRARY          NAMES xmlrpc_packetsocket   libxmlrpc_packetsocket   HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_CLIENT_LIBRARY                 NAMES xmlrpc_client         libxmlrpc_client         HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_CLIENT_PLUS_PLUS_LIBRARY       NAMES xmlrpc_client++       libxmlrpc_client++       HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_SERVER_PLUS_PLUS_LIBRARY       NAMES xmlrpc_server++       libxmlrpc_server++       HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_SERVER_ABYSS_PLUS_PLUS_LIBRARY NAMES xmlrpc_server_abyss++ libxmlrpc_server_abyss++ HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_UTIL_LIBRARY                   NAMES xmlrpc_util           libxmlrpc_util           HINTS ${XmlRpcInstall}/lib)
find_library(LIBXMLRPC_UTIL_PLUS_PLUS_LIBRARY         NAMES xmlrpc_util++         libxmlrpc_util++         HINTS ${XmlRpcInstall}/lib)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXMLRPC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibXmlRpc  DEFAULT_MSG LIBXMLRPC_INCLUDE_DIR
                                                         LIBXMLRPC_LIBRARY
                                                         LIBXMLRPC_PLUS_PLUS_LIBRARY
                                                         LIBXMLRPC_PACKET_SOCKET_LIBRARY
                                                         LIBXMLRPC_CLIENT_LIBRARY
                                                         LIBXMLRPC_CLIENT_PLUS_PLUS_LIBRARY
                                                         LIBXMLRPC_SERVER_PLUS_PLUS_LIBRARY
                                                         LIBXMLRPC_SERVER_ABYSS_PLUS_PLUS_LIBRARY
                                                         LIBXMLRPC_UTIL_LIBRARY
                                                         LIBXMLRPC_UTIL_PLUS_PLUS_LIBRARY
)

mark_as_advanced(LIBXMLRPC_INCLUDE_DIR LIBXMLRPC_LIBRARY)
set(LIBXMLRPC_INCLUDE_DIRS  ${LIBXMLRPC_INCLUDE_DIR})

set(LIBXMLRPC_LIBRARIES     ${LIBXMLRPC_LIBRARY}
                            ${LIBXMLRPC_PLUS_PLUS_LIBRARY}
                            ${LIBXMLRPC_PACKET_SOCKET_LIBRARY}
                            ${LIBXMLRPC_CLIENT_LIBRARY}
                            ${LIBXMLRPC_CLIENT_PLUS_PLUS_LIBRARY}
                            ${LIBXMLRPC_SERVER_PLUS_PLUS_LIBRARY}
                            ${LIBXMLRPC_SERVER_ABYSS_PLUS_PLUS_LIBRARY}
                            ${LIBXMLRPC_UTIL_LIBRARY}
                            ${LIBXMLRPC_UTIL_PLUS_PLUS_LIBRARY}
)



message(STATUS "FindLibXmlRpc: LIBXMLRPC_LIBRARIES:     ${LIBXMLRPC_LIBRARIES}")
message(STATUS "FindLibXmlRpc: LIBXMLRPC_INCLUDE_DIRS:  ${LIBXMLRPC_INCLUDE_DIRS}")
message(STATUS "FindLibXmlRpc: LIBXMLRPC_FOUND:         ${LIBXMLRPC_FOUND}")
message("================================================================================")
message("")
