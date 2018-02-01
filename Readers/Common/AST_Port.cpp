//===========================================================================
//                           AST_Port.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Port.cpp
//!
//! Implements class AST_Port
//!
//===========================================================================

#include "AST_Attribute.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_Visitor.hpp"
#include "AST_Builder.hpp"

using std::string;
using std::vector;

using namespace Parsers;


//! Initializes an AST_Port with just an identifier
//!
//! @param kind         Kind of port
//! @param identifier   Port identifier
AST_Port::AST_Port (Kind kind, AST_VectorIdentifier* identifier)
  : AST_ParentNode (kind)
  , m_identifier   (CHECK_PARAMETER_NOT_NULL(identifier, "Port must be initialized with a valid (not nullptr) identifier"))
{
}
//
//  End of: AST_Port::AST_Port
//---------------------------------------------------------------------------



//! Initializes an AST_Port with identifier and children nodes
//!
//! @param kind         Kind of port
//! @param identifier   Port identifier
//! @param children     Port children
//!
AST_Port::AST_Port (Kind kind, AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children)
  : AST_ParentNode (kind, std::move(children))
  , m_identifier   (CHECK_PARAMETER_NOT_NULL(identifier, "Port must be initialized with a valid (not nullptr) identifier"))
{
  DispatchChildren();
  CleanupChildren();
}
//
//  End of: AST_Port::AST_Port
//---------------------------------------------------------------------------





//! Visited part of the Visitor pattern
//!
void AST_Port::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Port(this);
}
//
//  End of: AST_Port::Accept
//---------------------------------------------------------------------------


//! Dispatches children to specific member (for ease of use)
//!
void AST_Port::DispatchChildren ()
{
  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute :
          AST_ParentNode::AppendChild(child, m_attributes);
          break;
        case Parsers::Kind::Source :
        case Parsers::Kind::InputPortSource :
          AST_ParentNode::SetChild(child, m_source);
          break;
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Port::DispatchChildren
//---------------------------------------------------------------------------


//! Returns true when Parameter is defined using Parameter reference(s)
//!
//! @note This is useful to know when it should be uniquified
bool AST_Port::HasParameterRef () const
{
  if (m_identifier->HasParameterRef())
  {
    return true;
  }

  if (AST_Parameter::HasParameterRef(m_attributes))
  {
    return true;
  }

  return false;
}
//
//  End of: AST_Port::HasParameterRef
//---------------------------------------------------------------------------


//! Replaces parameter references with their actual value, then resolve value expressions
//!
//! @param parameters   Actual parameter values - There should be no parameter reference in their values
//!
void AST_Port::Resolve (const vector<AST_Parameter*>& parameters)
{
  m_identifier->Resolve(parameters);

  if (m_source)
  {
//+    m_source->Resolve(parameters);
  }

  AST_Parameter::ResolveItems(m_attributes, parameters);
}
//
//  End of: AST_Port::Resolve
//---------------------------------------------------------------------------



//! Returns uniquified clone of value or string expression
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_Port
AST_Port* AST_Port::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto cloned = astBuilder.Clone_Port(this);

  if (m_identifier->HasParameterRef())
  {
    cloned->m_identifier = m_identifier->UniquifiedClone(astBuilder);
  }

  if (m_source && m_source->HasParameterRef())
  {
//+    cloned->m_source = m_source->UniquifiedClone();
  }

  UniquifyItemsWithParameterRef(cloned->m_attributes, astBuilder);

  return cloned;
}
//
//  End of: AST_Port::UniquifiedClone
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Port.cpp
//===========================================================================
