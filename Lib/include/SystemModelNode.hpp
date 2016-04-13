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
#include <string>
#include <functional>

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
  SystemModelNode()  = delete;
  SystemModelNode(std::string name)
    : m_identifier (GetNextIdentifier())
    , m_name       (name)
  {}


  void AppendSibling (SystemModelNode* pSibling); //!< Appends a new sibling node

  SystemModelNode* GetNextSibling() const { return m_pNextSibling; }; //!< Returns first sibling or nullptr

  using ConditionFunctor = std::function<bool(SystemModelNode*)>;   //!< Defines pre- and post- condition functors

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:
  static uint32_t GetNextIdentifier();

  // ---------------- Private  Fields
  //
  private:
  uint32_t         m_identifier   = 0;
  std::string      m_name;                   //!< Node readable name
  SystemModelNode* m_pNextSibling = nullptr; //!< Points to next node at same level (forming a singly linked list)
  bool             m_pending      = false;   //!< True when at least one node in the hierarchy is pending
  bool             m_hasCondition = false;   //!< True when the node has some condition to check (either pre- or post-)
};
//
//  End of SystemModelNode class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELNODE_H__1BEF31DB_FB8A_405C_78B9_75CDBD328A52__INCLUDED_

//===========================================================================
// End of SystemModelNode.hpp
//===========================================================================



















