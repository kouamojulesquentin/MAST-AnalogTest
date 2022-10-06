cmake_minimum_required(VERSION 3.5)
project(PDL_Applications)


add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PDL_Applications")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(GLOB PDL_Application_SOURCES "src/JTAG_BitBang_TranslatorProtocol.*")
set (LDO_Application_SOURCES "src/LDO.cpp")

set (ml505_kissfft_SOURCES "src/ml505_kissfft.cpp")

set(PDL_APPLICATIONS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(LIB_PDL_APPLICATIONS_INCLUDE_DIRS  "${CMAKE_CURRENT_SOURCE_DIR}/include/" CACHE INTERNAL "" FORCE )
include_directories(${PDL_APPLICATIONS_INCLUDE_DIRS})

message("")
message("================================================================================")
message(STATUS "PDL_Applications: LIBMAST_CORE_INCLUDE_DIRS:       ${LIBMAST_CORE_INCLUDE_DIRS}")
message(STATUS "PDL_Applications: PDL_APPLICATIONS_INCLUDE_DIRS:       ${PDL_APPLICATIONS_INCLUDE_DIRS}")
message(STATUS "PDL_Applications: LIB_PDL_APPLICATIONS_INCLUDE_DIRS:     ${LIB_PDL_APPLICATIONS_INCLUDE_DIRS}") 

if (WIN32)
  message(STATUS "PDL_Applications: kissfft not used on Windows!!!")
  message("================================================================================")
else()
  include_directories(${LIBKISSFFT_INCLUDE_DIRS})
  set(EXTRA_LIBS ${EXTRA_LIBS} ${KISS_FFT_LIBRARIES})
endif()
message("================================================================================")

message("================================================================================")
message("")

add_library           (PDL_Applications SHARED   ${PDL_Application_SOURCES})
target_link_libraries (PDL_Applications Mast_API_CPP_PDL)
# target_link_libraries (PDL_Applications Mast_API_CPP)

add_library           (LDO SHARED   ${LDO_Application_SOURCES})
target_link_libraries (LDO Mast_API_CPP_PDL)

add_library           (ML505_KISSFFT SHARED   ${ml505_kissfft_SOURCES})
target_link_libraries (ML505_KISSFFT Mast_API_CPP_PDL  ${KISS_FFT_LIBRARIES})
