//===========================================================================
//                           AST_Namespace.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Namespace.cpp
//!
//! Implements class AST_Namespace
//!
//===========================================================================

#include "AST_Namespace.hpp"

using std::string;

using namespace Parsers;

//! Text representation of value
//!
string AST_Namespace::AsText () const
{
  auto asText = m_name;
  asText.append("::");

  return asText;
}
//
//  End of: AST_Namespace::AsText
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_Namespace.cpp
//===========================================================================
