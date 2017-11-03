//===========================================================================
//                           AST_Attribute.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Attribute.cpp
//!
//! Implements class AST_Attribute
//!
//===========================================================================

#include "AST_Attribute.hpp"
#include "Utility.hpp"

#include <sstream>

using std::string;
using std::ostringstream;

using namespace Parsers;



//! Text representation of attribute
//!
string AST_Attribute::AsText () const
{
  ostringstream os;

  os << m_name;

  if      (!m_numbersValue.empty())
  {
    os << " = " << m_numbersValue;
  }
  else if (!m_stringsValue.empty())
  {
    os << " = ";

    auto first = true;
    for (const auto node : m_stringsValue)
    {
      CHECK_VALUE_NOT_NULL(node, "Expected to have not nullptr string or parameter ref");
      if (!first)
      {
        os << ", ";
      }
      else
      {
        first = false;
      }
      os << node->AsText();
    }
  }

  return os.str();
}
//
//  End of: AST_Attribute::AsText
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Attribute.cpp
//===========================================================================
