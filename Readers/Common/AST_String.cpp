//===========================================================================
//                           AST_String.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_String.cpp
//!
//! Implements class AST_String
//!
//===========================================================================

#include "AST_String.hpp"
using std::string;

using namespace Parsers;


//! Text representation of string
//!
string AST_String::AsText () const
{
  string asText("\"");

  for (const auto& character : m_content)
  {
    if (   (character == '\\')
        || (character == '"')
       )
    {
      asText.append("\\");
    }

    asText.push_back(character);
  }

  asText.append("\"");
  return asText;
}
//
//  End of: AST_String::AsText
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_String.cpp
//===========================================================================
