cmake_minimum_required(VERSION 3.5)
project(SIT_parser)

find_package(FLEX)
find_package(BISON)
if ( NOT FLEX_FOUND)
 message( FATAL_ERROR "Flex not found")
endif() 

if (NOT BISON_FOUND)
 message( FATAL_ERROR "Bison not found")
endif() 

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -std=c++14")

add_definitions(${Flex_Switches})
if (WIN32)
#+  set (P_CFLAGS ${P_CFLAGS} -DBUILD_DLL)
  set (P_CFLAGS ${P_CFLAGS})
	set (Flex_Switches -v -L --wincompat)
else()
  set (P_CFLAGS "${P_CFLAGS} -fPIC")
	set (Flex_Switches -v -L)
endif()


set (LEX_YACC_SRC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/")

# Create custom command for flex/lex (note the outputs)
ADD_CUSTOM_TARGET(SIT_Lexer_class echo "Creating SIT_lexer")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}SIT_lexer.yy.cc
  COMMAND flex ${Flex_Switches}  --outfile=${LEX_YACC_SRC_DIR}SIT_lexer.yy.cc ${LEX_YACC_SRC_DIR}SIT_lexer.l
  DEPENDS SIT_lexer.l
)

# Create target for the parser
ADD_CUSTOM_TARGET(SIT_Parser_class echo "Creating SIT_parser")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}SIT_parser.tab.cc ${LEX_YACC_SRC_DIR}SIT_parser.tab.hh
  COMMAND bison -d -v -l ${LEX_YACC_SRC_DIR}SIT_parser.yy -o ${LEX_YACC_SRC_DIR}SIT_parser.tab.cc
  DEPENDS SIT_parser.yy
)

# Add parser.c to the list of sources
#+SET(SIT_parser_SRCS ${SIT_parser_SRC} ${LEX_YACC_SRC_DIR}SIT_parser.tab.cc ${LEX_YACC_SRC_DIR}SIT_lexer.yy.cc)

# Since parser.c does not exists yet when cmake is run, mark it as generated
SET_SOURCE_FILES_PROPERTIES(${LEX_YACC_SRC_DIR}SIT_parser.tab.cc GENERATED)
SET_SOURCE_FILES_PROPERTIES(${LEX_YACC_SRC_DIR}SIT_lexer.yy.cc 	 GENERATED)

# Include binary directory to include lexer.c in parser.c
#+include_directories(${CMAKE_BINARY_DIR}/SIT_parser/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(../Lib/include/)
include_directories(../Lib/public_include/)
include_directories(../Logger/)
include_directories(../Logger/g3log/)
if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()

add_library (SIT_Reader_Lib SHARED	${LEX_YACC_SRC_DIR}SIT_reader.cpp ${LEX_YACC_SRC_DIR}SIT_parser.tab.cc ${LEX_YACC_SRC_DIR}SIT_lexer.yy.cc)
add_executable 	(SIT_reader_demo 	${LEX_YACC_SRC_DIR}main.cpp)

set_target_properties(SIT_Reader_Lib 	PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
set_target_properties(SIT_reader_demo PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )

target_link_libraries(SIT_Reader_Lib Lib)
target_link_libraries(SIT_reader_demo SIT_Reader_Lib Lib)
