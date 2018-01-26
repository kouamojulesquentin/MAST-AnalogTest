//===========================================================================
//                           AST_IntegerExpr.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerExpr.hpp
//!
//! Declares AST_IntegerExpr class
//!
//===========================================================================


#ifndef  AST_INTEGEREXPR_H__62389122_2D94_4FB0_9399_93E89882DB90__INCLUDED_
 #define AST_INTEGEREXPR_H__62389122_2D94_4FB0_9399_93E89882DB90__INCLUDED_

#include "AST_SimpleNode.hpp"

#include <string>

namespace Parsers
{
//+class AST_IntegerExprVisitor;

//! Represents an integer expression that resolves to unsigned integer (implicitly always positive)
//!
//! @note In ICL grammar, an integer expression (excerpt) is:
//!     integer_expr : integer_expr_lvl1 ;
//!     integer_expr_lvl1 : integer_expr_lvl2 ( ('+' | '-') integer_expr_lvl1 )? ;
//!     integer_expr_lvl2 : integer_expr_arg (('*' | '/' | '%') integer_expr_lvl2 )? ;
//!     integer_expr_paren : '(' integer_expr ')';
//!     integer_expr_arg : integer_expr_paren | pos_int | parameter_ref ;
//!
//!     So, it must support additive and multiplicative operators, parenthesis grouping as well as parameter references
//!
class AST_IntegerExpr : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_IntegerExpr() = default;
  AST_IntegerExpr()  = delete;

  virtual std::string AsText()   const = 0; //!< Text representation of integer expression
  virtual uint32_t    Evaluate() const = 0; //!< Evaluates expression

//+  virtual void Accept (AST_IntegerExprVisitor& visitor) = 0; //!< Visited part of the Visitor pattern

  // ---------------- Protected Methods
  //

  //! Initializes base class
  //!
  AST_IntegerExpr(Kind kind)
    : AST_SimpleNode (kind)
  {
  }
};
//
//  End of AST_IntegerExpr class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


 #endif // not defined AST_INTEGEREXPR_H__62389122_2D94_4FB0_9399_93E89882DB90__INCLUDED_

//===========================================================================
// End of AST_IntegerExpr.hpp
//===========================================================================
