if (WIN32)
	if (NOT ${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
		message("\n>>>>>>>>>>>>>>> Syntax should be: cmake -G \"MinGW Makefiles\" <<<<<<<<<<<<<<<\n\n")
	endif()
  set (CMAKE_CXX_COMPILER "g++")
  set (CMAKE_C_COMPILER   "gcc")
endif()

cmake_minimum_required(VERSION 3.5)

  set (CMAKE_CXX_COMPILER "g++-4.9")
  
project(SIT_parser)

add_definitions(${WARNINGS})
set (WARNINGS -Wall -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum)
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

# SIT_scanner:

set (LEX_YACC_SRC_DIR  "../")

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

# Since parser.c does not exists yet when cmake is run, mark
# it as generated
 SET_SOURCE_FILES_PROPERTIES(SIT_parser.tab.cc GENERATED)
 SET_SOURCE_FILES_PROPERTIES(SIT_lexer.yy.cc GENERATED)

# Include binary directory to include lexer.c in parser.c
 INCLUDE_DIRECTORIES(.)

include_directories(${CMAKE_BINARY_DIR})

add_executable (SIT_parser ${LEX_YACC_SRC_DIR}main.cpp ${LEX_YACC_SRC_DIR}SIT_driver.cpp SIT_parser.tab.cc SIT_lexer.yy.cc)
