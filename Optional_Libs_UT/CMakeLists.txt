# Optional_Libs_UT

project (Optional_Libs_UT)
message("")
message("================================================================================")

set(UT_WARNINGS ${WARNINGS})
set(UT_CFLAGS "${UT_WARNINGS} -fexceptions -std=c++14 -DCXXTEST_HAVE_EH -DCXXTEST_HAVE_STD -DCXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION")

set(UT_RootPath         "${CMAKE_CURRENT_LIST_DIR}")
set(CXXTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cxxtest")

include_directories(${CXXTEST_INCLUDE_DIR})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(UT_GENERATED_RUNNER   "Generated/Runner.cpp")

set(UT_SOURCES ${UT_GENERATED_RUNNER})
set(EXTRA_LIBS Logger)
message(STATUS "Optional_Libs_UT: CXXTEST_INCLUDE_DIR:        ${CXXTEST_INCLUDE_DIR}")
message(STATUS "Optional_Libs_UT: USE_XML_RPC:                ${USE_XML_RPC}")

# ---------------- XML RPC
#
if (${USE_XML_RPC})
  if (NOT XmlRpcInstall)
    message(FATAL_ERROR "Optional_Libs_UT: Missing definition for XmlRpcInstall")
  endif()
  message(STATUS "Optional_Libs_UT: XmlRpcInstall:              ${XmlRpcInstall}")
  message(STATUS "Optional_Libs_UT: LIBMASTRPC_INCLUDE_DIRS     ${LIBMASTRPC_INCLUDE_DIRS}")
  message(STATUS "Optional_Libs_UT: LIBMASTXMLRPC_INCLUDE_DIRS: ${LIBMASTXMLRPC_INCLUDE_DIRS}")
  find_package(LibXmlRpc  REQUIRED)

  include_directories(${LIBMASTRPC_INCLUDE_DIRS})
  include_directories(${LIBMASTXMLRPC_INCLUDE_DIRS})

  file(GLOB UT_MAST_RPC_HEADERS "UT_MastRpc/UT_*.hpp")
  file(GLOB UT_MAST_RPC         "UT_MastRpc/*")
  file(GLOB UT_XML_RPC_HEADERS  "UT_XmlRpc/UT_*.hpp")
  file(GLOB UT_XML_RPC          "UT_XmlRpc/*")

  set(UT_HEADERS ${UT_HEADERS} ${UT_MAST_RPC_HEADERS} ${UT_XML_RPC_HEADERS})
  set(UT_SOURCES ${UT_SOURCES} ${UT_MAST_RPC}         ${UT_XML_RPC})

  set(EXTRA_LIBS ${EXTRA_LIBS} Mast_RPC Mast_RPC_Client Mast_RPC_Server)
endif()

# ---------------- Runner
#
set(RUNNER_PATH          ${UT_RootPath}/Generated/Runner.cpp)
set(EXTRA_RUNNER_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../UserOptions.cmake ${CMAKE_CURRENT_LIST_FILE})
set(CXXTEST_PATH         ${CXXTEST_INCLUDE_DIR})

message(STATUS "Optional_Libs_UT: UT_HEADERS:                 ${UT_HEADERS}")
include(MakeRunner.cmake)
MakeRunner(${CXXTEST_PATH} ${RUNNER_PATH} "${EXTRA_RUNNER_DEPENDS}" "${UT_HEADERS}")

message("================================================================================")
message("")

if (WIN32)
  set(PLATFORM_LIBS DbgHelp)
else()
  set(PLATFORM_LIBS pthread)
endif()

# ---------------- Runner application
#
add_executable        (Optionals_UT ${UT_SOURCES})
set_target_properties (Optionals_UT PROPERTIES COMPILE_FLAGS "${UT_CFLAGS}" )
target_link_libraries (Optionals_UT ${PLATFORM_LIBS} ${EXTRA_LIBS})

include(CTest)
add_test(NAME Optionals_UT COMMAND Optionals_UT)
