project(mast_logger)


add_definitions(${WARNINGS})
set (WARNINGS -Wall -Wextra -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default)
add_definitions(${CFLAGS})
set (CFLAGS ${WARNINGS} -std=c++14)

if (WIN32)
#+  set (CFLAGS ${CFLAGS} -DBUILD_DLL)
  set (CFLAGS ${CFLAGS})
else()
  set (CFLAGS ${CFLAGS} -fPIC)
endif()

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

add_compile_options(${CFLAGS})
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")
#set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CFLAGS}")

#Bring the headers into the project
include_directories(../Lib/include/)
include_directories(../Lib/public_include/)
include_directories(./)
include_directories(g3log/)
include_directories(Unix)
include_directories(Windows)

# LOGGER:
file(GLOB Logger_SOURCES 			 ".*.cpp")
file(GLOB Logger_SOURCES_Linux "Unix/*.cpp")
file(GLOB Logger_SOURCES_Win32 "Windows/*.cpp")

if (WIN32)
	set (Logger_SOURCES ${Logger_SOURCES} ${Logger_SOURCES_Win32})
else ()
	set (Logger_SOURCES ${Logger_SOURCES} ${Logger_SOURCES_Linux})
endif ()

add_library(Logger SHARED ${Logger_SOURCES})

if (WIN32)
	target_link_libraries(Logger DbgHelp)
else ()
	target_link_libraries(Logger pthread)
endif ()
