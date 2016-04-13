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



//===========================================================================
// End of SystemModelNode.cpp
//===========================================================================
