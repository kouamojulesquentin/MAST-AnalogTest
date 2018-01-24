//===========================================================================
//                           AST_Instance.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Instance.cpp
//!
//! Implements class AST_Instance
//!
//===========================================================================

#include "AST_Identifier.hpp"
#include "AST_Instance.hpp"
#include "AST_Attribute.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_Visitor.hpp"
#include "AST_Builder.hpp"

using std::string;
using std::vector;

using namespace Parsers;


//! Visited part of the Visitor pattern
//!
void AST_Instance::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Instance(this);
}


//! Dispatches children to specific member (for ease of use)
//!
void AST_Instance::DispatchChildren ()
{
  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute: AST_ParentNode::AppendChild(child, m_attributes); break;
        case Parsers::Kind::Parameter: AST_ParentNode::AppendChild(child, m_parameters); break;
        case Parsers::Kind::InputPort: AST_ParentNode::AppendChild(child, m_inputPorts); break;

        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Instance::DispatchChildren
//---------------------------------------------------------------------------



//! Searches for a InputPort with specified identifier
//!
//! @param identifier   An identifier for InputPort to find
//!
const AST_Port* AST_Instance::FindInputPort (const AST_Identifier* identifier) const
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find InputPort from nullptr identifier");

  return FindNode(m_inputPorts, identifier);
}
//
//  End of: AST_Instance::FindInputPort
//---------------------------------------------------------------------------


//! Returns instance name (in enclosing module)
//!
string AST_Instance::Name () const
{
  return m_instanceIdentifier->AsText();
}
//
//  End of: AST_Instance::Name
//---------------------------------------------------------------------------


//! Replaces parameter references with their actual value, then resolve value expressions
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param parameters   Actual parameter values from embedding module - There should be no parameter reference in their values
//!
void AST_Instance::Resolve (AST_Builder& astBuilder, const vector<AST_Parameter*>& moduleParameters)
{
  // ---------------- For local Parameter Reference, use both instance parameters and "inherited" parameter from embedding module
  //
  auto mergedParameters = m_parameters;
  mergedParameters.insert(mergedParameters.end(), moduleParameters.cbegin(), moduleParameters.cend());

  AST_Parameter::ResolveItems(m_parameters, mergedParameters, astBuilder);
  AST_Parameter::ResolveItems(m_attributes, mergedParameters); //+, astBuilder);
  AST_Parameter::ResolveItems(m_inputPorts, mergedParameters);
}
//
//  End of: AST_Instance::Resolve
//---------------------------------------------------------------------------



//! Uniquifies instance
//!
//! @note Unification consist to have a single object representing that particular instance.
//!       The result can be modified without affecting any other Module/Instance
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New and unique AST_Instance
AST_Instance* AST_Instance::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto clone = astBuilder.Clone_Instance(this);

  UniquifyItemsWithParameterRef(clone->m_parameters, astBuilder);
  UniquifyItemsWithParameterRef(clone->m_attributes, astBuilder);
  UniquifyItemsWithParameterRef(clone->m_inputPorts, astBuilder);

  return clone;
}
//
//  End of: AST_Instance::UniquifiedClone
//---------------------------------------------------------------------------



//! Sets unique module representing that very instance
//!
//! @note It must be call only once by the unification process
//!
void AST_Instance::UniquifiedModule (AST_Module* uniquifiedModule, const AST_ModuleIdentifier* identifier)
{
  m_uniquifiedModule = uniquifiedModule;
  m_moduleIdentifier = identifier;
}
//
//  End of: AST_Instance::UniquifiedModule
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_Instance.cpp
//===========================================================================
