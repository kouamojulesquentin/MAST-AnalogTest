cmake_minimum_required(VERSION 2.6)
project(Reader_Common CXX)

message("")
message("================================================================================")
message(STATUS "Reader_Common: CODE_COVERAGE:   ${CODE_COVERAGE}")
message("================================================================================")
message("")

add_library (Reader_Common SHARED AST.hpp
                                  AST.cpp
                                  AST_Node.hpp
                                  AST_NamedNode.hpp
                                  AST_ParentNode.hpp
                                  AST_Module.hpp
                                  AST_Module.cpp
                                  AST_ScanRegister.hpp
                                  AST_ScanRegister.cpp
                                  AST_Visitor.hpp
                                  AST_PrettyPrinter.hpp
                                  AST_PrettyPrinter.cpp
                                  ParserException.hpp
                                  Parser_Types.hpp
                                  Parser_PublicData.hpp
                                  Parser_PrivateData.hpp
                                  Parser_PrivateData.cpp
                                  Reader.hpp
                                  Reader.cpp)

target_include_directories (Reader_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options     (Reader_Common PRIVATE -std=c++14
                                                  -Wall
                                                  -Wno-sign-compare
                                                  -Wnon-virtual-dtor
                                                  -Wredundant-decls
                                                  -Wundef
                                                  -Wmissing-declarations
                                                  -Wmissing-include-dirs
                                                  $<$<NOT:$<PLATFORM_ID:Windows>>:-fPIC>)
if (CODE_COVERAGE)
  target_compile_options(Reader_Common PRIVATE --coverage)
endif()

target_link_libraries(Reader_Common Mast_Core)
