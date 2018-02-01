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

#include "AST_Builder.hpp"
#include "AST_Attribute.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_String.hpp"
#include "AST_Parameter.hpp"
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

  if      (m_concatNumber != nullptr)
  {
    os << " = " << m_concatNumber->AsText();
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



//! Returns true when Attribute is defined using Parameter reference(s)
//!
bool AST_Attribute::HasParameterRef () const
{
  return AST_Parameter::HasParameterRef(m_stringsValue);
}
//
//  End of: AST_Attribute::HasParameterRef
//---------------------------------------------------------------------------



//! Replaces parameter references with their actual value, then resolve range expressions
//!
void AST_Attribute::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  AST_Parameter::ReplaceStringParameters(m_stringsValue, parameters);
}
//
//  End of: AST_Attribute::Resolve
//---------------------------------------------------------------------------


//! Uniquifies value or string explicit ression
//!
//! @note Only uniquifies parts that can be replaced during Parameter_Ref resolution
//! @note A Parameter Reference is never modified, when needed, it is replaced as a all by a Parameter Value
//!
//! @return Returns cloned AST_Attribute (with not shared members)
AST_Attribute* AST_Attribute::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto newAttribute = astBuilder.Clone_Attribute(this);

  return newAttribute;
}
//
//  End of: AST_Parameter::UniquifiedClone
//---------------------------------------------------------------------------


//! Text representation of attribute value
//!
string AST_Attribute::ValueAsText () const
{
  if (m_concatNumber != nullptr)
  {
    return m_concatNumber->AsText();
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
