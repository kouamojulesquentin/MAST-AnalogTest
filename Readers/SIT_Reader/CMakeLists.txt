cmake_minimum_required(VERSION 3.5)
project(SIT_Parser CXX)

find_package(FLEX)
find_package(BISON)
if (NOT FLEX_FOUND)
 message(FATAL_ERROR "SIT_Parser: Flex not found")
endif()

if (NOT BISON_FOUND)
 message(FATAL_ERROR "SIT_Parser: Bison not found")
endif()

if (CODE_COVERAGE)
  set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS} ")
endif()

set (Flex_Switches -v -L --c++)
if (WIN32)
  set (Flex_Switches ${Flex_Switches} --wincompat)
endif()

set (Bison_Switches -d --verbose --no-lines --warnings=all --feature=caret --report=itemset --debug)
set (LEX_YACC_SRC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/")

message("")
message("================================================================================")
message(STATUS "SIT_Parser: CODE_COVERAGE:    ${CODE_COVERAGE}")
message(STATUS "SIT_Parser: CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
message(STATUS "SIT_Parser: Flex_Switches:    ${Flex_Switches}")
message(STATUS "SIT_Parser: Bison_Switches:   ${Bison_Switches}")
message("================================================================================")
message("")

# Create custom command for lexer (using flex)
#
add_custom_target(SIT_Lexer_class echo "Creating SIT_Lexer")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}SIT_Lexer.yy.cc
  COMMAND flex ${Flex_Switches}  --outfile=${LEX_YACC_SRC_DIR}SIT_Lexer.yy.cc ${LEX_YACC_SRC_DIR}SIT_Lexer.l
  DEPENDS SIT_Lexer.l
)

# Create custom command for parser (using Bison)
#
add_custom_target(SIT_Parser_class echo "Creating SIT_Parser")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}SIT_Parser.tab.cc ${LEX_YACC_SRC_DIR}SIT_Parser.tab.hh
  COMMAND bison ${Bison_Switches} ${LEX_YACC_SRC_DIR}SIT_Parser.yy -o ${LEX_YACC_SRC_DIR}SIT_Parser.tab.cc
  DEPENDS SIT_Parser.yy
)

set_source_files_properties(${LEX_YACC_SRC_DIR}SIT_Parser.tab.cc GENERATED)
set_source_files_properties(${LEX_YACC_SRC_DIR}SIT_Lexer.yy.cc   GENERATED)

add_library(SIT_Reader SHARED  ${LEX_YACC_SRC_DIR}SIT_Reader.cpp
                               ${LEX_YACC_SRC_DIR}SIT_Parser.tab.cc
                               ${LEX_YACC_SRC_DIR}SIT_Lexer.yy.cc
                               SIT_Printer.cpp)

target_include_directories (SIT_Reader PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CMAKE_CURRENT_SOURCE_DIR}/../Common
                                       PRIVATE $<$<PLATFORM_ID:Windows>:$ENV{FLEX_DIR}>)

target_compile_options     (SIT_Reader PRIVATE -std=c++14
                                               -Wall
                                               -Wno-sign-compare
                                               -Wnon-virtual-dtor
                                               -Wredundant-decls
                                               -Wundef
                                               -Wmissing-declarations
                                               -Wmissing-include-dirs
                                               $<$<NOT:$<PLATFORM_ID:Windows>>:-fPIC>)

target_link_libraries       (SIT_Reader Mast_Core)
