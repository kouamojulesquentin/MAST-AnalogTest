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
#include <memory>

namespace mast
{
//! Abstract class to represent nodes that can have children
//!
class DLL_EXPORT ParentNode : public SystemModelNode, public std::enable_shared_from_this<ParentNode>
{
  // ---------------- Public  Methods
  //
  public:

  void AppendChild    (std::shared_ptr<SystemModelNode> node);       //!< Appends a new child node
  bool HasDirectChild (std::shared_ptr<SystemModelNode> node) const; //!< Returns true if node is a direct child

  uint32_t DirectChildrenCount() const;  //!< Returns current number of direct children

  bool                             IgnoreForNodePath()    const { return m_ignoreForNodePath;      } //!< When true the node name is ignored when search a node by its path
  std::shared_ptr<SystemModelNode> FirstChild()           const { return m_pFirstChild;            } //!< Returns first child or nullptr
  std::shared_ptr<ParentNode>      ChildAppender()        const { return m_pOptionalChildAppender; }
  std::shared_ptr<ParentNode>      DeepestChildAppender() const;                                     //!< Returns ParentNode that will effectively append childs in place of this ParentNode

  void  IgnoreForNodePath (bool ignoreForNodePath) { m_ignoreForNodePath = ignoreForNodePath; } //!< Set whether the node must be ignored when search a node by its path

  std::shared_ptr<ParentNode>      FindParentOfNode(std::shared_ptr<SystemModelNode> child); //!< Searches down the hierarchy, the parent of specified node

  std::shared_ptr<SystemModelNode> FindNode (std::experimental::string_view path);  //!< Searches node matching specified path


  virtual void DisconnectChild(std::shared_ptr<SystemModelNode> child); //!< Disconnects specified child from its parent

  virtual std::shared_ptr<SystemModelNode> DisconnectDerivation(uint32_t derivationId); //!< Disconnects a derivation from the parent
  virtual std::shared_ptr<SystemModelNode> DisconnectAllChildren();                     //!< Disconnects all direct children

  void  SetChildAppender (std::shared_ptr<ParentNode> childAppender) { m_pOptionalChildAppender = childAppender; }


  // ---------------- Protected Methods
  //
  protected:
  virtual ~ParentNode() = default;
  ParentNode() = delete;
  ParentNode(std::experimental::string_view name) : SystemModelNode(name) {}

  void DisconnectSibling(std::shared_ptr<SystemModelNode> beforeNode, std::shared_ptr<SystemModelNode> sibling);

  // ---------------- Private  Fields
  //
  private:
  std::shared_ptr<SystemModelNode> m_pFirstChild;
  std::shared_ptr<ParentNode>      m_pOptionalChildAppender;    //!< To be used when "logical child" does not include "first child"
  bool                             m_ignoreForNodePath = false; //!< When true the node name is ignored when search a node by its path
};
//
//  End of ParentNode class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PARENTNODE_H__EEDC0EF6_BF2B_4D20_6BBA_85F1386D5B97__INCLUDED_

//===========================================================================
// End of ParentNode.hpp
//===========================================================================
