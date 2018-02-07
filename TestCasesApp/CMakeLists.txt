# Test cases application

cmake_minimum_required(VERSION 3.5)
project(TestCasesApp)

message("")
message("================================================================================")
message(STATUS "TestCasesApp: LIBMASTSPI_INCLUDE_DIRS:  ${LIBMASTSPI_INCLUDE_DIRS}")

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

include_directories(${LIBMAST_CPP_API_INCLUDE_DIRS})
include_directories(${LIBMAST_C_API_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})
include_directories(${LIBMASTSPI_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

if (WIN32)
  message(STATUS "TestCasesApp: kissfft not used on Windows!!!")
  message("================================================================================")
else()
  include_directories(${LIBKISSFFT_INCLUDE_DIRS})
  set(EXTRA_LIBS ${EXTRA_LIBS} ${KISS_FFT_LIBRARIES})
endif()
message("================================================================================")

if (${USE_OPEN_OCD})
  message("")
  message("================================================================================")
  message(STATUS "TestCasesApp: OPEN_OCD_LIBRARIES:           ${OPEN_OCD_LIBRARIES}")
  message(STATUS "TestCasesApp: LIBOPEN_OCD_INCLUDE_DIRS:     ${LIBOPEN_OCD_INCLUDE_DIRS}")
  message(STATUS "TestCasesApp: LIBMASTOPEN_OCD_INCLUDE_DIRS: ${LIBMASTOPEN_OCD_INCLUDE_DIRS}")
  message("================================================================================")
  message("")

  include_directories(SYSTEM ${LIBOPEN_OCD_INCLUDE_DIRS})
  include_directories(SYSTEM ${LIBMASTOPEN_OCD_INCLUDE_DIRS})
  set(EXTRA_LIBS ${EXTRA_LIBS} ${OPEN_OCD_LIBRARIES} ${LIBMASTOPEN_OCD_LIBRARIES})
endif()

if (WIN32)
  include_directories($ENV{FLEX_DIR})
  set (APP_SOURCES "main.cpp" "Options.cpp" "Zybo.cpp")
else()
  file(GLOB APP_SOURCES "*")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../Bin")
add_executable          (TestCasesApp ${APP_SOURCES})
set_target_properties   (TestCasesApp PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

if (WIN32)
  set(EXTRA_LIBS ${EXTRA_LIBS} DbgHelp)
else()
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${LIBMASTSPI_LIBRARIES} ${KISS_FFT_LIBRARIES})
endif()

target_link_libraries(TestCasesApp Mast_API_CPP Mast_API_C Logger SIT_Reader ${EXTRA_LIBS})
