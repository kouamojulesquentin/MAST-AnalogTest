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
#include "BinaryVector.hpp"

#include <string>

namespace Parsers
{
//! Represents some value
//!
class AST_Value final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Value() = default;
  AST_Value()  = delete;

  std::string AsText() const override { return m_valueExpression; }; //!< Text representation of value

  mast::BinaryVector AsBinaryVector() const;    //!< Value as a BinaryVector

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                             // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Value)(Parsers::Kind& kind, std::experimental::string_view&); // AST currently uses make_unit<T>() to create nodes

  //! Initializes the value with specific value kind
  //!
  AST_Value(Kind kind, std::experimental::string_view valueExpression)
    : AST_SimpleNode    (kind)
    , m_valueExpression (valueExpression)
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::string   m_valueExpression;  //!< Expression representing the value
};
//
//  End of AST_Value class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_VALUE_H__C7D6CDC4_753_49F0_45B7_9FFA68F2FD4C__INCLUDED_

//===========================================================================
// End of AST_Value.hpp
//===========================================================================
