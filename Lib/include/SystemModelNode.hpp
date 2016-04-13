//===========================================================================
//                           SystemModelNode.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelNode.hpp
//!
//! Declares class SystemModelNode
//!
//===========================================================================

#ifndef SYSTEMMODELNODE_H__1BEF31DB_FB8A_405C_78B9_75CDBD328A52__INCLUDED_
  #define SYSTEMMODELNODE_H__1BEF31DB_FB8A_405C_78B9_75CDBD328A52__INCLUDED_

#include <cstdint>

namespace mast
{
//! Common features of a system model node
//!
class SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SystemModelNode() = default;
  SystemModelNode()  = default;


  void AppendSibling (SystemModelNode* pSibling); //!< Appends a new sibling node

  SystemModelNode* GetNextSibling() const { return m_pNextSibling; }; //!< Returns first sibling or nullptr


  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  uint32_t         m_identifier   = 0;
  SystemModelNode* m_pNextSibling = nullptr; //!< Points to next node at same level (forming a singly linked list)
};
//
//  End of SystemModelNode class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELNODE_H__1BEF31DB_FB8A_405C_78B9_75CDBD328A52__INCLUDED_

//===========================================================================
// End of SystemModelNode.hpp
//===========================================================================



















