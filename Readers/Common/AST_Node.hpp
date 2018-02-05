//===========================================================================
//                           AST_Node.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Node.hpp
//!
//! Declares AST_Node class
//!
//===========================================================================


#ifndef AST_NODE_H__E1535643_2BE2_4F0C_92B8_43667E41845B__INCLUDED_
  #define AST_NODE_H__E1535643_2BE2_4F0C_92B8_43667E41845B__INCLUDED_

#include "EnumsUtility.hpp"
#include <memory>
#include <initializer_list>
#include <cstdint>

namespace Parsers
{
  //! Lists the kind of concrete AST_Node
  enum class Kind : uint8_t
  {
    Undefined,

    AccessLink,          //!< For access link
    Alias,               //!< For alias definitions
    ApplyEndState,       //!< For alias iApplyEndState
    AccessTogether,      //!< For alias  AccessTogether
    Attribute,           //!< For attribute definitions
    ConcatenatedNumbers, //!< For concatenated numbers
    DefaultLoadValue,    //!< For registers default load value
    DataInPort,          //!< For modules data input port
    DataOutPort,         //!< For modules data output port
    Enum_def,            //!< For enums definitions
    Enum_item,           //!< For enums items
    BSDLEntity,          //!< For BSDL file referred by AccessLink
    IntegerExprRef,      //!< For an integer expression reference
    IntegerLiteral,      //!< For a, unsigned, integer literal
    InputPort,           //!< For input of instance input port
    InputPortSource,     //!< For input of instance input port source
    Instance,            //!< For modules instances
    LocalParameter,      //!< For local parameters definitions
    Module,              //!< For modules
    ModuleIdentifier,    //!< For modules instances indentification
    NameSpace,           //!< For namespace definitions and references
    Network,             //!< For complete test network
    Number_Binary,       //!< For number defined with explicit binary base
    Number_Decimal,      //!< For number defined with explicit decimal base
    Number_Hexa,         //!< For number defined with explicit hexadecimal base
    Operator_Add,        //!< For binary add operator
    Operator_Substract,  //!< For binary substract operator
    Operator_Multiply,   //!< For binary multiply operator
    Operator_Divide,     //!< For binary divide operator
    Operator_Modulo,     //!< For binary modulo operator
    BsdlInstructionRef,  //!< For BSDL instruction referred by AccessLink
    Parameter,           //!< For parameters definitions
    Parameter_ref,       //!< For parameters references
    ParenthesizedExpr,   //!< For parenthesized integer expressions
    PlaceHolder,         //!< For a node place holder
    Port,                //!< For ScanInterface Ports
    RefEnum,             //!< For enums references
    ResetValue,          //!< For registers reset value
    ScalarIdentifier,    //!< For identifier with a without scope prefix and no bit nor range identifier
    ScanInPort,          //!< For modules scan input port
    ScanInSource,        //!< For input of scan registers
    ScanInterface,       //!< For scan interfaces
    ScanInterfaceRef,    //!< For scan interfaces reference
    ScanMux,             //!< For scan multiplexers
    ScanMuxSelection,    //!< For description of scan multiplexer selection
    ScanOutPort,         //!< For modules scan output port
    ScanRegister,        //!< For scan registers
    Signal,              //!< For signal definitions
    Source,              //!< For unspecifed source signal(s)
    String,              //!< For string literals
    TranslationUnit,     //!< For source files as a all
    Value_Bool,          //!< For boolean value
    VectorIdentifier,    //!< For identifiers with a without scope prefix and bit or range identifier
  };

  //! Returns whether one kind is one of specified kind
  //!
  static inline bool IsOneOf (Kind lhs, std::initializer_list<Kind> kinds)
  {
    for (const auto& kind : kinds)
    {
      if (lhs == kind)
      {
        return true;
      }
    }
    return false;
  }
} // End of namespace Parsers


