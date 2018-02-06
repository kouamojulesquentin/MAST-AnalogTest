//===========================================================================
//                           AST_Alias.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Alias.hpp
//!
//! Declares AST_Alias class
//!
//===========================================================================

#ifndef AST_ALIAS_H__F87C2218_282B_4BF6_9189_E2055A796EC4__INCLUDED_
  #define AST_ALIAS_H__F87C2218_282B_4BF6_9189_E2055A796EC4__INCLUDED_

#include "AST_ParentNode.hpp"

#include <string>
#include <vector>

namespace Parsers
{
class AST_Attribute;
class AST_ConcatNumber;
class AST_EnumRef;
class AST_Signal;
class AST_VectorIdentifier;

//! Represents an alias statement
//!
//! @note RefEnum statement is not yet supported
//!
class AST_Alias final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Alias() = default;
  AST_Alias()  = delete;

  //! Returns alias identifier as text
  //!
  std::string Name() const override;

  //! Returns alias identifier (name and optional bits range)
  //!
  AST_VectorIdentifier* Identifier() const { return m_identifier; }

  //! Returns alias attributes
  //!
  const std::vector<AST_Attribute*>& Attributes() const { return m_attributes; }

  //! Returns aliased signals
  //!
  const std::vector<AST_Signal*>& Signals() const { return m_signals; }

  //! Returns optional Enum reference
  //!
  //! An AST_EnumRef when alias is associated with an enum nullptr otherwise
  AST_EnumRef* EnumRef() const { return m_enumRef; }

  //! Returns optional iApplyEndState
  //!
  //! An AST_ConcatNumber when alias is associated with an iApplyEndState value
  AST_ConcatNumber* ApplyEndState() const { return m_iApplyEndState; }

  //! Returns whether aliased signasl must be updated together (during the same iApply cycle)
  //!
  bool AccessTogether() const { return m_accessTogether; }

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor);

  //! Returns true when there is a parameter reference in part of alias
  //!
  bool HasParameterRef () const;

  //! Replaces parameter references by its actual value
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters);

  //! Returns uniquified clone
  //!
  AST_Alias* UniquifiedClone(AST_Builder& astBuilder) const;

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Alias)(Parsers::AST_VectorIdentifier*&,
                                   std::vector<Parsers::AST_Signal*>&&,
                                   std::vector<Parsers::AST_Node*>&&);

  //! Initializes AST_Alias
  //!
  AST_Alias(AST_VectorIdentifier*      aliasIdentifier,
            std::vector<AST_Signal*>&& signals,
            std::vector<AST_Node*>&&   optionalChildren
            );


  //! Dispatches optional parts to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Methods
  //
  std::string FoundAliasMessage () const;

  // ---------------- Private Fields
  //
  private:
  AST_VectorIdentifier*       m_identifier     = nullptr; //!< Alias identifier (scalar or vector)
  std::vector<AST_Signal*>    m_signals;                  //!< Aliased signals (may encompass bits of different registers)
  std::vector<AST_Attribute*> m_attributes;               //!< Alias attributes
  AST_EnumRef*                m_enumRef        = nullptr; //!< Enum associated with that aliased signals
  AST_ConcatNumber*           m_iApplyEndState = nullptr; //!< Used to automatically apply an "end state" after each iWrite to the aliased registers
  bool                        m_accessTogether = false;   //!< True when aliases must be updated together (they shall be accessible in a single scan chain operation)
};
//
//  End of AST_Alias class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_ALIAS_H__F87C2218_282B_4BF6_9189_E2055A796EC4__INCLUDED_
//===========================================================================
// End of AST_Alias.hpp
//===========================================================================
