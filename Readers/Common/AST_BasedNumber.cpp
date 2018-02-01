//===========================================================================
//                           AST_BasedNumber.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_BasedNumber.cpp
//!
//! Implements class AST_BasedNumber
//!
//===========================================================================

#include "AST_BasedNumber.hpp"
#include "AST_IntegerExpr.hpp"
#include "AST_Builder.hpp"

#include "Utility.hpp"

#include <experimental/string_view>
#include <algorithm>

using std::string;
using std::experimental::string_view;

using namespace std::experimental::literals::string_view_literals;
using namespace Parsers;
using namespace mast;


//! Initializes with defined number base and digits
//!
AST_BasedNumber::AST_BasedNumber (Kind kind, std::string&& digits)
  : AST_Number (kind)
  , m_digits   (std::move(digits))
{
  CHECK_PARAMETER_TRUE(Parsers::IsOneOf(kind, {
                                                Kind::Number_Binary,
                                                Kind::Number_Decimal,
                                                Kind::Number_Hexa,
                                              }),
                                              string(NameString(kind)).append(" is not valid as a \"based\" number"));

  CHECK_PARAMETER_NOT_EMPTY(m_digits,                        "Cannot construct a \"based\" number with no digits");
  CHECK_PARAMETER_GTE(m_digits.size(), 3u,                   "Based number requires a least a base prefix and one digit, got: "s.append(m_digits));
  CHECK_PARAMETER_EQ(m_digits.front(), '\'',                 "Based number must begin with \"'\" preceding base character, got: "s.append(m_digits));
  CHECK_PARAMETER_TRUE(IsElementOf(m_digits[1], "bBhHdD"sv), "Based number must begin with \"'\" preceding base character, got: "s.append(m_digits));
}
//
//  End of: AST_BasedNumber::AST_BasedNumber
//---------------------------------------------------------------------------


//! Returns number as a BinaryVector
//!
//! @param targetCount  When not zero, gives the number of target bits
//!
BinaryVector AST_BasedNumber::AsBinaryVector (uint32_t targetCount) const
{
  auto asBinaryVector = BinaryVector::CreateFromString(m_digits);

  auto hasTargetCount = targetCount != 0;
  if (hasTargetCount || IsSized())
  {
    auto targetBitscount = hasTargetCount ? targetCount : Size();

    auto currentBitsCount = asBinaryVector.BitsCount();
    if (targetBitscount > currentBitsCount)
    {
      auto missingBitsCount = targetBitscount - currentBitsCount;
      asBinaryVector.PrependBits(missingBitsCount);
    }
    else if (targetBitscount < currentBitsCount)
    {
      auto excessiveBitsCount = currentBitsCount - targetBitscount;
      asBinaryVector.TruncateLeadingZeroes(excessiveBitsCount);
    }
  }

  if (IsInverted())
  {
    asBinaryVector.ToggleBits();
  }

  return asBinaryVector;
}
//
//  End of: AST_BasedNumber::AsBinaryVector
//---------------------------------------------------------------------------


//! Build text representation of integer expression
//!
string AST_BasedNumber::AsText () const
{
  string asText;

  if (IsInverted())
  {
    asText.append("~");
  }

  if (m_sizeExpr != nullptr)
  {
    asText.append(m_sizeExpr->AsText());
  }

  asText.append(m_digits);

  return asText;
}
//
//  End of: AST_BasedNumber::AsText
//---------------------------------------------------------------------------



//! Replaces parameter references by their actual number
//!
//! @note All parameters must have been uniquified and transitive dependencies resolved
//!
//! @param parameters   Parameter definitions to use to replace parameter reference(s)
//!
void AST_BasedNumber::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  if (m_sizeExpr != nullptr)
  {
    m_sizeExpr->Resolve(parameters);
  }
}
//
//  End of: AST_BasedNumber::Resolve
//---------------------------------------------------------------------------



//! Returns size (only when sized)
//!
//! @return Size expression value or runtime_error when the based number is unsized
//!
uint32_t AST_BasedNumber::Size () const
{
  CHECK_VALUE_NOT_NULL(m_sizeExpr, "Cannot provide size when the number is not sized");

  return m_sizeExpr->Evaluate();
}
//
//  End of: AST_BasedNumber::Size
//---------------------------------------------------------------------------


//! Returns uniquified clone
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_BasedNumber
AST_BasedNumber* AST_BasedNumber::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto clone = astBuilder.Clone_BasedNumber(this);

  if (m_sizeExpr != nullptr)
  {
    clone->m_sizeExpr->UniquifiedClone(astBuilder);
  }
  return clone;
}
//
//  End of: AST_BasedNumber::UniquifiedClone
//---------------------------------------------------------------------------

//===========================================================================
// End of AST_BasedNumber.cpp
//===========================================================================
