if (WIN32)
	if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
		message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
	endif()
  set (CMAKE_CXX_COMPILER "g++")
  set (CMAKE_C_COMPILER   "gcc")
endif()

cmake_minimum_required(VERSION 3.5)

  
project(SIT_parser)

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

if (WIN32)
#+  set (P_CFLAGS ${P_CFLAGS} -DBUILD_DLL)
  set (P_CFLAGS ${P_CFLAGS})
else()
  set (P_CFLAGS "${P_CFLAGS} -fPIC")
endif()


#add_compile_options(${P_CFLAGS})


# SIT_scanner:

set (LEX_YACC_SRC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/")

# Create target for the parser
 ADD_CUSTOM_TARGET(SIT_Parser echo "Creating SIT_parser")

# Create custom command for flex/lex (note the outputs)
add_custom_command(
  OUTPUT SIT_lexer.yy.cc
  COMMAND flex --outfile=SIT_lexer.yy.cc ${LEX_YACC_SRC_DIR}SIT_lexer.l
  DEPENDS SIT_lexer.l
)

add_custom_command(
  OUTPUT SIT_parser.tab.cc SIT_parser.tab.hh
  COMMAND bison -d -v ${LEX_YACC_SRC_DIR}SIT_parser.yy
  DEPENDS SIT_parser.yy
)

# Add parser.c to the list of sources
 SET(SIT_parser_SRCS ${SIT_parser_SRC} SIT_parser.tab.cc SIT_lexer.yy.cc)

# Since parser.c does not exists yet when cmake is run mark
# it as generated
 SET_SOURCE_FILES_PROPERTIES(SIT_parser.tab.cc GENERATED)
 SET_SOURCE_FILES_PROPERTIES(SIT_lexer.yy.cc GENERATED)

# Include binary directory to include lexer.c in parser.c
 INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)

include_directories(${CMAKE_BINARY_DIR}/SIT_parser/)

add_executable (SIT_parser ${LEX_YACC_SRC_DIR}main.cpp ${LEX_YACC_SRC_DIR}SIT_driver.cpp SIT_parser.tab.cc SIT_lexer.yy.cc)

 set_target_properties(SIT_parser PROPERTIES
 	 COMPILE_FLAGS "${P_CFLAGS}"
	  )
 
