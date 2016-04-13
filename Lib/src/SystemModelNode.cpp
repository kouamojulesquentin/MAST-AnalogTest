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


using mast::SystemModelNode;


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
uint32_t SystemModelNode::GetNextIdentifier ()
{
  static uint32_t nextIdentifier = 0;
  return ++nextIdentifier;
}
//
//  End of: SystemModelNode::GetNextIdentifier
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelNode.cpp
//===========================================================================
