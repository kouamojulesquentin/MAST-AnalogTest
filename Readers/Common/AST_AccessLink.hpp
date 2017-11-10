//===========================================================================
//                           AST_AccessLink.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_AccessLink.hpp
//!
//! Declares AST_AccessLink class
//!
//===========================================================================

#ifndef AST_ACCESSLINK_H__75C7D49D_C514_4008_659A_61E0910BA6A6__INCLUDED_
  #define AST_ACCESSLINK_H__75C7D49D_C514_4008_659A_61E0910BA6A6__INCLUDED_

#include "AST_ParentNode.hpp"

#include <string>
#include <vector>
namespace Parsers
{
class AST_ScalarIdentifier;

enum class AccessLinkType
{
  Undefined,
  STD_1149_1_2001, //!< For 1149 JTAG
  STD_1149_1_2013, //!< For 1149 JTAG (2013 revision)
  Generic,         //!< For generic AccessLink
};

//! Represents an AccessLink
//!
class AST_AccessLink final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_AccessLink() = default;
  AST_AccessLink()  = delete;


  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Returns instance name (in enclosing scan interface)
  //!
  std::string Name() const override;

  //! Returns AccessLink identifier
  //!
  const AST_ScalarIdentifier* Identifier() const { return m_identifier; };

  //! Returns generic AccessLink identifier (only valid for generic AccessLink)
  //!
  const AST_ScalarIdentifier* GenericIdentifier() const { return m_genericId; };

  //! Returns AccessLink type
  //!
  AccessLinkType Type() const { return m_type; }

  //! Returns AccessLink type as text representation
  //!
  std::string TypeAsText() const;

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_AccessLink)(const Parsers::AST_ScalarIdentifier*&,
                                        Parsers::AccessLinkType&,
                                        std::vector<Parsers::AST_Node*>&&);
  MAKE_UNIQUE_AS_FRIEND(AST_AccessLink)(const Parsers::AST_ScalarIdentifier*&,
                                        const Parsers::AST_ScalarIdentifier*&);

  //! Initializes AST_AccessLink with children
  //!
  //! @param identifier   Access Link name
  //! @param type         Type of Access Link
  //! @param children     Access Link children nodes
  //!
  AST_AccessLink(const AST_ScalarIdentifier* identifier, AccessLinkType type, std::vector<AST_Node*>&& children)
    : AST_ParentNode (Kind::AccessLink, std::move(children))
    , m_identifier   (identifier)
    , m_type         (type)
  {
    DispatchChildren();
    CleanupChildren();
  }


  //! Initializes AST_AccessLink with children
  //!
  //! @param identifier   Access Link name
  //! @param genericId    Generic access link identifier
  //!
  AST_AccessLink(const AST_ScalarIdentifier* identifier, const AST_ScalarIdentifier* genericId)
    : AST_ParentNode (Kind::AccessLink)
    , m_identifier   (identifier)
    , m_genericId    (genericId)
    , m_type         (AccessLinkType::Generic)
  {
    DispatchChildren();
    CleanupChildren();
  }


  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  const AST_ScalarIdentifier* m_identifier = nullptr;                   //!< Access Link name
  const AST_ScalarIdentifier* m_genericId  = nullptr;                   //!< When type is "Generic", identifies the Access Link
  const AccessLinkType        m_type       = AccessLinkType::Undefined; //!< Specifies type of Access Link
};
//
//  End of AST_AccessLink class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_ACCESSLINK_H__75C7D49D_C514_4008_659A_61E0910BA6A6__INCLUDED_
//===========================================================================
// End of AST_AccessLink.hpp
//===========================================================================
