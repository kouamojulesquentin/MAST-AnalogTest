# This files describes what should be, minimally exported to execute MAST on an ATE
# running on RHEL5

set(ATE_Build_EXPORT ${EXPORT_DIR}/ATE_Build)

message("")
message("================================================================================")
message(STATUS "Export_ATE: ATE_Build_EXPORT:          ${ATE_Build_EXPORT}")
message(STATUS "Export_ATE: LOCAL_GCC_PATH:          ${LOCAL_GCC_PATH}")

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Lib
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT})

install(DIRECTORY   ${CMAKE_SOURCE_DIR}/External_Libs/xmlrpc-c/build/lib/
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT}/External_Libs/xmlrpc-c/)

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Bin/
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT}/Bin
	PATTERN     "*"
	PERMISSIONS OWNER_WRITE OWNER_READ  OWNER_EXECUTE
	            GROUP_READ GROUP_EXECUTE
		    WORLD_READ WORLD_EXECUTE
	)

install(FILES   ${CMAKE_SOURCE_DIR}/Run.sh
        COMPONENT   ATE_Build
	PERMISSIONS OWNER_WRITE OWNER_READ  OWNER_EXECUTE
	            GROUP_READ GROUP_EXECUTE
		    WORLD_READ WORLD_EXECUTE
        DESTINATION ${ATE_Build_EXPORT})

install(DIRECTORY   ${CMAKE_SOURCE_DIR}/MastExample_CPP/SIT/
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT}/SIT/)
 


get_filename_component(Gxx_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)


if (EXISTS ${LOCAL_GCC_PATH})
 set (STD_LIBS_PATH ${LOCAL_GCC_PATH}/../lib64/)
 message(STATUS "Export_ATE: Setting local STD_LIBS_PATH")
install(FILES   ${STD_LIBS_PATH}libgcc_s.so.1 ${STD_LIBS_PATH}libstdc++.so.6 ${STD_LIBS_PATH}libstdc++.so.6.0.20
        COMPONENT   ATE_Build
        DESTINATION ${ATE_Build_EXPORT}/Std_libs)
else()
 message(STATUS "Export_ATE: using global GCC/G++, no libraries are exported")
# install(FILES   ${STD_LIBS_PATH}libgcc_s.so.1 ${STD_LIBS_PATH}libm.so.6
#        COMPONENT   ATE_Build
#        DESTINATION ${ATE_Build_EXPORT}/Std_libs)
# install(FILES   /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#        COMPONENT   ATE_Build
#        DESTINATION ${ATE_Build_EXPORT}/Std_libs)
endif()
#message(STATUS "Export_ATE: STD_LIBS_PATH:          ${STD_LIBS_PATH}")
message("================================================================================")
message("")


#install(FILES       Optional_Libs/ATE_Build/run_cmake.sh
#        COMPONENT   ATE_Build
#        DESTINATION ${ATE_Build_EXPORT}
#        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ)

