//===========================================================================
//                           AST_Signal.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Signal.cpp
//!
//! Implements class AST_Signal
//!
//===========================================================================

#include "AST_Signal.hpp"
#include "AST_ScalarIdentifier.hpp"
#include <sstream>

using std::ostringstream;
using std::string;

using namespace Parsers;

//! Text representation of signal
//!
string AST_Signal::AsText () const
{
  ostringstream os;

  if (IsInverted())
  {
    os << "~";
  }

  if (IsNumber())
  {
    os << m_number;
  }
  else
  {
    for (const auto& instanceName : m_path)
    {
      os << instanceName->AsText() << ".";
    }

    os << m_portName->AsText();
  }

  return os.str();
}
//
//  End of: AST_Signal::AsText
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Signal.cpp
//===========================================================================
