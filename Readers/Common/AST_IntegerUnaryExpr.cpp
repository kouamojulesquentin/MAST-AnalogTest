//===========================================================================
//                           AST_IntegerUnaryExpr.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerUnaryExpr.cpp
//!
//! Implements class AST_IntegerUnaryExpr
//!
//===========================================================================

#include "AST_IntegerUnaryExpr.hpp"
#include "Utility.hpp"

using std::string;
using namespace mast;
using namespace Parsers;



//! Initializes a binary expression with kind of expression and operands
//!
//! @param kind     Kind of expression
//! @param operand  Expression operand
//!
AST_IntegerUnaryExpr::AST_IntegerUnaryExpr (Kind kind, AST_IntegerExpr* operand)
  : AST_IntegerExpr (kind)
  , m_operand       (CHECK_PARAMETER_NOT_NULL(operand, "Unary expression cannot be build with nullptr operand expression"))
{
  CHECK_PARAMETER_TRUE(Parsers::IsOneOf(kind, {
                                                Kind::ParenthesizedExpr,
                                              }),
                                              string(NameString(kind)).append(" is not valid as an unary expression"));
}
//
//  End of: AST_IntegerUnaryExpr::AST_IntegerUnaryExpr
//---------------------------------------------------------------------------


//! Build text representation of unary expression
//!
string AST_IntegerUnaryExpr::AsText () const
{
  switch (GetKind())
  {
    case Kind::ParenthesizedExpr:  return "("s.append(m_operand->AsText()).append(")");

    default:
      CHECK_FAILED("Unsuported kind for unary expression: "s + KindName());
  }
}
//
//  End of: AST_IntegerUnaryExpr::AsText
//---------------------------------------------------------------------------


//! Evaluates expression
//!
uint32_t AST_IntegerUnaryExpr::Evaluate () const
{
  uint32_t result = m_operand->Evaluate();

  return result;
}
//
//  End of: AST_IntegerUnaryExpr::Evaluate
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_IntegerUnaryExpr.cpp
//===========================================================================
