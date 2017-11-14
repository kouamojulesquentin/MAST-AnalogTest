//===========================================================================
//                           AST_Value.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Value.cpp
//!
//! Implements class AST_Value
//!
//===========================================================================

#include "AST_Value.hpp"

using mast::BinaryVector;
using namespace Parsers;



//! Value as a BinaryVector
//!
BinaryVector AST_Value::AsBinaryVector () const
{
  return BinaryVector::CreateFromString(m_valueExpression);
}
//
//  End of: AST_Value::AsBinaryVector
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Value.cpp
//===========================================================================
