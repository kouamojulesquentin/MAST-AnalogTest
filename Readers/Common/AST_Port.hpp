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
class AST_Source;

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

  //! Returns port identifier
  //!
  const AST_VectorIdentifier* Identifier() const { return m_identifier; }

  //! Returns port attribute
  //!
  const std::vector<AST_Attribute*>& Attributes() const { return m_attributes; }

  //! Returns true when port has a defined source
  //!
  bool  HasSource() const { return m_source != nullptr; }

  //! Returns port source (if any)
  //!
  const AST_Source* Source() const { return m_source; }

  //! Returns true when port is sourced by a scalar signal
  //!
  bool  IsSourcedByScalarSignal() const;

  //! Returns port source signal
  //!
  AST_Signal* SourceSignal () const;

  //! Returns true when Parameter is defined using Parameter reference(s)
  //!
  bool HasParameterRef () const override;

  //! Returns uniquified clone
  //!
  AST_Port* UniquifiedClone (AST_Builder& astBuilder) const;

  //! Replaces parameter references with their actual value, then resolve value expressions
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters);

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                              // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Port)(Parsers::Kind&,
                                  Parsers::AST_VectorIdentifier*&);
  MAKE_UNIQUE_AS_FRIEND(AST_Port)(Parsers::Kind&,
                                  Parsers::AST_VectorIdentifier*&,
                                  std::vector<AST_Node*>&&);

  AST_Port(Kind kind, AST_VectorIdentifier* identifier);
  AST_Port(Kind kind, AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children);

  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  AST_VectorIdentifier*       m_identifier = nullptr; //!< Port identifier
  AST_Source*                 m_source     = nullptr; //!< Port Source (for output ports)
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
