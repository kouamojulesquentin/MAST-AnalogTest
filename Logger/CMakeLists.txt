project(mast_logger)

# LOGGER:
file(GLOB Logger_SOURCES 	"*.cpp")
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

set_target_properties(Logger PROPERTIES
  COMPILE_FLAGS "${CFLAGS}"
  )
