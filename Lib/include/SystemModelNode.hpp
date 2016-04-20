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
class SystemModelVisitor;

constexpr char DEFAULT_NODE_NAME[] = "unnamed";
constexpr char DEFAULT_MIB_NAME[]  = "MIB";
constexpr char MIB_CTRL_EXT[]      = "_ctrl";
constexpr char MIB_MUX_EXT[]       = "_mux";
constexpr char DEFAULT_SIB_NAME[]  = "SIB";
constexpr char DEFAULT_1500_NAME[] = "1500_wrapper";
constexpr char DEFAULT_TAP_NAME[]  = "1149_1_TAP";

//! Abstract base for common features for system model nodes
//!
class SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:

  using NodeIdentifier   = uint32_t;                              //!< Uniquely identifies a node
  using ConditionFunctor = std::function<bool(SystemModelNode*)>; //!< Defines pre- and post- condition functors

  NodeIdentifier   GetIdentifier()  const { return m_identifier;   } //!< Returns node unique identifier
  SystemModelNode* GetNextSibling() const { return m_pNextSibling; } //!< Returns first sibling or nullptr

  void         AppendSibling (SystemModelNode*    pSibling);      //!< Appends a new sibling node
  virtual void Accept        (SystemModelVisitor& visitor) = 0;   //!< Visited part of the Visitor pattern


  // ---------------- Protected Methods
  //
  protected:
  virtual ~SystemModelNode() = default;
  SystemModelNode()  = delete;
  SystemModelNode(std::string name);

  // ---------------- Private  Methods
  //
  private:

  static NodeIdentifier GetNextIdentifier();

  // ---------------- Private  Fields
  //
  private:
  static NodeIdentifier sm_nextIdentifier;   //!< Identifier for the next node to create

  NodeIdentifier   m_identifier;             //!< Uniquely identifies a node
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



















