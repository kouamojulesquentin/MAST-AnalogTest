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
//! @param numbersValue Numbers that define parameter value
//!
AST_Parameter::AST_Parameter (Kind kind, string&& name, string&& numbersValue)
  : AST_SimpleNode (kind)
  , m_name         (std::move(name))
  , m_numbersValue (std::move(numbersValue))
{
  CHECK_PARAMETER_NOT_EMPTY(m_name,         "Parameter definition must have a valid, not empty, name");
  CHECK_PARAMETER_NOT_EMPTY(m_numbersValue, "Parameter definition must have a valid, not empty, number or number expression");
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
  if (!m_numbersValue.empty())
  {
    return Utility::Contains(m_numbersValue, '$');
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



//! Returns a copy of number expression with parameters references replaces with the actual parameter value
//!
//! @param numberValue  Number expression with some parameter reference(s)
//! @param parameters   Parameters value (they are supposed to be resolved i.e. do not contain parameter references)
//!
string AST_Parameter::ReplacedParameters (const string& numberValue, const vector<AST_Parameter*>& parameters)
{
  CHECK_PARAMETER_NOT_EMPTY(numberValue, "Number value must be non empty");

  // Lamba: Creates a string_view from result sub-match
  auto makeStringView = [](const auto& subMatch)
  {
    return string_view(subMatch.first, subMatch.length());
  };

  auto   newNumberValue = numberValue;
  regex  paramRefRegex( R"(\$(\w+))");
  cmatch searchResults;

  size_t searchOffset = 0;

  while (regex_search(&*newNumberValue.cbegin() + searchOffset, &*newNumberValue.cend(), searchResults, paramRefRegex))
  {
    CHECK_VALUE_EQ(searchResults.size(), 2u, "Houps successful match of parameter_ref must have exactly 2 sub-matches");

    auto parameterNameSubMatch = searchResults[1u];
    auto parameterName         = string_view(parameterNameSubMatch.first, parameterNameSubMatch.length());
    auto parameter             = LocateParameterDef(parameterName, parameters);

    CHECK_PARAMETER_NOT_NULL(parameter, "Houps: LocateParameterDef must return a valid (not nullptr) parameter definition");

    CHECK_TRUE(parameter->IsNumber(), "Parameter reference \""s.append(parameterName.cbegin(), parameterName.cend()).append("\" must refer to a parameter of type number"));

    // ---------------- Replace parameter reference with parameter value
    //
    const auto  prefix         = makeStringView(searchResults.prefix());
    const auto& parameterValue = parameter->m_numbersValue;
    const auto  suffix         = makeStringView(searchResults.suffix());

    string newValue(newNumberValue.cbegin(), newNumberValue.cbegin() + searchOffset);

    newValue.append(prefix.cbegin(), prefix.cend());
    newValue.append(parameterValue);
    newValue.append(suffix.cbegin(), suffix.cend());
    newNumberValue = newValue;

    // ---------------- Update search range
    //
    searchOffset += prefix.length() + parameterValue.length();
  }

  return newNumberValue;
}
//
//  End of: AST_Parameter::ReplacedParameters
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
//! @param           astBuilder Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param [in, out] parameters Parameter definitions to use to replace parameter reference(s)
//!
void AST_Parameter::Resolve (AST_Builder& astBuilder, vector<AST_Parameter*>& parameters)
{
  CHECK_FALSE(m_parameterRefResolutionInProgress, "There is a circular parameter reference with parameter \""s.append(m_name).append("\""));

  AT_SCOPE_EXIT([&]() { m_parameterRefResolutionInProgress = false; });
  m_parameterRefResolutionInProgress = true;

  if (!m_numbersValue.empty())
  {
    Resolve(astBuilder, m_numbersValue, parameters);
//+    m_numbersValue = ReducedValueExpr(m_numbersValue);
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
//! @param            astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param [in, out]  numberValue  Number expression containing Parameter reference(s)
//! @param [in, out]  parameters   Parameters actual values
//!
void AST_Parameter::Resolve (AST_Builder&            astBuilder,
                             string&                 numberValue,
                             vector<AST_Parameter*>& parameters)
{
  CHECK_PARAMETER_NOT_EMPTY(numberValue, "Number value must be non empty");

  // Lamba: Creates a string_view from result sub-match
  auto makeStringView = [](const auto& subMatch)
  {
    return string_view(subMatch.first, subMatch.length());
  };

  regex  paramRefRegex(R"(\$(\w+))");
  cmatch results;
  string newValue;  // Will accumulate stuff before Parameter Reference and Parameter actual value
  size_t searchOffset = 0;

  while (regex_search(&*numberValue.cbegin() + searchOffset, &*numberValue.cend(), results, paramRefRegex) && !results.empty())
  {
    CHECK_VALUE_EQ(results.size(), 2u, "Houps successful match of parameter_ref must have exactly 2 sub-matches");

    auto parameterNameSubMatch = results[1u];
    auto parameterName         = string_view(parameterNameSubMatch.first, parameterNameSubMatch.length());
    auto parameter             = LocateParameterDef(parameterName, parameters);

    CHECK_PARAMETER_NOT_NULL(parameter, "Houps: LocateParameterDef must return a valid (not nullptr) parameter definition");

    CHECK_TRUE(parameter->IsNumber(), "Parameter reference \""s.append(parameterName.cbegin(), parameterName.cend()).append("\" must refer to a parameter of type number"));

    if (parameter->HasParameterRef())
    {
      parameter->Resolve(astBuilder, parameters);
    }

    // ---------------- Replace parameter reference with parameter value
    //
    const auto  prefix         = makeStringView(results.prefix());
    const auto& parameterValue = parameter->m_numbersValue;

    newValue.append(prefix.cbegin(), prefix.cend());
    newValue.append(parameterValue);

    searchOffset += prefix.length() + parameterName.length() + 1u;  // +1 is for '$' character
  }

  // ---------------- Accumulate remaining stuff
  //
  if (searchOffset != 0)
  {
    newValue.append(numberValue.cbegin() + searchOffset, numberValue.cend());
    numberValue = newValue;
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
//! @return Returns cloned AST_Parameter (with not shared members)
AST_Parameter* AST_Parameter::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto newParameter = astBuilder.Clone_Parameter(this);

  return newParameter;
}
//
//  End of: AST_Parameter::UniquifiedClone
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_Parameter.cpp
//===========================================================================
