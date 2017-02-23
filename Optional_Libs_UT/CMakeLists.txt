# Optional_Libs_UT

project (Optional_Libs_UT)
message("")
message("================================================================================")

#+add_definitions(${UT_WARNINGS})
set (UT_WARNINGS ${WARNINGS})

#+add_definitions(${UT_CFLAGS})
set(UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")

set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

message(STATUS "Optional_Libs_UT: USE_XML_RPC:          ${USE_XML_RPC}")
message(STATUS "Optional_Libs_UT: CXXTEST_INCLUDE_DIR:  ${CXXTEST_INCLUDE_DIR}")
message("================================================================================")
message("")

file(GLOB UT_HEADERS      "./*/UT_*.hpp")
add_custom_target(Optional_Libs_Runner echo "Building Optional_Libs_UT runner")
add_custom_command (OUTPUT    ${UT_RootPath}/Generated/Runner.cpp
                    DEPENDS   ${UT_RootPath}/Runner.mak ${UT_HEADERS}
										COMMAND 	make -C ${UT_RootPath} -f Runner.mak
										)

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${LibXmlRpc_INCLUDE_DIRS})
include_directories(../Optional_Libs/MastRpc/)
include_directories(../Optional_Libs/MastXmlRpc/)
include_directories(../Mast_Core/include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)
#+include_directories(${UT_RootPath}/CxxTest_Traits)
#+include_directories(${UT_RootPath}/UT_Helpers/)

file(GLOB UT_MAST_RPC     "UT_MastRpc/*")
file(GLOB UT_XML_RPC      "UT_XmlRpc/*")
set(UT_GENERATED_RUNNER   "Generated/Runner.cpp")

set(UT_SOURCES ${UT_GENERATED_RUNNER} ${UT_MAST_RPC} ${UT_XML_RPC} )

set_source_files_properties(${UT_GENERATED_RUNNER} GENERATED)

#+set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable        (Optionals_UT ${UT_SOURCES})
set_target_properties (Optionals_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )

if (WIN32)
  target_link_libraries(Optionals_UT Mast_RPC Mast_RPC_Client Mast_RPC_Server Logger DbgHelp )
else ()
  target_link_libraries(Optionals_UT Mast_RPC Mast_RPC_Client Mast_RPC_Server Logger pthread)
endif ()

