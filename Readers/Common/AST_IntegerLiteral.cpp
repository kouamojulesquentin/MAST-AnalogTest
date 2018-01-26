//===========================================================================
//                           AST_IntegerLiteral.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerLiteral.cpp
//!
//! Implements class AST_IntegerLiteral
//!
//===========================================================================

#include "AST_IntegerLiteral.hpp"
#include "Utility.hpp"


#include <algorithm>

using std::string;
using namespace Parsers;
using namespace mast;


//! Initializes a literal expression
//!
//! @param integerText  Text representing the, unsigned, integer value
//!                     The underscore character (_) is legal anywhere in the number
//!                     except as the first character and is ignored.
//!                     The underscore character is used for readability purposes
//!
AST_IntegerLiteral::AST_IntegerLiteral (string&& integerText)
  : AST_IntegerExpr (Kind::IntegerLiteral)
{
  auto text = std::move(integerText);

  CHECK_PARAMETER_NOT_EMPTY(text, "Empty string is not a valid integer literal");

  CHECK_PARAMETER_FALSE(Utility::Contains(text, ' '), "Integral value cannot embed space(s)");
  CHECK_PARAMETER_FALSE(Utility::Contains(text, '-'), "Integral value cannot embed minus sign");

  // ---------------- Remove any embedded underscores (used as separators)
  //
  auto newEnd = std::remove(text.begin(), text.end(), '_');
  text.resize(newEnd - text.begin());

  try
  {
    auto tmpValue = std::stoul(text);
    CHECK_PARAMETER_LTE(tmpValue, UINT32_MAX, "Integer value \""s.append(text).append("\" is too large (limit is 32 bits)"));
    m_value = static_cast<uint32_t>(tmpValue);
  }
  catch(std::invalid_argument& exc)
  {
    CHECK_PARAMETER_FAILED(text, "Integer value \""s.append(text).append("\" contains invalid character(s)"));
  }
  catch(std::out_of_range & exc)
  {
    CHECK_PARAMETER_FAILED(text, "Integer value \""s.append(text).append("\" is too large (limit is 32 bits)"));
  }
}
//
//  End of: AST_IntegerLiteral::AST_IntegerLiteral
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_IntegerLiteral.cpp
//===========================================================================
