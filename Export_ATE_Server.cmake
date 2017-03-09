# This files describes what should be, minimally exported to build Xml-Rpc server for ATE (using only C++03)

set(ATE_SERVER_EXPORT ${EXPORT_DIR}/ATE_Server)

message("")
message("================================================================================")
message(STATUS "Export_ATE: ATE_SERVER_EXPORT:          ${ATE_SERVER_EXPORT}")
message("================================================================================")
message("")

set(Xmlrpc_Excluded "/xmlrpc-c/(build|tmp|(xmlrpc_config.h|xmlrpc_amconfig.h|xmlrpc-c-config.test|xmlrpc-c-config)$)|(blddir|srcdir)$")
set(Server_Headers Mast_Core/include/Remote_Protocol.hpp
                   Mast_Core/include/Remote_Protocol_Client.hpp
                   Mast_Core/include/Remote_Protocol_Server.hpp)

set(Optionals_Excluded "ATE_Server.cmake|Examples|run_cmake.sh")

install(DIRECTORY   Optional_Libs
        COMPONENT   ATE_Server
        DESTINATION ${ATE_SERVER_EXPORT}
        REGEX       ${Optionals_Excluded} EXCLUDE)

install(DIRECTORY   External_Libs/CMake
        COMPONENT   ATE_Server
        DESTINATION ${ATE_SERVER_EXPORT}/External_Libs)

install(DIRECTORY   External_Libs/xmlrpc-c
        COMPONENT   ATE_Server
        DESTINATION ${ATE_SERVER_EXPORT}/External_Libs
        REGEX       ${Xmlrpc_Excluded} EXCLUDE)

install(FILES       External_Libs/xmlrpc-c/build.sh
        COMPONENT   ATE_Server
        DESTINATION ${ATE_SERVER_EXPORT}/External_Libs/xmlrpc-c
        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE)

install(FILES       ${Server_Headers}
        COMPONENT   ATE_Server
        DESTINATION ${ATE_SERVER_EXPORT}/Mast_Headers)

install(FILES       Optional_Libs/ATE_Server/ATE_Server.cmake
        COMPONENT   ATE_Server
        DESTINATION ${ATE_SERVER_EXPORT}
        RENAME      CMakeLists.txt)

install(FILES       Optional_Libs/ATE_Server/run_cmake.sh
        COMPONENT   ATE_Server
        DESTINATION ${ATE_SERVER_EXPORT}
        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE)





