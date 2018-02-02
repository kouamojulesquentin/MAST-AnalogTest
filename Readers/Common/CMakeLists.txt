cmake_minimum_required(VERSION 2.6)
project(Reader_Common CXX)

message("")
message("================================================================================")
message(STATUS "Reader_Common: CODE_COVERAGE:   ${CODE_COVERAGE}")
message("================================================================================")
message("")

add_library (Reader_Common SHARED AST.hpp
                                  AST.cpp
                                  AST_AccessLink.hpp
                                  AST_AccessLink.cpp
                                  AST_Attribute.hpp
                                  AST_Attribute.cpp
                                  AST_BasedNumber.hpp
                                  AST_BasedNumber.cpp
                                  AST_BsdlInstructionRef.hpp
                                  AST_BsdlInstructionRef.cpp
                                  AST_ConcatNumber.hpp
                                  AST_ConcatNumber.cpp
                                  AST_EnumRef.hpp
                                  AST_IntegerExpr.hpp
                                  AST_IntegerExpr.cpp
                                  AST_IntegerExprRef.hpp
                                  AST_IntegerExprRef.cpp
                                  AST_IntegerLiteral.hpp
                                  AST_IntegerLiteral.cpp
                                  AST_IntegerBinaryExpr.hpp
                                  AST_IntegerBinaryExpr.cpp
                                  AST_IntegerUnaryExpr.hpp
                                  AST_IntegerUnaryExpr.cpp
                                  AST_FileRef.hpp
                                  AST_Instance.hpp
                                  AST_Instance.cpp
                                  AST_Node.hpp
                                  AST_NamedNode.hpp
                                  AST_ParentNode.hpp
                                  AST_ParentNode.cpp
                                  AST_Identifier.hpp
                                  AST_ModuleIdentifier.hpp
                                  AST_ModuleIdentifier.cpp
                                  AST_Module.hpp
                                  AST_Module.cpp
                                  AST_Namespace.hpp
                                  AST_Namespace.cpp
                                  AST_Network.hpp
                                  AST_Network.cpp
                                  AST_Number.hpp
                                  AST_Parameter.hpp
                                  AST_Parameter.cpp
                                  AST_ParameterRef.hpp
                                  AST_PlaceHolder.hpp
                                  AST_Port.hpp
                                  AST_Port.cpp
                                  AST_ScalarIdentifier.hpp
                                  AST_SimpleNode.hpp
                                  AST_Signal.hpp
                                  AST_Signal.cpp
                                  AST_Source.hpp
                                  AST_Source.cpp
                                  AST_ScanInterface.hpp
                                  AST_ScanInterface.cpp
                                  AST_ScanInterfaceRef.hpp
                                  AST_ScanMux.hpp
                                  AST_ScanMux.cpp
                                  AST_ScanMuxSelection.hpp
                                  AST_ScanMuxSelection.cpp
                                  AST_ScanRegister.hpp
                                  AST_ScanRegister.cpp
                                  AST_String.hpp
                                  AST_String.cpp
                                  AST_VectorIdentifier.hpp
                                  AST_VectorIdentifier.cpp
                                  AST_Value.hpp
                                  AST_Value.cpp
                                  AST_Visitor.hpp
                                  AST_Builder.hpp
                                  AST_Checker.hpp
                                  AST_Checker.cpp
                                  AST_PrettyPrinter.hpp
                                  AST_PrettyPrinter.cpp
                                  AST_SystemModelGenerator.hpp
                                  AST_SystemModelGenerator.cpp
                                  BSDL_Reader.hpp
                                  BSDL_Reader.cpp
                                  ParserException.hpp
                                  Parser_Types.hpp
                                  Parser_PublicData.hpp
                                  Parser_PrivateData.hpp
                                  Parser_PrivateData.cpp
                                  Reader.hpp
                                  Reader.cpp
                                  UnresolvedPathSelector.hpp)

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
