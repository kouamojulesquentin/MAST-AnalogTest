//===========================================================================
//                           AST_Alias.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Alias.cpp
//!
//! Implements class AST_Alias
//!
//===========================================================================

#include "AST_Alias.hpp"
#include "AST_Attribute.hpp"
#include "AST_Alias.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_EnumRef.hpp"
#include "AST_Signal.hpp"
#include "AST_Value.hpp"
#include "AST_VectorIdentifier.hpp"
#include "AST_Visitor.hpp"
#include "Utility.hpp"

using std::vector;
using std::string;

using namespace mast;
using namespace Parsers;



//! Initializes AST_Alias
//!
//! @param aliasIdentifier    Alias identifier
//! @param signals            Aliased signal(s)
//! @param optionalChildren   Optional parts
//!
AST_Alias::AST_Alias (AST_VectorIdentifier* aliasIdentifier,
                      vector<AST_Signal*>&& signals,
                      vector<AST_Node*>&&   optionalChildren)
  : AST_ParentNode (Kind::Alias, std::move(optionalChildren))
  , m_identifier   (aliasIdentifier)
  , m_signals      (std::move(signals))
{
  CHECK_PARAMETER_NOT_NULL(m_identifier, "Alias must be constructed with a valid not nullptr identifier");
  CHECK_PARAMETER_NOT_EMPTY(m_signals,   FoundAliasMessage().append("defined with no signals"));

  // ---------------- Check that no signal is nullptr and is not a number
  //
  for (auto signal : signals)
  {
    CHECK_PARAMETER_NOT_NULL (signal,             FoundAliasMessage().append("with a not valid (nullptr) signal"));
    CHECK_PARAMETER_FALSE    (signal->IsNumber(), FoundAliasMessage().append("with a signal that is a number (instead to referring to a port)"));
  }

  // ---------------- Assign other, recognized, members
  //
  DispatchChildren();
  CleanupChildren();
}
//
//  End of: AST_Alias::AST_Alias
//---------------------------------------------------------------------------


//! Visited part of the Visitor pattern
//!
void AST_Alias::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Alias(this);
}
//
//  End of AST_Alias::Accept
//---------------------------------------------------------------------------




//! Dispatches optional parts to specific members
//!
void AST_Alias::DispatchChildren ()
{
  for (auto&  child:  UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute:
          AST_ParentNode::AppendChild(child, m_attributes);
          break;
        case Parsers::Kind::RefEnum:
          CHECK_VALUE_NULL(m_enumRef, FoundAliasMessage().append("with more than one EnumRef (there shall be at most one alias_refEnum)"));
          SetChild(child, m_enumRef);
          break;
        case Parsers::Kind::PlaceHolder:
        {
          auto placeHolder = static_cast<AST_PlaceHolderBase*>(child);
          auto targetKind  = placeHolder->TargetKind();

          switch (targetKind)
          {
            case Parsers::Kind::ApplyEndState :
              CHECK_VALUE_NULL(m_iApplyEndState, FoundAliasMessage().append("with more than one iApplyEndState (there shall be at most one iApplyEndState in a single Alias statement)"));
              SetChild(child, m_iApplyEndState);
              break;
            case Parsers::Kind::AccessTogether :
            {
              CHECK_FALSE(m_accessTogether, FoundAliasMessage().append("with more than one AccessTogether statement"));
              AST_Value<bool>* accessTogetherNode = nullptr;
              SetChild(child, accessTogetherNode);
              m_accessTogether = accessTogetherNode->Value();
              break;
            }
            default: // Ignore all other for now
              break;
          }
          child = nullptr;  // "Discard" all other place holders
          break;
        }
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Alias::DispatchChildren
//---------------------------------------------------------------------------




//! Constructs part of an error message reporting alias name
//!
string AST_Alias::FoundAliasMessage () const
{
  return "Found Alias \""s.append(m_identifier->Name()).append("\" ");
}
//
//  End of: AST_Alias::FoundAliasMessage
//---------------------------------------------------------------------------


//! Returns true when there is a parameter reference in any part of alias
//!
bool AST_Alias::HasParameterRef () const
{
  // ---------------- Identifier
  //
  if ((m_identifier != nullptr) && m_identifier->HasParameterRef())
  {
    return true;
  }

  // ---------------- iApplyEndState
  //
  if ((m_iApplyEndState != nullptr) && m_iApplyEndState->HasParameterRef())
  {
    return true;
  }

  // ---------------- Signals
  //
  for (const auto signal : m_signals)
  {
    if (signal->HasParameterRef())
    {
      return true;
    }
  }

  // ---------------- Attributes
  //
  for (const auto attribute : m_attributes)
  {
    if (attribute->HasParameterRef())
    {
      return true;
    }
  }

  return false;
}
//
//  End of: AST_Alias::HasParameterRef
//---------------------------------------------------------------------------


//! Returns alias identifier as text
//!
string AST_Alias::Name () const
{
  return m_identifier->AsText();
}
//
//  End of: AST_Alias::Name
//---------------------------------------------------------------------------


//! Replaces parameter references by its actual value
//!
//! @note This must be called after unification pass
//!
//! @param parameters   Parameter definitions to use to replace parameter reference(s)
//!
void AST_Alias::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  // ---------------- Identifier
  //
  if ((m_identifier != nullptr) && m_identifier->HasParameterRef())
  {
    m_identifier->Resolve(parameters);
  }

  // ---------------- iApplyEndState
  //
  if ((m_iApplyEndState != nullptr) && m_iApplyEndState->HasParameterRef())
  {
    m_iApplyEndState->Resolve(parameters);
  }

  // ---------------- Signals
  //
  for (auto signal : m_signals)
  {
    if (signal->HasParameterRef())
    {
      signal->Resolve(parameters);
    }
  }

  // ---------------- Attributes
  //
  for (auto attribute : m_attributes)
  {
    if (attribute->HasParameterRef())
    {
      attribute->Resolve(parameters);
    }
  }

}
//
//  End of: AST_Alias::Resolve
//---------------------------------------------------------------------------



//! Returns uniquified clone
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_Alias
AST_Alias* AST_Alias::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto clone = astBuilder.Clone_Alias(this);

  // ---------------- Identifier
  //
  if (m_identifier != nullptr)
  {
    clone->m_identifier = m_identifier->UniquifiedClone(astBuilder);
  }

  // ---------------- iApplyEndState
  //
  if (m_iApplyEndState != nullptr)
  {
    clone->m_iApplyEndState = m_iApplyEndState->UniquifiedClone(astBuilder);
  }

  // ---------------- Signals
  //
  for (auto& signal : clone->m_signals)
  {
    signal = signal->UniquifiedClone(astBuilder);
  }

  // ---------------- Attributes
  //
  for (auto& attribute : clone->m_attributes)
  {
    attribute = attribute->UniquifiedClone(astBuilder);
  }

  return clone;
}
//
//  End of: AST_Alias::UniquifiedClone
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Alias.cpp
//===========================================================================
