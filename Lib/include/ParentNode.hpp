//===========================================================================
//                           ParentNode.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ParentNode.hpp
//!
//! Declares class ParentNode
//!
//===========================================================================

#ifndef PARENTNODE_H__EEDC0EF6_BF2B_4D20_6BBA_85F1386D5B97__INCLUDED_
  #define PARENTNODE_H__EEDC0EF6_BF2B_4D20_6BBA_85F1386D5B97__INCLUDED_

#include "SystemModelNode.hpp"

namespace mast
{
//! Abstract class to represent nodes that can have children
//!
class ParentNode : public SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:
  //! Appends a new child node
  //! @note It forms a singly linked list of children
  void AppendChild (SystemModelNode* pChild)
  {
    if (m_pFirstChild)
    {
      m_pFirstChild->AppendSibling(pChild);
    }
    else
    {
      m_pFirstChild = pChild;
    }
  }

  SystemModelNode* GetFirstChild()   const { return m_pFirstChild; } //!< Returns first child or nullptr


  // ---------------- Protected Methods
  //
  protected:
  virtual ~ParentNode() = default;
  ParentNode()  = default;

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  SystemModelNode* m_pFirstChild = nullptr;
};
//
//  End of ParentNode class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PARENTNODE_H__EEDC0EF6_BF2B_4D20_6BBA_85F1386D5B97__INCLUDED_

//===========================================================================
// End of ParentNode.hpp
//===========================================================================
