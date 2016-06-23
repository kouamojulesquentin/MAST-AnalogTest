//===========================================================================
//                           ParentNode.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ParentNode.cpp
//!
//! Implements class ParentNode
//!
//===========================================================================

#include "ParentNode.hpp"
#include "Utility.hpp"

using std::shared_ptr;
using std::to_string;
using namespace mast;


//! Appends a new child node
//!
//! @param pChild Child to append as the last child
//!
//! @note It forms a singly linked list of children
//!
void ParentNode::AppendChild (std::shared_ptr<SystemModelNode> pChild)
{
  if (m_pOptionalChildAppender)
  {
    m_pOptionalChildAppender->AppendChild(pChild);
  }
  else if (m_pFirstChild)
  {
    m_pFirstChild->AppendSibling(pChild);
  }
  else
  {
    m_pFirstChild = pChild;
  }
}
//
//  End of ParentNode::AppendChild
//---------------------------------------------------------------------------


//! Returns current number of children
//!
//! @note It is intended for test or checking purpose (It is O(N) time complexity)
uint32_t ParentNode::DirectChildrenCount () const
{
  uint32_t count = 0;

  auto child = m_pFirstChild;
  while (child)
  {
    if (++count > 10000)
    {
      THROW_LOGIC_ERROR("There seems a child has been appended twice");
    }

    child = child->NextSibling();
  }
  return count;
}
//
//  End of: ParentNode::DirectChildrenCount
//---------------------------------------------------------------------------


//! Disconnects all direct children
//!
//! @return Formerly first child
//!
shared_ptr<SystemModelNode> ParentNode::DisconnectAllChildren ()
{
  auto disconnectedNode = m_pFirstChild;
  m_pFirstChild = nullptr;
  return disconnectedNode;
}
//
//  End of: ParentNode::DisconnectAllChildren
//---------------------------------------------------------------------------


//! Disconnects a derivation from the parent
//!
//! @param derivationId Identifies the derivation to disconnect
//!
//! @return Disconnected derivation
//!
shared_ptr<SystemModelNode> ParentNode::DisconnectDerivation (uint32_t derivationId)
{
  CHECK_PARAMETER_NOT_ZERO (derivationId,  "Cannot remove derivation '0'");
  CHECK_VALUE_NOT_NULL     (m_pFirstChild, "Cannot disconnect a derivation when there is no child");

  uint32_t currentId       = 1u;
  auto     currentChild    = m_pFirstChild;
  auto     previousSibling = shared_ptr<SystemModelNode>();

  while (currentId != derivationId)
  {
    previousSibling = currentChild;
    currentChild    = currentChild->NextSibling();

    CHECK_VALUE_NOT_NULL (currentChild, "Cannot disconnect derivation "s + to_string(derivationId) +
                                        " when there is/are only "       + to_string(currentId) + " derivation(s)");
    ++currentId;
  }

  // ---------------- Splice previous sibling to next sibling
  //                  Or change first child
  //
  auto nextSibling = currentChild->NextSibling();
  if (previousSibling)
  {
    previousSibling->SetNextSibling(nextSibling);
  }
  else
  {
    m_pFirstChild = nextSibling;
  }

  return currentChild;
}
//
//  End of: ParentNode::DisconnectDerivation
//---------------------------------------------------------------------------



//===========================================================================
// End of ParentNode.cpp
//===========================================================================
