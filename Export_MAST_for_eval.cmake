# This files describes what should be, minimally exported to execute MAST on an Eval
# running on RHEL5

set(Eval_Build_EXPORT ${EXPORT_DIR}/Eval_Build)

message("")
message("================================================================================")
message(STATUS "Export_Eval: Eval_Build_EXPORT:          ${_Build_EXPORT}")
message(STATUS "Export_Eval: LOCAL_GCC_PATH:          ${LOCAL_GCC_PATH}")

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Lib
        COMPONENT   Eval_Build
        DESTINATION ${Eval_Build_EXPORT})

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Bin/
        COMPONENT   Eval_Build
        DESTINATION ${Eval_Build_EXPORT}
	PATTERN     "*"
	PERMISSIONS OWNER_WRITE OWNER_READ  OWNER_EXECUTE
	            GROUP_READ GROUP_EXECUTE
		    WORLD_READ WORLD_EXECUTE
	)

install(DIRECTORY   ${CMAKE_SOURCE_DIR}/Tutorials/ICL_Tutorial_2
        COMPONENT   Eval_Build
        DESTINATION ${Eval_Build_EXPORT}/ICL_Tutorial_2)
 
 
install(DIRECTORY   ${CMAKE_SOURCE_DIR}/Tutorials/ICL_Tutorial_1
        COMPONENT   Eval_Build
        DESTINATION ${Eval_Build_EXPORT}/ICL_Tutorial_1)
 


get_filename_component(Gxx_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)


 
# install(FILES   ${STD_LIBS_PATH}libgcc_s.so.1 ${STD_LIBS_PATH}libm.so.6
#        COMPONENT   Eval_Build
#        DESTINATION ${Eval_Build_EXPORT}/Std_libs)
# install(FILES   /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#        COMPONENT   Eval_Build
#        DESTINATION ${Eval_Build_EXPORT}/Std_libs)
message(STATUS "Export_Eval: STD_LIBS_PATH:          ${STD_LIBS_PATH}")
message("================================================================================")
message("")


#install(FILES       Optional_Libs/Eval_Build/run_cmake.sh
#        COMPONENT   Eval_Build
#        DESTINATION ${Eval_Build_EXPORT}
#        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ)

