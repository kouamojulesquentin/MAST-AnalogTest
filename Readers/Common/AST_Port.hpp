//===========================================================================
//                           AST_Port.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Port.hpp
//!
//! Declares AST_Port class
//!
//===========================================================================

#ifndef AST_PORT_H__A8276CED_1E93_4374_A79F_9DA3231CAAEF__INCLUDED_
  #define AST_PORT_H__A8276CED_1E93_4374_A79F_9DA3231CAAEF__INCLUDED_

#include "AST_ParentNode.hpp"
#include "AST_VectorIdentifier.hpp"

#include <memory>
#include <vector>
#include <string>
#include <experimental/string_view>

namespace Parsers
{
class AST_Attribute;

//! Represents a parsed test network, instiable, module
//!
class AST_Port final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Port() = default;
  AST_Port()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Returns port name
  //!
  std::string Name() const override { return m_identifier->AsText(); }

  //! Returns port attribute
  //!
  const std::vector<AST_Attribute*>& Attributes() const { return m_attributes; }

  // ---------------- Private Methods
  //
  private:

  friend class AST;                                              // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Port)(Parsers::Kind& kind,
                                  Parsers::AST_VectorIdentifier*&);
  MAKE_UNIQUE_AS_FRIEND(AST_Port)(Parsers::Kind& kind,
                                  Parsers::AST_VectorIdentifier*&,
                                  std::vector<AST_Node*>&&);

  AST_Port(Kind kind, AST_VectorIdentifier* identifier)
    : AST_ParentNode (kind)
    , m_identifier   (identifier)
  {
  }

  AST_Port(Kind kind, AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children)
    : AST_ParentNode (kind, std::move(children))
    , m_identifier   (identifier)
  {
    DispatchChildren();
  }

  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  const AST_VectorIdentifier* m_identifier = nullptr; //!< Port identifier
  std::vector<AST_Attribute*> m_attributes;           //!< Port attributes
};
//
//  End of AST_Port class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_PORT_H__A8276CED_1E93_4374_A79F_9DA3231CAAEF__INCLUDED_
//===========================================================================
// End of AST_Port.hpp
//===========================================================================
