//===========================================================================
//                           AST_VectorIdentifier.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_VectorIdentifier.cpp
//!
//! Implements class AST_VectorIdentifier
//!
//===========================================================================

#include "AST_VectorIdentifier.hpp"
#include <sstream>

using std::string;

using namespace Parsers;

//! Text representation of identifier with its index(es)
//!
string AST_VectorIdentifier::AsText () const
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
//  End of: AST_VectorIdentifier::AsText
//---------------------------------------------------------------------------


//! Text representation of only index(es)
//!
string AST_VectorIdentifier::RangeAsText () const
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
//  End of: AST_VectorIdentifier::RangeAsText
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_VectorIdentifier.cpp
//===========================================================================
