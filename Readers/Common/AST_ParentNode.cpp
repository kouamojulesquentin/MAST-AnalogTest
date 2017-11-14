//===========================================================================
//                           AST_ParentNode.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ParentNode.cpp
//!
//! Implements class AST_ParentNode
//!
//===========================================================================

#include "AST_ParentNode.hpp"
#include "Utility.hpp"

using std::vector;

using namespace Parsers;


//! Initializes with children
//!
AST_ParentNode::AST_ParentNode (Kind kind, vector<AST_Node*>&& children)
  : AST_NamedNode          (kind)
  , m_undispatchedChildren (std::move(children))
{
  for (const auto node : m_undispatchedChildren)
  {
    CHECK_PARAMETER_NOT_NULL(node, "AST_ParentNode accepts only not nullptr children");
  }
}
//
//  End of: AST_ParentNode::AST_ParentNode
//---------------------------------------------------------------------------



//! Removes children that have been dispatched (leaving nullptr)
//!
void AST_ParentNode::CleanupChildren ()
{
  vector<AST_Node*> cleanedUp;

  for (const auto node : m_undispatchedChildren)
  {
    if (node != nullptr)
    {
      cleanedUp.push_back(node);
    }
  }

  std::swap(cleanedUp, m_undispatchedChildren);
}
//
//  End of: AST_ParentNode::CleanupChildren
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_ParentNode.cpp
//===========================================================================
