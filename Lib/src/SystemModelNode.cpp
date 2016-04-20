//===========================================================================
//                           SystemModelNode.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelNode.cpp
//!
//! Implements class SystemModelNode
//!
//===========================================================================


#include "SystemModelNode.hpp"

using namespace mast;

SystemModelNode::NodeIdentifier SystemModelNode::sm_nextIdentifier = 0;

//! Sets unique identifier and name (using default one if unspecified)
//!
SystemModelNode::SystemModelNode (std::string name)
  : m_identifier (GetNextIdentifier())
  , m_name       (std::move(name))
{
  // ---------------- Make sure there is at least a default name
  //
  if (m_name.empty())
  {
    m_name = std::string(mast::DEFAULT_NODE_NAME);
  }
}
//
//  End of: SystemModelNode::SystemModelNode
//---------------------------------------------------------------------------
//




//! Appends a new sibling node
//!
void SystemModelNode::AppendSibling (SystemModelNode* pSibling)
{
  if (m_pNextSibling)
  {
    m_pNextSibling->AppendSibling(pSibling);
  }
  else
  {
    m_pNextSibling = pSibling;
  }
}
//
//  End of: SystemModelNode::AppendSibling
//---------------------------------------------------------------------------


//! Provides unique node identifier
//!
//! @note This method IS NOT THREAD SAFE
//!
//! @return New node identifier
SystemModelNode::NodeIdentifier SystemModelNode::GetNextIdentifier ()
{
  return sm_nextIdentifier++; // Root node must have identifier 0
}
//
//  End of: SystemModelNode::GetNextIdentifier
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelNode.cpp
//===========================================================================
