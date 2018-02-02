//===========================================================================
//                           AST_ParentNode.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ParentNode.hpp
//!
//! Declares AST_ParentNode class
//!
//===========================================================================


#ifndef AST_PARENTNODE_H__9C01A73E_808_43D6_A6B3_243B7A5440B__INCLUDED_
  #define AST_PARENTNODE_H__9C01A73E_808_43D6_A6B3_243B7A5440B__INCLUDED_

#include "AST_NamedNode.hpp"
#include "AST_PlaceHolder.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_Builder.hpp"

#include <vector>

namespace Parsers
{
class AST_ScalarIdentifier;
class AST_Parameter;

//! Represents a named AST node that can have children
//!
//!
class AST_ParentNode : public AST_NamedNode
{
  // ---------------- Public Methods
  //
  public:
  virtual ~AST_ParentNode() = default;
  AST_ParentNode()          = delete;

  //! Returns handle on not yet "dispatched" children
  //!
  //! @note This is mainly intended to be "processed" by visitors
  //!
  std::vector<AST_Node*>& UndispatchedChildren() { return m_undispatchedChildren; }

  //! Removes children that have been dispatched from undispatched list
  //!
  void CleanupChildren ();

  // ---------------- Protected Methods
  //
  protected:

  //! Initializes with no children
  //!
  //! @note This is for case when children are optional
  //!
  explicit AST_ParentNode(Kind kind)
    : AST_NamedNode(kind)
  {
  }

  //! Initializes with children
  //!
  AST_ParentNode(Kind kind, std::vector<AST_Node*>&& children);

  //! Dispatches children to specific members
  //!
  virtual void DispatchChildren () = 0;

  template<typename T>
  static void AppendChild(AST_Node*& child, T& dest)
  {
    using dest_t = typename T::value_type;

    dest.push_back(static_cast<dest_t>(child));
    child = nullptr;
  };

  template<typename T>
  static void SetChild(AST_Node*& child, T*& dest)
  {
    if (child->IsKind(Parsers::Kind::PlaceHolder))
    {
      auto placeHolder = dynamic_cast<AST_PlaceHolder<T>*>(child);
      CHECK_VALUE_NOT_NULL(placeHolder, "Houps: Attempt to assign a place holder wrapped node to a not compatible target type");

      placeHolder->SetTargetKindToWrappedNode();
      dest = placeHolder->WrappedNode();
    }
    else
    {
      dest  = static_cast<T*>(child);
    }

    child = nullptr;
  };

  //! Searches for a AST_Node of some kind with specified identifier
  //!
  //! @param nodes        Collection of nodes to search into
  //! @param identifier   An identifier for AST_Node to find
  //!
  template<typename T>
  static T* FindNode (const std::vector<T*>& nodes, const AST_Identifier* identifier)
  {
    T* foundNode = nullptr;

    for (auto node : nodes)
    {
      if (node->Identifier()->Name() == identifier->Name())
      {
        foundNode = node;
        break;
      }
    }
    return foundNode;
  }


  //! Searches for a AST_Node of some kind with specified name
  //!
  //! @param nodes  Collection of nodes to search into
  //! @param name   Name of named AST_Node to find
  //!
  template<typename T>
  static T* FindNode (const std::vector<T*>& nodes, std::experimental::string_view name)
  {
    T* foundNode = nullptr;

    for (auto node : nodes)
    {
      if (node->Name() == name)
      {
        foundNode = node;
        break;
      }
    }
    return foundNode;
  }


  //! Uniquifies all collection items
  //!
  template<typename T>
  static void UniquifyItems(T& nodes, AST_Builder& astBuilder)
  {
    for (auto& node: nodes)
    {
      node = node->UniquifiedClone(astBuilder);
    }
  }


  //! Uniquifies collection items that have reference(s) to Parameter(s)
  //!
  template<typename T>
  static void UniquifyItemsWithParameterRef(T& nodes, AST_Builder& astBuilder)
  {
    for (auto& node: nodes)
    {
      if (node->HasParameterRef())
      {
        node = node->UniquifiedClone(astBuilder);
      }
    }
  }

  // ---------------- Protected Fields
  //
  protected:
  std::vector<AST_Node*>  m_undispatchedChildren; //!< Children nodes got from parsing and not yet processed (some may be nullptr)
};
//
//  End of AST_ParentNode class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_PARENTNODE_H__9C01A73E_808_43D6_A6B3_243B7A5440B__INCLUDED_

//===========================================================================
// End of AST_ParentNode.hpp
//===========================================================================
