# This files describes what should be, minimally exported to execute MAST on an ARM


set(ARM_Build_EXPORT ${EXPORT_DIR}/ARM_Build)

message("")
message("================================================================================")
message(STATUS "Export_ARM: ARM_Build_EXPORT:          ${ARM_Build_EXPORT}")

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Lib
        COMPONENT   ARM_Build
        DESTINATION ${ARM_Build_EXPORT})

#install(DIRECTORY   ${CMAKE_SOURCE_DIR}/External_Libs/xmlrpc-c/build/lib/
#        COMPONENT   ARM_Build
#        DESTINATION ${ARM_Build_EXPORT}/External_Libs/xmlrpc-c/)

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Bin/
        COMPONENT   ARM_Build
        DESTINATION ${ARM_Build_EXPORT}/Bin
	PATTERN     "*"
	PERMISSIONS OWNER_WRITE OWNER_READ  OWNER_EXECUTE
	            GROUP_READ GROUP_EXECUTE
		    WORLD_READ WORLD_EXECUTE
	)


install(DIRECTORY   ${CMAKE_SOURCE_DIR}/Examples/
        COMPONENT   ARM_Build
        DESTINATION ${ARM_Build_EXPORT}/Examples/)
 

get_filename_component(Gxx_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)

#These commands can be used to add specific local libraries to the Installation
#They can be useful when the target system libc is older than the one used when cross-compiling
#install(FILES   ${CMAKE_SOURCE_DIR}/Run.sh
#        COMPONENT   ARM_Build
#	PERMISSIONS OWNER_WRITE OWNER_READ  OWNER_EXECUTE
#	            GROUP_READ GROUP_EXECUTE
#		    WORLD_READ WORLD_EXECUTE
#        DESTINATION ${ARM_Build_EXPORT})
#SET(CMAKE_ROOT_PATH /opt/Xilinx/petalinux/tools/linux-i386/gcc-arm-linux-gnueabi/bin)
#set (STD_LIBS_PATH "${CMAKE_ROOT_PATH}/../arm-linux-gnueabihf/libc/lib/")
#message(STATUS "Export_ARM: CMAKE_ROOT_PATH  set to ${CMAKE_ROOT_PATH}")
#message(STATUS "Export_ARM: STD_LIBS_PATH  set to ${STD_LIBS_PATH}")
#install(FILES   ${STD_LIBS_PATH}libgcc_s.so.1 ${STD_LIBS_PATH}libstdc++.so.6.0.22 
#        COMPONENT   ARM_Build
#        DESTINATION ${ARM_Build_EXPORT}/Std_libs)

#INSTALL(CODE "execute_process( \
#    COMMAND ${CMAKE_COMMAND} -E create_symlink -r\
#    ${CMAKE_INSTALL_PREFIX}/${ARM_Build_EXPORT}/Std_libs/libstdc++.so.6.0.22 \
#    ${CMAKE_INSTALL_PREFIX}/${ARM_Build_EXPORT}/Std_libs/libstdc++.so.6   \
#    )"
#)
#Manually create a simlink to be used in dynamic library linking (i.e. with LD_LIBRARY_PATH)
#execute_process(COMMAND ln -rs ${CMAKE_INSTALL_PREFIX}/${ARM_Build_EXPORT}/Std_libs/libstdc++.so.6.0.22  ${CMAKE_INSTALL_PREFIX}/${ARM_Build_EXPORT}/Std_libs/libstdc++.so.6)
#install(FILES   ${CMAKE_INSTALL_PREFIX}/${ARM_Build_EXPORT}/Std_libs/libstdc++.so.6
#        COMPONENT   ARM_Build
#        DESTINATION ${ARM_Build_EXPORT}/Std_libs)

#message(STATUS "Export_ARM: STD_LIBS_PATH:          ${STD_LIBS_PATH}")
message("================================================================================")
message("")


