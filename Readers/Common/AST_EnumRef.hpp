//===========================================================================
//                           AST_EnumRef.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_EnumRef.hpp
//!
//! Declares AST_EnumRef class
//!
//===========================================================================

#ifndef AST_ENUMREF_H__6F896BC1_EC9D_44F9_788_B0916625A92A__INCLUDED_
  #define AST_ENUMREF_H__6F896BC1_EC9D_44F9_788_B0916625A92A__INCLUDED_

#include "AST_SimpleNode.hpp"
#include <experimental/string_view>
#include <string>

namespace Parsers
{
//! Represents a enum reference
//!
class AST_EnumRef final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_EnumRef() = default;
  AST_EnumRef()  = delete;

  //! Enum name
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of enum
  //!
  std::string AsText() const override { return std::string("$").append(m_name); }

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_EnumRef)(std::string&&);

  //! Initializes AST_EnumRef with a number
  //!
  //! @param name  Refered enum name
  //!
  AST_EnumRef(std::string&& name)
    : AST_SimpleNode (Kind::Enum_ref)
    , m_name         (std::move(name))
  {
  }

  // ---------------- Private Fields
  //
  private:
  const std::string m_name; //!< Refereed enum name
};
//
//  End of AST_EnumRef class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_ENUMREF_H__6F896BC1_EC9D_44F9_788_B0916625A92A__INCLUDED_
//===========================================================================
// End of AST_EnumRef.hpp
//===========================================================================
