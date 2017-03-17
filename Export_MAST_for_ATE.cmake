# This files describes what should be, minimally exported to build Xml-Rpc server for ATE (using only C++03)

set(ATE_Build_EXPORT ${EXPORT_DIR}/ATE_Build)

message("")
message("================================================================================")
message(STATUS "Export_ATE: ATE_Build_EXPORT:          ${ATE_Build_EXPORT}")
message("================================================================================")
message("")

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Lib
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT})

install(DIRECTORY   ${CMAKE_BINARY_DIR}/External_Libs/
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT})

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Bin/
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT}/Bin)

get_filename_component(Gxx_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)


if (DEFINED ${LOCAL_GCC_PATH})
 set (STD_LIBS_PATH ${Gxx_PATH}/../lib)
 install(FILES   ${STD_LIBS_PATH}libgcc_s.so.1 ${STD_LIBS_PATH}libm.so.6
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT}/Std_libs)
 install(FILES   /usr/lib/x86_64-linux-gnu/libstdc++.so.6
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT}/Std_libs)
else()

endif()


message("")
message("================================================================================")
message(STATUS "Export_ATE: G++_PATH:          ${Gxx_PATH}")
message("================================================================================")
message("")
  
#install(FILES       Optional_Libs/ATE_Build/run_cmake.sh
#        COMPONENT   ATE_Build
#        DESTINATION ${ATE_Build_EXPORT}
#        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ)

