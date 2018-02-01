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

#include "AST_Builder.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_String.hpp"
#include "Utility.hpp"

#include <sstream>
#include <algorithm>
#include <regex>

using std::vector;
using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::regex;
using std::smatch;
using std::cmatch;

using namespace mast;
using namespace Parsers;



//! Initializes AST_Parameter representing a number
//!
//! @param kind         Either Parameter or LocalParameter
//! @param name         Parameter name
//! @param concatNumber Numbers that define parameter value
//!
AST_Parameter::AST_Parameter (Kind kind, string&& name, AST_ConcatNumber* concatNumber)
  : AST_SimpleNode (kind)
  , m_name         (std::move(name))
  , m_concatNumber (concatNumber)
{
  CHECK_PARAMETER_NOT_EMPTY (m_name,         "Parameter definition must have a valid, not empty, name");
  CHECK_PARAMETER_NOT_NULL  (m_concatNumber, "Parameter definition must have a valid, not empty, number, number expression or numbers concatenation");
}
//
//  End of: AST_Parameter::AST_Parameter
//---------------------------------------------------------------------------



//! Initializes AST_Parameter representing a string
//!
//! @param kind               Either Parameter or LocalParameter
//! @param name               Parameter name
//! @param stringsOrRefsValue Strings and/or parameter ref that define parameter value
//!                           Should be AST_String and/or AST_ParameterRef
//!
AST_Parameter::AST_Parameter (Kind kind, string&& name, vector<AST_SimpleNode*>&& stringsOrRefsValue)
  : AST_SimpleNode (kind)
  , m_name         (std::move(name))
  , m_stringsValue (stringsOrRefsValue.cbegin(), stringsOrRefsValue.cend())
{
  CHECK_PARAMETER_NOT_EMPTY(m_name,         "Parameter definition must have a valid, not empty, name");
  CHECK_PARAMETER_NOT_EMPTY(m_stringsValue, "Parameter definition must have a valid, not empty, string and/or parameter reference(s)");
}
//
//  End of: AST_Parameter::AST_Parameter
//---------------------------------------------------------------------------


//! Text representation of attribute
//!
string AST_Parameter::AsText () const
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
//  End of: AST_Parameter::AsText
//---------------------------------------------------------------------------



//! Returns true when some string has Parameter reference(s)
//!
//! @param value  Value to check for parameter reference
//!
bool AST_Parameter::HasParameterRef (const string& value)
{
  return Utility::Contains(value, '$');
}
//
//  End of: AST_Parameter::HasParameterRef
//---------------------------------------------------------------------------



//! Returns true when Parameter is defined using Parameter reference(s)
//!
bool AST_Parameter::HasParameterRef () const
{
  // ---------------- When represents a number
  //
  if (m_concatNumber != nullptr)
  {
    return m_concatNumber->HasParameterRef();
  }

  // ---------------- When represents a string
  //
  return HasParameterRef(m_stringsValue);
}
//
//  End of: AST_Parameter::HasParameterRef
//---------------------------------------------------------------------------


//! Locates a parameter definition with given name
//!
//! @param name         Name of parameter to locate
//! @param parameters   Set of parameters definition
//!
//! @return Found AST_Parameter (throw in case of failure)
AST_Parameter* AST_Parameter::LocateParameterDef (string_view name, const vector<AST_Parameter*>& parameters)
{
  auto pos = std::find_if(parameters.begin(), parameters.end(), [name](auto iter) { return iter->Name() == name; });

  CHECK_VALUE_NEQ(pos, parameters.end(), "Parameter reference \""s.append(name.cbegin(), name.cend()).append("\" cannot be resolved with a parameter value"));

  auto parameter = *pos;

  return parameter;
}
//
//  End of: AST_Parameter::LocateParameterDef
//---------------------------------------------------------------------------



//! Computes string concatenation from a collection of AST_String nodes
//!
string AST_Parameter::ReducedStringExpr (const vector<AST_SimpleNode*>& stringsValue)
{
  string result;

  for (auto node : stringsValue)
  {
    CHECK_TRUE(node->IsKind(Kind::String), "Cannot reduced string expression from nodes not representing a string chunk");

    auto stringNode = static_cast<const AST_String*>(node);

    result.append(stringNode->Value());
  }

  return result;
}
//
//  End of: AST_Parameter::ReducedStringExpr
//---------------------------------------------------------------------------



