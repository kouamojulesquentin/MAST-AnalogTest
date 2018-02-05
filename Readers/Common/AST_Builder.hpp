//===========================================================================
//                           AST_Builder.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Builder.hpp
//!
//! Declares AST_Builder class
//!
//===========================================================================

#ifndef AST_CLONER_H__87628D5E_5B7_455A_EE81_2F5FC373AAEF__INCLUDED_
  #define AST_CLONER_H__87628D5E_5B7_455A_EE81_2F5FC373AAEF__INCLUDED_

#include <string>
#include <vector>

namespace Parsers
{
class AST_Alias;
class AST_Attribute;
class AST_BasedNumber;
class AST_ConcatNumber;
class AST_IntegerExpr;
class AST_IntegerExprRef;
class AST_IntegerBinaryExpr;
class AST_IntegerUnaryExpr;
class AST_IntegerLiteral;
class AST_Instance;
class AST_Module;
class AST_Network;
class AST_Node;
class AST_Number;
class AST_Parameter;
class AST_ParameterRef;
class AST_Port;
class AST_ScalarIdentifier;
class AST_ScanMux;
class AST_ScanRegister;
class AST_Signal;
class AST_String;
class AST_ModuleIdentifier;
class AST_VectorIdentifier;
enum class Kind : uint8_t;

//! Interfaces for cloning AST nodes
//!
//! @note It is mainly used to avoid cyclic dependency (as a benefit, there is also less compile dependency)
class AST_Builder
{
  // ---------------- Public Methods
  //
  public:
  virtual AST_Attribute*         Clone_Attribute         (const AST_Attribute*         attribute) = 0;         //!< Clones an attribute
  virtual AST_BasedNumber*       Clone_BasedNumber       (const AST_BasedNumber*       basedNumber) = 0;       //!< Clones a "based" number
  virtual AST_ConcatNumber*      Clone_ConcatNumber      (const AST_ConcatNumber*      concatNumber) = 0;      //!< Clones a concated number
  virtual AST_Port*              Clone_Port              (const AST_Port*              port) = 0;              //!< Clones a port
  virtual AST_Instance*          Clone_Instance          (const AST_Instance*          instance) = 0;          //!< Clones an instance
  virtual AST_IntegerBinaryExpr* Clone_IntegerBinaryExpr (const AST_IntegerBinaryExpr* integerBinaryExpr) = 0; //!< Clones an integer binary expression
  virtual AST_IntegerExprRef*    Clone_IntegerExprRef    (const AST_IntegerExprRef*    integerExprRef) = 0;    //!< Clones an integer expression reference
  virtual AST_IntegerLiteral*    Clone_IntegerLiteral    (const AST_IntegerLiteral*    integerLiteral) = 0;    //!< Clones an integer literal
  virtual AST_IntegerUnaryExpr*  Clone_IntegerUnaryExpr  (const AST_IntegerUnaryExpr*  integerUnaryExpr) = 0;  //!< Clones an integer unary expression
  virtual AST_Module*            Clone_Module            (const AST_Module*            module) = 0;            //!< Clones a module
  virtual AST_Parameter*         Clone_Parameter         (const AST_Parameter*         parameter) = 0;         //!< Clones a parameter definition
  virtual AST_ScanMux*           Clone_ScanMux           (const AST_ScanMux*           scanMux) = 0;           //!< Clones a scan multiplexer
  virtual AST_ScanRegister*      Clone_ScanRegister      (const AST_ScanRegister*      scanRegister) = 0;      //!< Clones a scan register
  virtual AST_VectorIdentifier*  Clone_VectorIdentifier  (const AST_VectorIdentifier*  identifier) = 0;        //!< Clones a vector identifier

  virtual AST_Alias*             Create_Alias                      (AST_VectorIdentifier*       aliasIdentifier,
                                                                    std::vector<AST_Signal*>&&  signals,
                                                                    std::vector<AST_Node*>&&    optionalChildren) = 0; //!< Creates an AST_Alias
  virtual AST_BasedNumber*       Create_BasedNumber                (Kind                        kind, std::string&&    baseAndDigits) = 0;                              //!< Creates an AST_BasedNumber
  virtual AST_ConcatNumber*      Create_ConcatNumber               () = 0;                                                                                              //!< Creates an AST_ConcatNumber empty
  virtual AST_ConcatNumber*      Create_ConcatNumber               (std::vector<AST_Number*>&&  numbers) = 0;                                                           //!< Creates an AST_ConcatNumber
  virtual AST_IntegerExprRef*    Create_IntegerExprRef             (AST_ParameterRef*           parameterRef) = 0;                                                      //!< Creates an AST_IntegerExprRef node
  virtual AST_IntegerUnaryExpr*  Create_IntegerUnaryExpr           (Kind                        kind, AST_IntegerExpr* operand) = 0;                                    //!< Creates an AST_IntegerUnaryExpr node
  virtual AST_IntegerBinaryExpr* Create_IntegerBinaryExpr          (Kind                        kind, AST_IntegerExpr* leftOperand, AST_IntegerExpr* rightOperand) = 0; //!< Creates an AST_IntegerBinaryExpr node
  virtual AST_IntegerLiteral*    Create_IntegerLiteral             (std::string&&               integerText) = 0;                                                       //!< Creates an AST_IntegerLiteral node
  virtual AST_IntegerLiteral*    Create_IntegerLiteral             (uint32_t                    value) = 0;                                                             //!< Creates an AST_IntegerLiteral node
  virtual AST_String*            Create_String                     (std::string&&               content) = 0;                                                           //!< Creates an AST_String node
  virtual AST_ScalarIdentifier*  Create_UniquifiedIdentifier       (const AST_ScalarIdentifier* identifier) = 0;                                                        //!< Creates an identifier for a uniquified entity
  virtual AST_ModuleIdentifier*  Create_UniquifiedModuleIdentifier (const AST_Module*           module) = 0;                                                            //!< Creates an identifier for a uniquified module

  virtual AST_Network* Network() = 0;  //!< Returns instruments test network

  // ---------------- Protected Methods
  //
  protected:
  virtual ~AST_Builder() = default;
  AST_Builder()          = default;
};
//
//  End of AST_Builder class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_CLONER_H__87628D5E_5B7_455A_EE81_2F5FC373AAEF__INCLUDED_

//===========================================================================
// End of AST_Builder.hpp
//===========================================================================
