# This files describes what should be, minimally exported to execute MAST on an RiscV processor running Linux

set(RISCV_Build_EXPORT ${EXPORT_DIR}/RISCV_Build)

message("")
message("================================================================================")
message(STATUS "Export_RISCV: RISCV_Build_EXPORT:          ${RISCV_Build_EXPORT}")

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Lib
        COMPONENT   RISCV_Build
        DESTINATION ${RISCV_Build_EXPORT})

install(DIRECTORY   ${CMAKE_BINARY_DIR}/Bin/
        COMPONENT   RISCV_Build
        DESTINATION ${RISCV_Build_EXPORT}/Bin
	PATTERN     "*"
	PERMISSIONS OWNER_WRITE OWNER_READ  OWNER_EXECUTE
	            GROUP_READ GROUP_EXECUTE
		    WORLD_READ WORLD_EXECUTE
	)


install(DIRECTORY   ${CMAKE_SOURCE_DIR}/Examples/
        COMPONENT   RISCV_Build
        DESTINATION ${RISCV_Build_EXPORT}/Examples/)
 


#message(STATUS "Export_RISCV: STD_LIBS_PATH:          ${STD_LIBS_PATH}")
message("================================================================================")
message("")


