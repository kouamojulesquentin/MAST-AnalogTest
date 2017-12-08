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
#include "AST_String.hpp"
#include "AST_ParameterRef.hpp"

#include "Utility.hpp"
#include "EnumsUtility.hpp"

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
    os << AST_SimpleNode::AsText(m_stringsValue, ", ");
  }

  return os.str();
}
//
//  End of: AST_Attribute::AsText
//---------------------------------------------------------------------------


//! Text representation of attribute value
//!
string AST_Attribute::ValueAsText () const
{
  if (!m_numbersValue.empty())
  {
    return m_numbersValue;
  }

  if (!m_stringsValue.empty())
  {
    ostringstream os;
    auto          first = true;

    for (const auto node : m_stringsValue)
    {
      if   (!first) { os << "\n"; }
      else { first = false; }

      if      (node->IsKind(Kind::String))
      {
        auto asAST_String = dynamic_cast<const AST_String*>(node);
        os << asAST_String->Value();
      }
      else if (node->IsKind(Kind::Parameter_ref))
      {
        auto asAST_ParameterRef = dynamic_cast<const AST_ParameterRef*>(node);
        os << "$" << asAST_ParameterRef->Name();
      }
      else
      {
        CHECK_FAILED("Only support String and Parameter_Ref for attribute, got: "s.append(::ScopedNameString(node->GetKind())));
      }

    }
    return os.str();
  }

  return string();
}
//
//  End of: AST_Attribute::AsText
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Attribute.cpp
//===========================================================================
