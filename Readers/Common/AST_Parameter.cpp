//===========================================================================
//                           AST_Parameter.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Parameter.cpp
//!
//! Implements class AST_Parameter
//!
//===========================================================================

#include "AST_Parameter.hpp"
#include "Utility.hpp"

#include <sstream>

using std::string;
using std::ostringstream;

using namespace Parsers;



//! Text representation of attribute
//!
string AST_Parameter::AsText () const
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
    os << AST_SimpleNode::AsText(m_stringsValue, ", ");
  }

  return os.str();
}
//
//  End of: AST_Parameter::AsText
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Parameter.cpp
//===========================================================================
