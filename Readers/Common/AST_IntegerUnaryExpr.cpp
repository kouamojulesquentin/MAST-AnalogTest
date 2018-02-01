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
#include "AST_ConcatNumber.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_Parameter.hpp"
#include "AST_Builder.hpp"
#include "Utility.hpp"

using std::string;
using namespace mast;
using namespace Parsers;



//! Initializes unary expression with kind of expression and operand
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



//! Returns true when operand is defined using Parameter reference(s)
//!
bool AST_IntegerUnaryExpr::HasParameterRef () const
{
  switch (GetKind())
  {
    case Kind::ParenthesizedExpr: return m_operand->HasParameterRef();

    default:
      CHECK_FAILED("Unsuported kind for unary expression: "s + KindName());
  }
}
//
//  End of: AST_IntegerUnaryExpr::HasParameterRef
//---------------------------------------------------------------------------


//! Replaces parameter references by their actual number
//!
//! @note All parameters must have been uniquified and transitive dependencies resolved
//!
//! @param parameters   Parameter definitions to use to replace parameter reference(s)
//!
void AST_IntegerUnaryExpr::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  switch (GetKind())
  {
    case Kind::ParenthesizedExpr:
      m_operand->Resolve(parameters);
      break;
    default:
      CHECK_FAILED("Unsuported kind for unary expression: "s + KindName());
  }
}
//
//  End of: AST_IntegerUnaryExpr::Resolve
//---------------------------------------------------------------------------



//! Returns uniquified clone
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_IntegerUnaryExpr
AST_IntegerUnaryExpr* AST_IntegerUnaryExpr::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto unaryExprClone = astBuilder.Clone_IntegerUnaryExpr(this);

  unaryExprClone->m_operand = m_operand->UniquifiedClone(astBuilder);

  return unaryExprClone;
}
//
//  End of: AST_IntegerUnaryExpr::UniquifiedClone
//---------------------------------------------------------------------------

//===========================================================================
// End of AST_IntegerUnaryExpr.cpp
//===========================================================================