ENUM_TRAITS(Parsers::Kind,
            ENUM_MEMBER(Parsers::Kind::Undefined)

            ENUM_MEMBER(Parsers::Kind::AccessLink)
            ENUM_MEMBER(Parsers::Kind::AccessTogether)
            ENUM_MEMBER(Parsers::Kind::Alias)
            ENUM_MEMBER(Parsers::Kind::ApplyEndState)
            ENUM_MEMBER(Parsers::Kind::Attribute)
            ENUM_MEMBER(Parsers::Kind::BsdlInstructionRef)
            ENUM_MEMBER(Parsers::Kind::ConcatenatedNumbers)
            ENUM_MEMBER(Parsers::Kind::DefaultLoadValue)
            ENUM_MEMBER(Parsers::Kind::DataInPort)
            ENUM_MEMBER(Parsers::Kind::DataOutPort)
            ENUM_MEMBER(Parsers::Kind::Enum_def)
            ENUM_MEMBER(Parsers::Kind::Enum_item)
            ENUM_MEMBER(Parsers::Kind::BSDLEntity)
            ENUM_MEMBER(Parsers::Kind::InputPort)
            ENUM_MEMBER(Parsers::Kind::InputPortSource)
            ENUM_MEMBER(Parsers::Kind::Instance)
            ENUM_MEMBER(Parsers::Kind::IntegerExprRef)
            ENUM_MEMBER(Parsers::Kind::IntegerLiteral)
            ENUM_MEMBER(Parsers::Kind::LocalParameter)
            ENUM_MEMBER(Parsers::Kind::Module)
            ENUM_MEMBER(Parsers::Kind::ModuleIdentifier)
            ENUM_MEMBER(Parsers::Kind::NameSpace)
            ENUM_MEMBER(Parsers::Kind::Network)
            ENUM_MEMBER(Parsers::Kind::Number_Binary)
            ENUM_MEMBER(Parsers::Kind::Number_Decimal)
            ENUM_MEMBER(Parsers::Kind::Number_Hexa)
            ENUM_MEMBER(Parsers::Kind::Operator_Add)
            ENUM_MEMBER(Parsers::Kind::Operator_Substract)
            ENUM_MEMBER(Parsers::Kind::Operator_Multiply)
            ENUM_MEMBER(Parsers::Kind::Operator_Divide)
            ENUM_MEMBER(Parsers::Kind::Operator_Modulo)
            ENUM_MEMBER(Parsers::Kind::Parameter)
            ENUM_MEMBER(Parsers::Kind::Parameter_ref)
            ENUM_MEMBER(Parsers::Kind::ParenthesizedExpr)
            ENUM_MEMBER(Parsers::Kind::PlaceHolder)
            ENUM_MEMBER(Parsers::Kind::Port)
            ENUM_MEMBER(Parsers::Kind::RefEnum)
            ENUM_MEMBER(Parsers::Kind::ResetValue)
            ENUM_MEMBER(Parsers::Kind::ScalarIdentifier)
            ENUM_MEMBER(Parsers::Kind::ScanInPort)
            ENUM_MEMBER(Parsers::Kind::ScanInSource)
            ENUM_MEMBER(Parsers::Kind::ScanInterface)
            ENUM_MEMBER(Parsers::Kind::ScanInterfaceRef)
            ENUM_MEMBER(Parsers::Kind::ScanMux)
            ENUM_MEMBER(Parsers::Kind::ScanMuxSelection)
            ENUM_MEMBER(Parsers::Kind::ScanOutPort)
            ENUM_MEMBER(Parsers::Kind::ScanRegister)
            ENUM_MEMBER(Parsers::Kind::Signal)
            ENUM_MEMBER(Parsers::Kind::Source)
            ENUM_MEMBER(Parsers::Kind::String)
            ENUM_MEMBER(Parsers::Kind::TranslationUnit)
            ENUM_MEMBER(Parsers::Kind::Value_Bool)
            ENUM_MEMBER(Parsers::Kind::VectorIdentifier)
           );

namespace Parsers
{
class AST_Visitor;

//! Base, abstract, class for AST specialized nodes
//!
class AST_Node
{
  // ---------------- Public Methods
  //
  public:
  virtual ~AST_Node() = default;
  AST_Node() = delete;

  virtual void Accept (AST_Visitor& visitor) {  }; //!< Visited part of the Visitor pattern (by default, do not participate in tree traversal)

  std::experimental::string_view KindName() const { return NameString(m_kind); }  //!< Returns human readable kind

  Kind GetKind () const { return m_kind; }; //!< Returns node kind (some share the same C++ type)

  bool IsOneOf(std::initializer_list<Kind> kinds) const { return Parsers::IsOneOf(m_kind, kinds); } //!< Returns whether node is one of specified kinds

  bool IsKind(Kind kind) const { return m_kind == kind; } //!< Returns whether node is specified kind

  void ChangeKind (Kind newKind) { m_kind = newKind; }; //!< Changes current kind (mainly used for place holder that can assign a new kind once assigned to a "target" variable)

  //! Returns true when a node is defined using Parameter reference(s)
  //!
  virtual bool HasParameterRef () const { return false; };


  // ---------------- Protected Methods
  //
  protected:

  explicit AST_Node(Kind kind)
    : m_kind (kind)
  {
  }


  // ---------------- Private Fields
  //
  private:
  Kind m_kind = Kind::Undefined;  //!< Returns node kind (some nodes share the same C++ type). This is also useful for fast downcasting and for debug purpose
};
//
//  End of AST_Node class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers

#define MAKE_UNIQUE_AS_FRIEND(x) friend std::unique_ptr<x> std::make_unique<x>





#endif  // not defined AST_NODE_H__E1535643_2BE2_4F0C_92B8_43667E41845B__INCLUDED_

//===========================================================================
// End of AST_Node.hpp
//===========================================================================
