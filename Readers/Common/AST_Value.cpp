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
#include "AST_ConcatNumber.hpp"

using std::string;
using mast::BinaryVector;
using namespace Parsers;


//! Text representation of value
//!
string AST_Value::AsText () const
{
  return m_concatNumber->AsText();
}
//
//  End of: AST_Value::AsText
//---------------------------------------------------------------------------


//! Value as a BinaryVector
//!
BinaryVector AST_Value::AsBinaryVector (uint32_t targetSize) const
{
  return m_concatNumber->AsBinaryVector(targetSize);
}
//
//  End of: AST_Value::AsBinaryVector
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Value.cpp
//===========================================================================
