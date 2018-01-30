//===========================================================================
//                           AST_IntegerExpr.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerExpr.cpp
//!
//! Implements class AST_IntegerExpr
//!
//===========================================================================

#include "AST_IntegerExpr.hpp"

using namespace Parsers;
using namespace mast;


//! Returns number as a BinaryVector
//!
//! @param targetCount  When not zero, gives the number of target bits
//!
BinaryVector AST_IntegerExpr::AsBinaryVector (uint32_t targetCount) const
{
  CHECK_PARAMETER_NOT_ZERO(targetCount, "Cannot convert to BinaryVector an integer expression without a size");

  auto value = Evaluate();

  BinaryVector asBinaryVector;
  asBinaryVector.Append(value);

  auto currentBitsCount = asBinaryVector.BitsCount();
  if (targetCount > currentBitsCount)
  {
    auto missingBitsCount = targetCount - currentBitsCount;
    asBinaryVector.PrependBits(missingBitsCount);
  }
  else if (targetCount < currentBitsCount)
  {
    auto excessiveBitsCount = currentBitsCount - targetCount;
    asBinaryVector.TruncateLeadingZeroes(excessiveBitsCount);
  }

  if (IsInverted())
  {
    asBinaryVector.ToggleBits();
  }

  return asBinaryVector;
}
//
//  End of: AST_IntegerExpr::AsBinaryVector
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_IntegerExpr.cpp
//===========================================================================
