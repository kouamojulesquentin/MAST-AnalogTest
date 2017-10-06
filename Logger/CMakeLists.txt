project(mast_logger)

include_directories(./)
include_directories(g3log/)
include_directories(Unix/)
include_directories(Windows/)
set(LIBLOGGER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/g3log" "${CMAKE_CURRENT_SOURCE_DIR}/." PARENT_SCOPE)

add_definitions(${LOGGER_WARNINGS})
set (LOGGER_WARNINGS "-Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default")
add_definitions(${LOGGER_CFLAGS})
set (LOGGER_CFLAGS "${LOGGER_WARNINGS} -std=c++14")
set (LOGGER_CFLAGS "${LOGGER_CFLAGS}   -DG3_DYNAMIC_LOGGING")


file(GLOB Logger_SOURCES       "*.cpp")
file(GLOB Logger_SOURCES_Linux "Unix/*.cpp")
file(GLOB Logger_SOURCES_Win32 "Windows/*.cpp")

if (WIN32)
	set (Logger_SOURCES ${Logger_SOURCES} ${Logger_SOURCES_Win32})
else ()
	set (Logger_SOURCES ${Logger_SOURCES} ${Logger_SOURCES_Linux})
endif ()

add_library(Logger SHARED ${Logger_SOURCES})
target_include_directories(Logger PUBLIC . g3log)

if (WIN32)
	target_link_libraries(Logger DbgHelp)
else ()
	target_link_libraries(Logger pthread)
endif ()

set_target_properties(Logger PROPERTIES COMPILE_FLAGS "${LOGGER_CFLAGS}" )
