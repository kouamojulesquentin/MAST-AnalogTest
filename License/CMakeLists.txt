project(License)

set (WARNINGS "-Wall -Wextra -Wpedantic  -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum")
set (CFLAGS "${WARNINGS}")
set (CPPFLAGS "${WARNINGS} -std=c++14")

include_directories(./)

add_library 	(aes	STATIC 	aes.c )
add_executable 	(make_license 	make_license.cpp )
add_executable 	(check_license 	check_license.cpp)
add_library 	(Mast_license 	STATIC check_license.cpp )

set_target_properties	(aes PROPERTIES COMPILE_FLAGS "${CFLAGS}")
set_target_properties	(make_license PROPERTIES COMPILE_FLAGS "${CPPFLAGS}")
set_target_properties	(check_license PROPERTIES COMPILE_FLAGS "${CPPFLAGS} -DTEST_LICENSE")
set_target_properties	(Mast_license PROPERTIES COMPILE_FLAGS "${CPPFLAGS}")

target_link_libraries(make_license aes)
target_link_libraries(check_license aes)
target_link_libraries(Mast_license aes)
