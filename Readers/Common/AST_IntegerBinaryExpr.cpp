//===========================================================================
//                           AST_IntegerBinaryExpr.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerBinaryExpr.cpp
//!
//! Implements class AST_IntegerBinaryExpr
//!
//===========================================================================

#include "AST_IntegerBinaryExpr.hpp"
#include "Utility.hpp"

using std::string;
using namespace mast;
using namespace Parsers;



//! Initializes a binary expression with kind of expression and operands
//!
//! @param kind           Kind of binary expression
//! @param leftOperand    Left side operand expression
//! @param rightOperand   Right side operand expression
//!
AST_IntegerBinaryExpr::AST_IntegerBinaryExpr (Kind kind, AST_IntegerExpr* leftOperand, AST_IntegerExpr* rightOperand)
  : AST_IntegerExpr (kind)
  , m_left          (CHECK_PARAMETER_NOT_NULL(leftOperand,  "Binary expression cannot be build with nullptr left side expression"))
  , m_right         (CHECK_PARAMETER_NOT_NULL(rightOperand, "Binary expression cannot be build with nullptr right side expression"))
{
  CHECK_PARAMETER_TRUE(Parsers::IsOneOf(kind, {
                                                Kind::Operator_Add,
                                                Kind::Operator_Substract,
                                                Kind::Operator_Multiply,
                                                Kind::Operator_Divide,
                                                Kind::Operator_Modulo,
                                              }),
                                              string(NameString(kind)).append(" is not valid as a binary expression"));
}
//
//  End of: AST_IntegerBinaryExpr::AST_IntegerBinaryExpr
//---------------------------------------------------------------------------


//! Build text representation of integer expression
//!
string AST_IntegerBinaryExpr::AsText () const
{
  string asText(m_left->AsText());

  switch (GetKind())
  {
    case Kind::Operator_Add:       asText.append(" + "); break;
    case Kind::Operator_Substract: asText.append(" - "); break;
    case Kind::Operator_Multiply:  asText.append(" * "); break;
    case Kind::Operator_Divide:    asText.append(" / "); break;
    case Kind::Operator_Modulo:    asText.append(" % "); break;

    default:
      CHECK_FAILED("Unsuported kind for binary expression: "s + KindName());
  }

  asText.append(m_right->AsText());
  return asText;
}
//
//  End of: AST_IntegerBinaryExpr::AsText
//---------------------------------------------------------------------------

//! Evaluates expression
//!
//! @note Each sub-expressions must evaluate as positive integer
uint32_t AST_IntegerBinaryExpr::Evaluate () const
{
  uint32_t result = 0;
  uint32_t left   = m_left->Evaluate();
  uint32_t right  = m_right->Evaluate();

  using std::to_string;

  switch (GetKind())
  {
    case Kind::Operator_Add:
      result = left + right;
      CHECK_VALUE_GTE(result, left,  "Cannot add values '"s.append(to_string(left)).append("' and '").append(to_string(right)).append("' without an overflow"));
      break;
    case Kind::Operator_Substract:
      CHECK_VALUE_GTE(left, right, "Cannot substract value '"s.append(to_string(right)).append("' from value '").append(to_string(left)).append("'"));
      result = left - right;
      break;
    case Kind::Operator_Multiply:
    {
      auto tmpResult = static_cast<uint64_t>(left) * static_cast<uint64_t>(right);
      CHECK_VALUE_LTE(tmpResult, UINT32_MAX, "Cannot multiply values '"s.append(to_string(left)).append("' and '").append(to_string(right)).append("' without an overflow"));
      result = static_cast<uint32_t>(tmpResult);
      break;
    }
    case Kind::Operator_Divide:
      CHECK_VALUE_NEQ(right, 0, "Cannot divide value '"s.append(to_string(left)).append("' by 0'"));
      result = left / right;
      break;
    case Kind::Operator_Modulo:
      CHECK_VALUE_NEQ(right, 0, "Cannot compute the modulo of value '"s.append(to_string(left)).append("' by 0'"));
      result = left % right;
      break;
    default:
      CHECK_FAILED("Unsuported kind for binary expression: "s + KindName());
  }

  return result;
}
//
//  End of: AST_IntegerBinaryExpr::Evaluate
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_IntegerBinaryExpr.cpp
//===========================================================================
