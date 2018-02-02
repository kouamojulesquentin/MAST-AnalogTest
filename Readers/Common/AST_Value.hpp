//===========================================================================
//                           AST_Value.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Value.hpp
//!
//! Declares AST_Value class
//!
//===========================================================================

#ifndef AST_VALUE_H__C7D6CDC4_753_49F0_45B7_9FFA68F2FD4C__INCLUDED_
  #define AST_VALUE_H__C7D6CDC4_753_49F0_45B7_9FFA68F2FD4C__INCLUDED_

#include "AST_SimpleNode.hpp"

#include <string>

namespace Parsers
{
class AST_ConcatNumber;

//! Represents a C++ built-in type
//!
template<typename T>
class AST_Value final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Value() = default;
  AST_Value()  = delete;

  std::string AsText() const override { return ToString(m_value); }; //!< Text representation of value

  //! Retrieves embedded (C++ built-in type) value
  //!
  T Value() const { return m_value; }

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                        // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Value<T>)(Parsers::Kind&, T&); // AST currently uses make_unit<T>() to create nodes

  //! Initializes the value with specific value kind
  //!
  AST_Value(Kind kind, T value)
    : AST_SimpleNode (kind)
    , m_value        (value)
  {
  }

  static std::string ToString(bool value) { return value ? "true" : "false"; }
  template<typename U>
  static std::string ToString(U    value) { return std::to_string(value); }

  // ---------------- Private Fields
  //
  private:
  T m_value = T(); //!< The effective value
};
//
//  End of AST_Value class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_VALUE_H__C7D6CDC4_753_49F0_45B7_9FFA68F2FD4C__INCLUDED_

//===========================================================================
// End of AST_Value.hpp
//===========================================================================
