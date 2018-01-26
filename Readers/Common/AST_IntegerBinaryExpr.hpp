//===========================================================================
//                           AST_IntegerBinaryExpr.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerBinaryExpr.hpp
//!
//! Declares AST_IntegerBinaryExpr class
//!
//===========================================================================

#ifndef AST_INTEGERBINARYEXPR_H__47548788_7967_487A_F284_2F22B3FBAA5__INCLUDED_
  #define AST_INTEGERBINARYEXPR_H__47548788_7967_487A_F284_2F22B3FBAA5__INCLUDED_

#include "AST_IntegerExpr.hpp"

namespace Parsers
{
//! Represents a binary integer expression
//!
class AST_IntegerBinaryExpr final : public AST_IntegerExpr
{
  // ---------------- Public Methods
  //
  public:
  ~AST_IntegerBinaryExpr() = default;
  AST_IntegerBinaryExpr()  = delete;


  std::string AsText()   const override; //!< Build text representation of integer expression
  uint32_t    Evaluate() const override; //!< Evaluates expression

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                             // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_IntegerBinaryExpr)(Parsers::Kind& kind, AST_IntegerExpr*&, AST_IntegerExpr*&); // AST currently uses make_unit<T>() to create nodes

  //! Initializes a binary expression with kind of expression and operands
  //!
  AST_IntegerBinaryExpr(Kind kind, AST_IntegerExpr* leftOperand, AST_IntegerExpr* rightOperand);

  // ---------------- Private Fields
  //
  private:
  AST_IntegerExpr* m_left;  // Left side of the binary expression
  AST_IntegerExpr* m_right; // Right side of the binary expression
};
//
//  End of AST_IntegerBinaryExpr class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_INTEGERBINARYEXPR_H__47548788_7967_487A_F284_2F22B3FBAA5__INCLUDED_

//===========================================================================
// End of AST_IntegerBinaryExpr.hpp
//===========================================================================
