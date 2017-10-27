//===========================================================================
//                           AST_Identifier.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Identifier.cpp
//!
//! Implements class AST_Identifier
//!
//===========================================================================

#include "AST_Identifier.hpp"
#include <sstream>

using std::string;

using namespace Parsers;

//! Text representation of identifier with its index(es)
//!
string AST_Identifier::AsText () const
{
  std::ostringstream os;

  os << m_identifier;

  if (!IsScalar())
  {
    os << RangeAsText();
  }

  return os.str();
}

//
//  End of: AST_Identifier::AsText
//---------------------------------------------------------------------------


//! Text representation of only index(es)
//!
string AST_Identifier::RangeAsText () const
{
  std::ostringstream os;

  os << '[' << m_left;

  if (!m_right.empty())
  {
    os << ":" << m_right;
  }

  os << ']';
  return os.str();
}
//
//  End of: AST_Identifier::RangeAsText
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Identifier.cpp
//===========================================================================
