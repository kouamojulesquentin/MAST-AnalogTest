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

    Alias,            //!< For alias definitions
    Attribute,        //!< For attribute definitions
    DefaultLoadValue, //!< For registers default load value
    Enum_def,         //!< For enums definitions
    Enum_item,        //!< For enums items
    Enum_ref,         //!< For enums references
    Instance,         //!< For modules instances
    Module,           //!< For modules
    LocalParameter,   //!< For local parameters definitions
    Namespace_def,    //!< For namespace definitions
    Namespace_ref,    //!< For namespace references
    Parameter,        //!< For parameters definitions
    Parameter_ref,    //!< For parameters references
    ResetValue,       //!< For registers reset value
    ScalarIdentifier, //!< For identifier with a without scope prefix and no bit nor range identifier
    ScanInPort,       //!< For modules scan input port
    ScanInSource,     //!< For input of scan registers
    ScanOutPort,      //!< For modules scan output port
    ScanRegister,     //!< For scan registers
    Signal,           //!< For signal definitions
    Source,           //!< For unspecifed source signal(s)
    String,           //!< For string literals
    TranslationUnit,  //!< For source files as a all
    VectorIdentifier, //!< For identifiers with a without scope prefix and bit or range identifier
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

            ENUM_MEMBER(Parsers::Kind::Alias)
            ENUM_MEMBER(Parsers::Kind::Attribute)
            ENUM_MEMBER(Parsers::Kind::DefaultLoadValue)
            ENUM_MEMBER(Parsers::Kind::Enum_def)
            ENUM_MEMBER(Parsers::Kind::Enum_item)
            ENUM_MEMBER(Parsers::Kind::Enum_ref)
            ENUM_MEMBER(Parsers::Kind::Instance)
            ENUM_MEMBER(Parsers::Kind::LocalParameter)
            ENUM_MEMBER(Parsers::Kind::Module)
            ENUM_MEMBER(Parsers::Kind::Namespace_def)
            ENUM_MEMBER(Parsers::Kind::Namespace_ref)
            ENUM_MEMBER(Parsers::Kind::Parameter)
            ENUM_MEMBER(Parsers::Kind::Parameter_ref)
            ENUM_MEMBER(Parsers::Kind::ResetValue)
            ENUM_MEMBER(Parsers::Kind::ScalarIdentifier)
            ENUM_MEMBER(Parsers::Kind::ScanInPort)
            ENUM_MEMBER(Parsers::Kind::ScanInSource)
            ENUM_MEMBER(Parsers::Kind::ScanOutPort)
            ENUM_MEMBER(Parsers::Kind::ScanRegister)
            ENUM_MEMBER(Parsers::Kind::Signal)
            ENUM_MEMBER(Parsers::Kind::Source)
            ENUM_MEMBER(Parsers::Kind::String)
            ENUM_MEMBER(Parsers::Kind::TranslationUnit)
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


  // ---------------- Protected Methods
  //
  protected:

  AST_Node(Kind kind)
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
