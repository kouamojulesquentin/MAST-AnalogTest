cmake_minimum_required(VERSION 3.5)
project(Mast)
enable_testing()

message("")
message("================================================================================")
if (WIN32)
  if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
    message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
  endif()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/External_Libs/CMake ${CMAKE_MODULE_PATH})

add_definitions(${WARNINGS})
set (WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
add_definitions(${CFLAGS})
set (CFLAGS "${WARNINGS} -std=c++14 -g")

if (WIN32)
  set (CFLAGS ${CFLAGS})
else()
  set (CFLAGS "${CFLAGS} -fPIC")
endif()

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Lib")
set(EXPORT_DIR                     ".")

if (WIN32)
  set(USE_OPEN_OCD     OFF)
  set(USE_LIBFTDISPI   OFF)
  set(BUILD_ATE_SERVER OFF)
  set(INTEL_EXPERIMENT OFF)
  set(KISSFFT          OFF)
else()
  set(USE_OPEN_OCD     ON)
  set(USE_LIBFTDISPI   ON)
  set(BUILD_ATE_SERVER ON)
  set(INTEL_EXPERIMENT ON)
  set(KISSFFT          ON)
endif()

message(STATUS "")
message(STATUS "Master: User specific values for build options can be overridden in UserOptions.cmake")
message(STATUS "Master: See model file: UserOptions.model.cmake")

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/UserOptions.cmake)
  message("STATUS")
  include(${CMAKE_CURRENT_LIST_DIR}/UserOptions.cmake)
endif()

message(STATUS "")
message(STATUS "Master: USE_OPEN_OCD:       ${USE_OPEN_OCD}")
message(STATUS "Master: USE_LIBFTDISPI:     ${USE_LIBFTDISPI}")
message(STATUS "Master: BUILD_ATE_SERVER:   ${BUILD_ATE_SERVER}")
message(STATUS "Master: USE_XML_RPC:        ${USE_XML_RPC}")
message(STATUS "Master: INTEL_EXPERIMENT:   ${INTEL_EXPERIMENT}")
message(STATUS "Master: KISSFFT:            ${KISSFFT}")

configure_file      ("${PROJECT_SOURCE_DIR}/MastConfig.hpp.in" "${PROJECT_BINARY_DIR}/MastConfig.hpp")
include_directories ("${PROJECT_BINARY_DIR}")

message(STATUS "")
message(STATUS "Master: CMAKE_MODULE_PATH:              ${CMAKE_MODULE_PATH}")
message(STATUS "Master: CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Master: CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "Master: EXPORT_DIR:                     ${EXPORT_DIR}")


if (USE_XML_RPC)
  set(XmlRpcInstall ${CMAKE_CURRENT_LIST_DIR}/External_Libs/xmlrpc-c/build)

  message(STATUS "Master: XmlRpcInstall:                  ${XmlRpcInstall}")
endif()

message("================================================================================")
message("")

add_subdirectory(External_Libs)
add_subdirectory(Logger)
add_subdirectory(Mast_Core)
add_subdirectory(SIT_reader)
add_subdirectory(Mast)

add_subdirectory(Optional_Libs)

if (NOT WIN32)
add_subdirectory (License)
endif()

if (${KISSFFT})
  add_subdirectory (TestCasesApp)
endif()

add_subdirectory (MastExample_CPP)
add_subdirectory (Mast_UT)
add_subdirectory (SIT_reader_UT)

if (USE_XML_RPC)
  add_subdirectory(Optional_Libs_UT)
  include(Export_ATE_Server.cmake)
  include(Export_MAST_for_ATE.cmake)
endif()

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY  OFF)
set(CPACK_COMPONENTS_GROUPING         ONE_PER_GROUP)
set(CPACK_COMPONENTS_ALL              ATE_Server ATE_Build) #SIT_Files

set(CPACK_PACKAGE_VENDOR              "OWL")
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "${CMAKE_BINARY_DIR}/${EXPORT_DIR}")
set(CPACK_PACKAGE_NAME                "MAST")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Files to build an ATE server on system with only C++03")
set(CPACK_PACKAGE_VERSION_MAJOR       "1")
set(CPACK_PACKAGE_VERSION_MINOR       "0")
set(CPACK_PACKAGE_VERSION_PATCH       "0")
set(CPACK_PACKAGE_VERSION             "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_SYSTEM_NAME                 "") # To prevent "-Linux" in file name

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
include(CPack)

