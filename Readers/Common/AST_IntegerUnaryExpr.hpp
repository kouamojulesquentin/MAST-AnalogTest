//===========================================================================
//                           AST_IntegerUnaryExpr.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerUnaryExpr.hpp
//!
//! Declares AST_IntegerUnaryExpr class
//!
//===========================================================================

#ifndef AST_INTEGERUNARYEXPR_H__1D99211C_CCBD_4073_9A1_C1F1E6074A60__INCLUDED_
  #define AST_INTEGERUNARYEXPR_H__1D99211C_CCBD_4073_9A1_C1F1E6074A60__INCLUDED_

#include "AST_IntegerExpr.hpp"

namespace Parsers
{
//! Represents an unary integer expression
//!
class AST_IntegerUnaryExpr final : public AST_IntegerExpr
{
  // ---------------- Public Methods
  //
  public:
  ~AST_IntegerUnaryExpr() = default;
  AST_IntegerUnaryExpr()  = delete;


  std::string AsText()   const override; //!< Build text representation of integer expression
  uint32_t    Evaluate() const override; //!< Evaluates expression

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                                    // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_IntegerUnaryExpr)(Parsers::Kind& kind, AST_IntegerExpr*&); // AST currently uses make_unit<T>() to create nodes

  //! Initializes an unary expression with kind of expression and operand
  //!
  AST_IntegerUnaryExpr(Kind kind, AST_IntegerExpr* operand);

  // ---------------- Private Fields
  //
  private:
  AST_IntegerExpr* m_operand;  // Operand expression
};
//
//  End of AST_IntegerUnaryExpr class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_INTEGERUNARYEXPR_H__1D99211C_CCBD_4073_9A1_C1F1E6074A60__INCLUDED_
//===========================================================================
// End of AST_IntegerUnaryExpr.hpp
//===========================================================================
