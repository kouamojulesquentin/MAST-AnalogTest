//===========================================================================
//                           AST_Source.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Source.cpp
//!
//! Implements class AST_Source
//!
//===========================================================================

#include "AST_Source.hpp"
#include "AST_Signal.hpp"

#include <sstream>

using std::ostringstream;
using std::string;
using namespace Parsers;


//! Text representation of source
//!
string AST_Source::AsText () const
{
  ostringstream os;

  auto isFirst = true;
  for (const auto signal : m_signals)
  {
    if (isFirst)
    {
      isFirst = false;
    }
    else
    {
      os << ", ";
    }
    os << signal->AsText();
  }

  return os.str();
}
//
//  End of: AST_Source::AsText
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Source.cpp
//===========================================================================
