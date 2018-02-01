//===========================================================================
//                           AST_ConcatNumber.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ConcatNumber.cpp
//!
//! Implements class AST_ConcatNumber
//!
//===========================================================================

#include "AST_ConcatNumber.hpp"
#include "AST_Number.hpp"
#include "AST_Builder.hpp"

using std::string;
using namespace mast;
using namespace Parsers;


//! Returns concatenated numbers as a BinaryVector
//!
//! @note At most one number is allowed to be not sized
//!
//! @param targetCount  When not zero, gives the number of target bits
//!                     It must be non zero when one (and only one) number is not sized
//!
//! @return BinaryVector for all concatenated numbers or throw runtime_error when each number size cannot be assessed
//!
BinaryVector AST_ConcatNumber::AsBinaryVector (uint32_t targetCount) const
{
  CHECK_FALSE(m_numbers.empty(), "Cannot create a vector from \"empty\" concatenated number");

  // ---------------- Deal with case of one is not sized
  //
  uint32_t unsizedSize = 0u;  // Defines size of number that is not sized (at most one)
  bool     hasUnsized  = false;
  if (!IsFullySized())
  {
    CHECK_PARAMETER_NOT_ZERO(targetCount, "A valid size is required when one concatenated number is not sized");

    uint32_t sizedTotalSize = 0u;
    for (auto number : m_numbers)
    {
      if (number->IsSized())
      {
        sizedTotalSize += number->Size();
      }
      else
      {
        CHECK_FALSE(hasUnsized, "Cannot create a vector with more than one unsized number");
        hasUnsized = true;
      }
    }

    CHECK_PARAMETER_GT(targetCount, sizedTotalSize, "Cannot create a vector with one unsized number and a target bits count less than total of sized numbers");
    unsizedSize = targetCount - sizedTotalSize;
  }

  // ---------------- Do concatenate each numbers as BinaryVector
  //
  BinaryVector asBinaryVector;

  for (auto number : m_numbers)
  {
    auto numberSize     = number->IsSized() ? 0u : unsizedSize;
    auto numberAsVector = number->AsBinaryVector(numberSize);

    asBinaryVector.Append(numberAsVector);
  }

  // ---------------- Do final size adjustment (on left side)
  //
  auto hasTargetCount   = targetCount != 0;
  auto currentBitsCount = asBinaryVector.BitsCount();

  if (hasTargetCount && (currentBitsCount != targetCount))
  {
    if (targetCount > currentBitsCount)
    {
      auto missingBitsCount = targetCount - currentBitsCount;
      asBinaryVector.PrependBits(missingBitsCount);
    }
    else
    {
      auto excessiveBitsCount = currentBitsCount - targetCount;
      asBinaryVector.TruncateLeadingZeroes(excessiveBitsCount);
    }
  }

  return asBinaryVector;
}
//
//  End of: AST_ConcatNumber::AsBinaryVector
//---------------------------------------------------------------------------



//! Builds text representation of concatenated numbers
//!
string AST_ConcatNumber::AsText () const
{
  string asText;

  auto first = true;

  for (auto number : m_numbers)
  {
    CHECK_VALUE_NOT_NULL(number, "Houps: AST_ConcatNumber has been build with a nullptr number");
    if (first)
    {
      first = false;
    }
    else
    {
      asText.append(", ");
    }
    asText.append(number->AsText());
  }

  return asText;
}
//
//  End of: AST_ConcatNumber::AsText
//---------------------------------------------------------------------------



//! Returns true when there is a parameter reference in any of concatenated numbers
//!
bool AST_ConcatNumber::HasParameterRef () const
{
  for (const auto number : m_numbers)
  {
    if (number->HasParameterRef())
    {
      return true;
    }
  }

  return false;
}
//
//  End of: AST_ConcatNumber::HasParameterRef
//---------------------------------------------------------------------------



//! Returns true when a size (bits count) is defined for each of concatenated number
//!
//! @note When it represents no number at all, the size is considered undefined (not fully sized)
//!
//! @return true When all concatenated numbers are sized, false otherwise
bool AST_ConcatNumber::IsFullySized () const
{
  for (auto number : m_numbers)
  {
    if (!number->IsSized())
    {
      return false;
    }
  }
  return !m_numbers.empty();
}
//
//  End of: AST_ConcatNumber::IsFullySized
//---------------------------------------------------------------------------



//! Replaces parameter references by its actual number
//!
//! @note This must be called after unification pass
//!
//! @param parameters   Parameter definitions to use to replace parameter reference(s)
//!
void AST_ConcatNumber::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  for (auto& number : m_numbers)
  {
    if (number->HasParameterRef())
    {
      number->Resolve(parameters);
    }
  }
}
//
//  End of: AST_ConcatNumber::Resolve
//---------------------------------------------------------------------------



//! Returns uniquified clone
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_ConcatNumber
AST_ConcatNumber* AST_ConcatNumber::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto clone = astBuilder.Clone_ConcatNumber(this);

  for (auto& number : clone->m_numbers)
  {
    number = number->UniquifiedClone(astBuilder);
  }
  return clone;
}
//
//  End of: AST_ConcatNumber::UniquifiedClone
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_ConcatNumber.cpp
//===========================================================================
