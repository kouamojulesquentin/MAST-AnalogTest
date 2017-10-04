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

set (Flex_Switches -v -L --c++)
if (WIN32)
  set (Flex_Switches ${Flex_Switches} --wincompat)
else()
  set (P_CFLAGS "${P_CFLAGS} -fPIC")
endif()

set (Bison_Switches -d --verbose --no-lines --warnings=all --feature=caret --report=itemset --debug)
set (LEX_YACC_SRC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/")

message("")
message("================================================================================")
message(STATUS "ICL_Parser: CODE_COVERAGE:    ${CODE_COVERAGE}")
message(STATUS "ICL_Parser: CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
message(STATUS "ICL_Parser: Flex_Switches:    ${Flex_Switches}")
message(STATUS "ICL_Parser: Bison_Switches:   ${Bison_Switches}")
message("================================================================================")
message("")

# Create custom command for lexer (using flex)
#
add_custom_target(ICL_Lexer_class echo "Creating ICL_Lexer")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc
  COMMAND flex ${Flex_Switches}  --outfile=${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc ${LEX_YACC_SRC_DIR}ICL_Lexer.l
  DEPENDS ICL_Lexer.l
)

# Create custom command for parser (using Bison)
#
add_custom_target(ICL_Parser_class echo "Creating ICL_Parser")
add_custom_command(
  OUTPUT ${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc ${LEX_YACC_SRC_DIR}ICL_Parser.tab.hh
  COMMAND bison ${Bison_Switches} ${LEX_YACC_SRC_DIR}ICL_Parser.yy -o ${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc
  DEPENDS ICL_Parser.yy
)

set_source_files_properties(${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc GENERATED)
set_source_files_properties(${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc   GENERATED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${LIBMAST_CORE_INCLUDE_DIRS})
include_directories(${LIBLOGGER_INCLUDE_DIRS})

set(LIBICL_READER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/." PARENT_SCOPE)

if (WIN32)
  include_directories($ENV{FLEX_DIR})
endif()

add_library(ICL_Reader SHARED  ${LEX_YACC_SRC_DIR}ICL_Reader.cpp ${LEX_YACC_SRC_DIR}ICL_Parser.tab.cc ${LEX_YACC_SRC_DIR}ICL_Lexer.yy.cc)

set_target_properties(ICL_Reader PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries(ICL_Reader Mast_Core)


# ---------------- Smoke tests
#
add_executable        (ICL_SmokeTest ICL_Parser_SmokeTest.cpp)
set_target_properties (ICL_SmokeTest PROPERTIES COMPILE_FLAGS "${P_CFLAGS}" )
target_link_libraries (ICL_SmokeTest ICL_Reader Mast_Core)

include(CTest)

install(DIRECTORY SmokeTests_Files DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ICL)
set(SMOKE_FILES_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ICL/SmokeTests_Files)

add_test(NAME ICL:Parser:fragment.icl_source.comment      COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/fragment.icl_source.comment.icl "ICL error: Line 5:12-14: ")
add_test(NAME ICL:Parser:fragment.icl_source.module_items COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/fragment.icl_source.module_items.icl)
add_test(NAME ICL:Parser:fragment.icl_source.namespace    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/fragment.icl_source.namespace.icl)
add_test(NAME ICL:Parser:fragment.icl_source.ports        COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/fragment.icl_source.ports.icl)

add_test(NAME ICL:Parser:guide_F_1    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/guide_F_1.icl)
add_test(NAME ICL:Parser:guide_F_2    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/guide_F_2.icl)
add_test(NAME ICL:Parser:guide_F_3    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/guide_F_3.icl)
add_test(NAME ICL:Parser:guide_F_4    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/guide_F_4.icl)
add_test(NAME ICL:Parser:guide_F_5    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/guide_F_5.icl)
add_test(NAME ICL:Parser:guide_F_6    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/guide_F_6.icl)

add_test(NAME ICL:Parser:example_E_1  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_1.icl)
add_test(NAME ICL:Parser:example_E_2  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_2.icl)
add_test(NAME ICL:Parser:example_E_3  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_3.icl)
add_test(NAME ICL:Parser:example_E_4  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_4.icl)
add_test(NAME ICL:Parser:example_E_5  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_5.icl)
add_test(NAME ICL:Parser:example_E_6  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_6.icl)
add_test(NAME ICL:Parser:example_E_7  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_7.icl)
add_test(NAME ICL:Parser:example_E_8  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_8.icl)
add_test(NAME ICL:Parser:example_E_9  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_9.icl)
add_test(NAME ICL:Parser:example_E_10 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_10.icl)
add_test(NAME ICL:Parser:example_E_11 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_11.icl)
add_test(NAME ICL:Parser:example_E_12 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_12.icl)
add_test(NAME ICL:Parser:example_E_13 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_13.icl)
add_test(NAME ICL:Parser:example_E_14 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_14.icl)
add_test(NAME ICL:Parser:example_E_15 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_15.icl)
add_test(NAME ICL:Parser:example_E_16 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_16.icl)
add_test(NAME ICL:Parser:example_E_17 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_17.icl)
add_test(NAME ICL:Parser:example_E_18 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_18.icl)
add_test(NAME ICL:Parser:example_E_19 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_19.icl)
add_test(NAME ICL:Parser:example_E_20 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_20.icl)
add_test(NAME ICL:Parser:example_E_21 COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/example_E_21.icl)

add_test(NAME ICL:Parser:annex_e_ICL  COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/annex_e_ICL.icl)


install(DIRECTORY Benches DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ICL)
set(SMOKE_FILES_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ICL/Benches)

add_test(NAME ICL:Parser:Bench:ScanCell       COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/ScanCell.icl)
add_test(NAME ICL:Parser:Bench:NetworkStructs COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/NetworkStructs.icl)
add_test(NAME ICL:Parser:Bench:Instruments    COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/Instruments.icl)
add_test(NAME ICL:Parser:Bench:E30            COMMAND ICL_SmokeTest ${SMOKE_FILES_DIR}/E30.icl)
