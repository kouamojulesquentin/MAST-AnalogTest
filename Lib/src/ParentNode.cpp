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

using namespace mast;



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



//===========================================================================
// End of ParentNode.cpp
//===========================================================================
