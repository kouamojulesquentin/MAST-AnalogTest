//===========================================================================
//                           AST_IntegerExprRef.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerExprRef.cpp
//!
//! Implements class AST_IntegerExprRef
//!
//===========================================================================

#include "AST_IntegerExprRef.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_Parameter.hpp"
#include "AST_Builder.hpp"
#include "Utility.hpp"

using std::string;
using namespace mast;
using namespace Parsers;



//! Initializes with parameter reference
//!
//! @param parameterRef  Parameter reference. Must refer to a parameter representing an integer expression
//!
AST_IntegerExprRef::AST_IntegerExprRef (AST_ParameterRef* parameterRef)
  : AST_IntegerExpr (Kind::IntegerExprRef)
  , m_parameterRef  (CHECK_PARAMETER_NOT_NULL(parameterRef, "Integer expression reference cannot be build with nullptr"))
{
}
//
//  End of: AST_IntegerExprRef::AST_IntegerExprRef
//---------------------------------------------------------------------------


//! Build text representation of unary expression
//!
string AST_IntegerExprRef::AsText () const
{
  if (m_parameterRef != nullptr)
  {
    string asText;

    if (IsInverted())
    {
      asText.append("~");
    }

    asText.append(m_parameterRef->AsText());

    return asText;
  }

  if (m_integerExpr != nullptr)
  {
    string asText;
    auto   isLiterral = m_integerExpr->IsKind(Kind::IntegerLiteral);

    if (!isLiterral)
    {
      asText.append("(");    // Parentheses are used to show that the expression will be computed as a single entity
    }

    asText.append(m_integerExpr->AsText());

    if (!isLiterral)
    {
      asText.append(")");
    }
    return asText;
  }

  if (m_concatNumber != nullptr)
  {
    return m_concatNumber->AsText();
  }

  CHECK_FAILED("Houps: must handle either a parameter reference, an integer expression or a concatenated number");
}
//
//  End of: AST_IntegerExprRef::AsText
//---------------------------------------------------------------------------


//! Evaluates integer expression
//!
uint32_t AST_IntegerExprRef::Evaluate () const
{
  CHECK_VALUE_NOT_NULL(m_integerExpr, "Cannot evaluate from not yet resolved Parameter Reference or a concatenated number");

  uint32_t result = m_integerExpr->Evaluate();

  return result;
}
//
//  End of: AST_IntegerExprRef::Evaluate
//---------------------------------------------------------------------------



//! Returns true when operand is defined using Parameter reference(s)
//!
bool AST_IntegerExprRef::HasParameterRef () const
{
  return m_parameterRef != nullptr;
}
//
//  End of: AST_IntegerExprRef::HasParameterRef
//---------------------------------------------------------------------------


//! Replaces parameter references by their actual number
//!
//! @note All parameters must have been uniquified and transitive dependencies resolved
//!
//! @param parameters   Parameter definitions to use to replace parameter reference(s)
//!
void AST_IntegerExprRef::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  if (m_parameterRef != nullptr)
  {
    auto parameter = AST_Parameter::LocateParameterDef(m_parameterRef->Name(), parameters);

    CHECK_TRUE(parameter->IsNumber(), "Parameter reference \""s.append(m_parameterRef->Name())
                                                               .append("\" in integer expression must refers to a \"number\" parameter. \nGot: ")
                                                               .append(parameter->AsText()));

    auto        concatNumber = parameter->Number();
    const auto& numbers      = concatNumber->Numbers();

    CHECK_VALUE_NOT_EMPTY(numbers,  "Parameter reference \""s.append(m_parameterRef->Name())
                                                             .append("\" is expected to refer to a valid (non empty) \"number\" parameter. \nGot: ")
                                                             .append(parameter->AsText()));

    if (numbers.size() > 1u)
    {
      m_concatNumber = concatNumber;
    }
    else
    {
      auto number        = numbers.front();
      auto asIntegerExpr = dynamic_cast<AST_IntegerExpr*>(number);
      if (asIntegerExpr != nullptr)
      {
        m_integerExpr = asIntegerExpr;
      }
      else
      {
        m_concatNumber = concatNumber;
      }
    }

    m_parameterRef = nullptr; // Forget it as it is resolved
  }
}
//
//  End of: AST_IntegerExprRef::Resolve
//---------------------------------------------------------------------------



//! Returns uniquified clone
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_IntegerExprRef
AST_IntegerExprRef* AST_IntegerExprRef::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto clone = astBuilder.Clone_IntegerExprRef(this);

  if (m_integerExpr != nullptr)
  {
    clone->m_integerExpr = m_integerExpr->UniquifiedClone(astBuilder);
  }
  else if (m_concatNumber != nullptr)
  {
    clone->m_concatNumber = m_concatNumber->UniquifiedClone(astBuilder);
  }

  return clone;
}
//
//  End of: AST_IntegerExprRef::UniquifiedClone
//---------------------------------------------------------------------------

//===========================================================================
// End of AST_IntegerExprRef.cpp
//===========================================================================
