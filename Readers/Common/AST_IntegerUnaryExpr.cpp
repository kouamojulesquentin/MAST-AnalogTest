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
//! @param kind     Kind of binary expression
//! @param operand  Expression operand
//!
AST_IntegerUnaryExpr::AST_IntegerUnaryExpr (Kind kind, AST_IntegerExpr* operand)
  : AST_IntegerExpr (kind)
  , m_operand       (CHECK_PARAMETER_NOT_NULL(operand, "Unary expression cannot be build with nullptr operaand expression"))
{
}
//
//  End of: AST_IntegerUnaryExpr::AST_IntegerUnaryExpr
//---------------------------------------------------------------------------


//! Build text representation of integer expression
//!
string AST_IntegerUnaryExpr::AsText () const
{
  string asText(m_operand->AsText());

  return asText;
}
//
//  End of: AST_IntegerUnaryExpr::AsText
//---------------------------------------------------------------------------


//! Evaluates expression
//!
//! @note Each sub-expressions must evaluate as positive integer
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