//! Replaces parameter references nodes with parameter string value
//!
//! @param nodes        Collection with parameter references nodes to replace with string parameter value
//! @param parameters   Actual parameter values
//!
void AST_Parameter::ReplaceStringParameters (vector<AST_SimpleNode*>& nodes, const vector<AST_Parameter*>& parameters)
{
  for (auto& node : nodes)
  {
    if (node->IsKind(Kind::Parameter_ref))
    {
      auto paramRef  = static_cast<const AST_ParameterRef*>(node);
      auto parameter = LocateParameterDef(paramRef->Name(), parameters);

      CHECK_TRUE(parameter->IsString(), "Parameter reference \""s.append(paramRef->Name()).append("\" must refer to a parameter of type string"));
      CHECK_VALUE_EQ(parameter->m_stringsValue.size(), 1u, "Houps: Parameter of type string is expected to having be reduced to single string");

      auto reducedValue = parameter->m_stringsValue.front();
      CHECK_VALUE_EQ(reducedValue->GetKind(), Kind::String, "Houps: Parameter of type string is expected to having be reduced as string");

      node = reducedValue;
    }
  }
}
//
//  End of: AST_Parameter::ReplaceStringParameters
//---------------------------------------------------------------------------




//! Replaces parameter references used by this parameter (solving transitive definitions)
//!
//! @param           astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param [in, out] parameters   Parameter definitions to use to replace parameter reference(s)
//!
void AST_Parameter::Resolve (AST_Builder& astBuilder, vector<AST_Parameter*>& parameters)
{
  CHECK_FALSE(m_parameterRefResolutionInProgress, "There is a circular parameter reference with parameter \""s.append(m_name).append("\""));

  AT_SCOPE_EXIT([&]() { m_parameterRefResolutionInProgress = false; });
  m_parameterRefResolutionInProgress = true;

  if (m_concatNumber != nullptr)
  {
    m_concatNumber->Resolve(parameters);
  }
  else if (!m_stringsValue.empty())
  {
    Resolve(astBuilder, m_stringsValue, parameters);
    auto reducedString = ReducedStringExpr(m_stringsValue);
    auto stringNode    = astBuilder.Create_String(std::move(reducedString));

    m_stringsValue.clear();
    m_stringsValue.push_back(stringNode);
  }
}
//
//  End of: AST_Parameter::Resolve
//---------------------------------------------------------------------------


//! Replaces parameter references within string definition with referred parameter value
//!
//! @param            astBuilder Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param [in, out]  stringsValue   Nodes defining a string value
//! @param [in, out]  parameters     Parameters actual values
//!
void AST_Parameter::Resolve (AST_Builder&             astBuilder,
                             vector<AST_SimpleNode*>& stringsValue,
                             vector<AST_Parameter*>&  parameters)
{
  vector<AST_SimpleNode*> newStringsValue;

  for (auto node : stringsValue)
  {
    if (!node->IsKind(Kind::Parameter_ref))
    {
      newStringsValue.push_back(node);
    }
    else
    {
      auto paramRef = static_cast<const AST_ParameterRef*>(node);

      auto pos = std::find_if(parameters.begin(), parameters.end(), [name = paramRef->Name()](auto iter) { return iter->Name() == name; });
      CHECK_VALUE_NEQ(pos, parameters.end(), "Parameter reference \""s.append(paramRef->Name())
                                                                      .append("\" cannot be resolved with a parameter value.\n")
                                                                      .append("The parameter_def included in a parameter_override "
                                                                              "element shall match both the name and type of a "
                                                                              "parameter_def within the module being instantiated"));

      auto parameter = *pos;
      CHECK_TRUE(parameter->IsString(), "Parameter reference \""s.append(paramRef->Name()).append("\" must refer to a parameter of type string"));

      if ((parameter->m_stringsValue.size() > 1u) || parameter->HasParameterRef())
      {
        parameter->Resolve(astBuilder, parameters);
      }

      newStringsValue.insert(newStringsValue.end(), parameter->m_stringsValue.cbegin(), parameter->m_stringsValue.cend());
    }
  }

  stringsValue = std::move(newStringsValue);
}
//
//  End of: AST_Parameter::ResolveString
//---------------------------------------------------------------------------



//! Uniquifies value or string explicit ression
//!
//! @note Only uniquifies parts that can be replaced during Parameter_Ref resolution
//! @note A Parameter Reference is never modified, when needed, it is replaced as a all by a Parameter Value
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return Returns cloned AST_Parameter (with not shared members)
AST_Parameter* AST_Parameter::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto newParameter = astBuilder.Clone_Parameter(this);

  if (m_concatNumber != nullptr)
  {
    newParameter->m_concatNumber = m_concatNumber->UniquifiedClone(astBuilder);
  }
  return newParameter;
}
//
//  End of: AST_Parameter::UniquifiedClone
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_Parameter.cpp
//===========================================================================
