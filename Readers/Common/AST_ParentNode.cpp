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

using std::vector;

using namespace Parsers;


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
