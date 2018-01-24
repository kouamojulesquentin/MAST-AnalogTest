//===========================================================================
//                           AST_ScanMux.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanMux.cpp
//!
//! Implements class AST_ScanMux
//!
//===========================================================================

#include "AST_ScanMux.hpp"
#include "AST_Visitor.hpp"
#include "AST_Builder.hpp"
#include "Utility.hpp"

using std::vector;
using namespace Parsers;


//! Initializes AST_ScanMux
//!
//! @param identifier           ScanMux identifier
//! @param selectors            Selection signals that are used to drive the ScanMux
//! @param scanMuxSelections    Selections definition i.e. which value(s) select which signal(s)
//!
AST_ScanMux::AST_ScanMux (AST_VectorIdentifier*           identifier,
                          vector<Parsers::AST_Signal*>&&  selectors,
                          vector<AST_ScanMuxSelection*>&& scanMuxSelections)
  : AST_ParentNode (Kind::ScanMux)
  , m_identifier   (identifier)
  , m_selectors    (std::move(selectors))
  , m_selections   (std::move(scanMuxSelections))
{
  CHECK_PARAMETER_NOT_NULL(identifier,         "ScanMux must have a valid identifier");
  CHECK_PARAMETER_NOT_EMPTY(m_selectors,       "ScanMux must have at least one selector");
  CHECK_PARAMETER_GTE(m_selections.size(), 2u, "ScanMux must have at least two defined selection");

  DispatchChildren();
}
//
//  End of: AST_ScanMux::AST_ScanMux
//---------------------------------------------------------------------------




//! Visited part of the Visitor pattern
//!
void AST_ScanMux::Accept (AST_Visitor& visitor)
{
  visitor.Visit_ScanMux(this);
}
//
//  End of: AST_ScanMux::Accept
//---------------------------------------------------------------------------


//! Does nothing
//!
void AST_ScanMux::DispatchChildren ()
{
}
//
//  End of: AST_ScanMux::DispatchChildren
//---------------------------------------------------------------------------



//! Tells whether this ScanMux multiplexes more than one bit
//!
bool AST_ScanMux::IsBusMux () const
{
  return !m_identifier->IsSingleBit();
}
//
//  End of: AST_ScanMux::IsBusMux
//---------------------------------------------------------------------------


//! Replaces parameter references with their actual value, then resolves value expressions
//!
//! @param parameters   Actual parameter values - There should be no parameter reference in their values
//!
void AST_ScanMux::Resolve (const vector<AST_Parameter*>& parameters)
{
  m_identifier->Resolve(parameters);
}
//
//  End of: AST_ScanMux::Resolve
//---------------------------------------------------------------------------


//! Returns uniquified clone of value or string expression
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_ScanMux
AST_ScanMux* AST_ScanMux::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto cloned = astBuilder.Clone_ScanMux(this);

  if (m_identifier->HasParameterRef())
  {
    cloned->m_identifier = m_identifier->UniquifiedClone(astBuilder);
  }

  //! @todo [JFC]-[January/22/2018]: In UniquifiedClone(): Complete it
  //!
//+  UniquifiedCloneItemsWithParameterRef(clone->m_selectors, astBuilder);
//+  UniquifiedCloneItemsWithParameterRef(clone->m_selections, astBuilder);

  return cloned;
}
//
//  End of: AST_ScanMux::UniquifiedClone
//---------------------------------------------------------------------------

//===========================================================================
// End of AST_ScanMux.cpp
//===========================================================================
