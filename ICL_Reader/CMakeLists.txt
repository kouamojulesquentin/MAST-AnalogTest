cmake_minimum_required(VERSION 3.5)
project(ICL_Parser)

find_package(FLEX)
find_package(BISON)
if (NOT FLEX_FOUND)
 message(FATAL_ERROR "ICL_Parser: Flex not found")
endif()

if (NOT BISON_FOUND)
 message(FATAL_ERROR "ICL_Parser: Bison not found")
endif()

add_definitions(${P_WARNINGS})
set (P_WARNINGS "-Wall -Wno-sign-compare -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-declarations -Wmissing-include-dirs")
add_definitions(${P_CFLAGS})
set (P_CFLAGS "${P_WARNINGS} -DICL_READER_EXPORTS -std=c++14")

if (CODE_COVERAGE)
  set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS} ")
endif()

add_definitions(${Flex_Switches})
set (Flex_Switches -v -L --c++)
if (WIN32)
  set (Flex_Switches ${Flex_Switches} --wincompat)
else()
  set (P_CFLAGS "${P_CFLAGS} -fPIC")
endif()

message("")
message("================================================================================")
message(STATUS "ICL_Parser: CODE_COVERAGE:    ${CODE_COVERAGE}")
message(STATUS "ICL_Parser: CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
message(STATUS "ICL_Parser: Flex_Switches:    ${Flex_Switches}")

set (LEX_YACC_SRC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/")

# Create custom command for flex/lex (note the outputs)
add_custom_target(ICL_Lexer_class echo "Creating ICL_Lexer")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc
  COMMAND flex ${Flex_Switches}  --outfile=${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc ${LEX_YACC_SRC_DIR}ICL_Lexer.l
  DEPENDS ICL_Lexer.l
)

# Create target for the parser
set(Bison_Switches -d --verbose --no-lines --warnings=all --feature=caret --report=itemset --debug)
message(STATUS "ICL_Parser: Bison_Switches:   ${Bison_Switches}")
message("================================================================================")
message("")

add_custom_target(ICL_Parser_class echo "Creating ICL_Parser")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc ${LEX_YACC_SRC_DIR}ICL_Parser.tab.hh
  COMMAND bison ${Bison_Switches} ${LEX_YACC_SRC_DIR}ICL_Parser.yy -o ${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc
  DEPENDS ICL_Parser.yy
)

# Since parser.c does not exists yet when cmake is run, mark it as generated
set_source_files_properties(${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc GENERATED)
set_source_files_properties(${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc   GENERATED)

# Include binary directory to include lexer.c in parser.c

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(LIBICL_READER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/." PARENT_SCOPE)

if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()

add_library(ICL_Reader_Lib SHARED  ${LEX_YACC_SRC_DIR}ICL_Reader.cpp ${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc ${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc)

set_target_properties(ICL_Reader_Lib PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries(ICL_Reader_Lib Mast_Core)
