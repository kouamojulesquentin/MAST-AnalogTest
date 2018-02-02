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

using std::string;
using namespace Parsers;


//! Text representation of value
//!
//+template<> string AST_Value<bool>::AsText () const
//+{
//+  return m_value ? "true" : false;
//+}
//
//  End of: AST_Value::AsText
//---------------------------------------------------------------------------


//! Text representation of uint32_t value
//!
//+template<> string AST_Value<uint32_t>::AsText () const
//+{
//+  return std::to_string(m_value);
//+}
//
//  End of: AST_Value::AsText
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Value.cpp
//===========================================================================
