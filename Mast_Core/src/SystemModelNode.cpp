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
#include "Utility.hpp"

using std::string;
using std::experimental::string_view;
using namespace mast;

SystemModelNode::NodeIdentifier SystemModelNode::sm_nextIdentifier = 0;

//! Sets unique identifier and name (using default one if unspecified)
//!
SystemModelNode::SystemModelNode (string_view name)
  : m_identifier (NextIdentifier())
  , m_name       (MakeNodeName(name))
{
}
//
//  End of: SystemModelNode::SystemModelNode
//---------------------------------------------------------------------------


//! Appends a new sibling node at end of sibling list
//!
//! @param pSibling Sibling to append
//!
void SystemModelNode::AppendSibling (std::shared_ptr<SystemModelNode> pSibling)
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


//! Splices a new sibling node just after this node (before next sibling)
//!
//! @param pSibling Sibling to insert
//!
void SystemModelNode::SpliceSibling (std::shared_ptr<SystemModelNode> pSibling)
{
  CHECK_PARAMETER_NOT_NULL(pSibling, "Cannot splice nullptr node");

  if (m_pNextSibling)
  {
    CHECK_VALUE_NULL(pSibling->m_pNextSibling, "SpliceSibling only splices a single node");

    pSibling->m_pNextSibling = m_pNextSibling;
    m_pNextSibling = pSibling;
  }
  else
  {
    m_pNextSibling = pSibling;
  }
}
//
//  End of: SystemModelNode::SpliceSibling
//---------------------------------------------------------------------------


//! Provides unique node identifier
//!
//! @note This method IS NOT THREAD SAFE
//!
//! @return New node identifier
SystemModelNode::NodeIdentifier SystemModelNode::NextIdentifier ()
{
  return sm_nextIdentifier++; // Root node must have identifier 0
}
//
//  End of: SystemModelNode::NextIdentifier
//---------------------------------------------------------------------------


//! Makes a valid node name
//!
//! @note Returns name if it is valid, a default one when not valid
string SystemModelNode::MakeNodeName (string_view name)
{
  // ---------------- Make sure there is at least a default name
  //
  if (name.empty())
  {
    return string(mast::DEFAULT_NODE_NAME);
  }

  return string(name);
}
//
//  End of: SystemModelNode::MakeNodeName
//---------------------------------------------------------------------------




//===========================================================================
// End of SystemModelNode.cpp
//===========================================================================
