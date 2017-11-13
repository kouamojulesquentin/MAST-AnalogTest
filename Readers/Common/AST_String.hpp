//===========================================================================
//                           AST_String.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_String.hpp
//!
//! Declares AST_String class
//!
//===========================================================================

#ifndef AST_STRING_H__6A98C11C_3ED_4566_3B3_7153BE82CA31__INCLUDED_
  #define AST_STRING_H__6A98C11C_3ED_4566_3B3_7153BE82CA31__INCLUDED_


#include "AST_SimpleNode.hpp"
#include <string>

namespace Parsers
{
//! Represents a string literal
//!
class AST_String final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_String() = default;
  AST_String()  = delete;

  //! Text representation of value
  //!
  std::string AsText() const override;

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_String)(std::string&&);

  //! Initializes the value with specific value kind
  //!
  AST_String(std::string&& content)
    : AST_SimpleNode (Kind::String)
    , m_content      (content)
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::string m_content; //!< String content
};
//
//  End of AST_String class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_STRING_H__6A98C11C_3ED_4566_3B3_7153BE82CA31__INCLUDED_
//===========================================================================
// End of AST_String.hpp
//===========================================================================
