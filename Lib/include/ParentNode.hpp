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
class DLL_EXPORT ParentNode : public SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:

  //! Appends a new child node
  //!
  void AppendChild (std::shared_ptr<SystemModelNode> pChild);

  uint32_t DirectChildrenCount() const;  //!< Returns current number of direct children

  std::shared_ptr<SystemModelNode> FirstChild()    const { return m_pFirstChild;            } //!< Returns first child or nullptr
  std::shared_ptr<ParentNode>      ChildAppender() const { return m_pOptionalChildAppender; }

  virtual std::shared_ptr<SystemModelNode> DisconnectDerivation(uint32_t derivationId); //!< Disconnects a derivation from the parent
  virtual std::shared_ptr<SystemModelNode> DisconnectAllChildren();                     //!< Disconnects all direct children

  void  SetChildAppender (std::shared_ptr<ParentNode> childAppender) { m_pOptionalChildAppender = childAppender; }

  // ---------------- Protected Methods
  //
  protected:
  virtual ~ParentNode() = default;
  ParentNode() = delete;
  ParentNode(std::experimental::string_view name) : SystemModelNode(name) {}


  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  std::shared_ptr<SystemModelNode> m_pFirstChild;
  std::shared_ptr<ParentNode>      m_pOptionalChildAppender;  //!< To be used when "logical child" does not include "first child"
};
//
//  End of ParentNode class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PARENTNODE_H__EEDC0EF6_BF2B_4D20_6BBA_85F1386D5B97__INCLUDED_

//===========================================================================
// End of ParentNode.hpp
//===========================================================================
