//===========================================================================
//                           AST_IntegerBinaryExpr.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerBinaryExpr.hpp
//!
//! Declares AST_IntegerBinaryExpr class
//!
//===========================================================================

#ifndef AST_INTEGERBINARYEXPR_H__47548788_7967_487A_F284_2F22B3FBAA5__INCLUDED_
  #define AST_INTEGERBINARYEXPR_H__47548788_7967_487A_F284_2F22B3FBAA5__INCLUDED_

#include "AST_IntegerExpr.hpp"

namespace Parsers
{
//! Represents a binary integer expression
//!
class AST_IntegerBinaryExpr final : public AST_IntegerExpr
{
  // ---------------- Public Methods
  //
  public:
  ~AST_IntegerBinaryExpr() = default;
  AST_IntegerBinaryExpr()  = delete;


  std::string AsText()   const override; //!< Build text representation of integer expression
  uint32_t    Evaluate() const override; //!< Evaluates expression

  //! Returns true when at least one of operands is defined using Parameter reference(s)
  //!
  bool HasParameterRef () const override;

  //! Returns uniquified clone
  //!
  AST_IntegerBinaryExpr* UniquifiedClone(AST_Builder& astBuilder) const override;

  //! Replaces parameter references by its actual number
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters) override;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                             // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_IntegerBinaryExpr)(Parsers::Kind& kind, AST_IntegerExpr*&, AST_IntegerExpr*&); // AST currently uses make_unit<T>() to create nodes

  //! Initializes a binary expression with kind of expression and operands
  //!
  AST_IntegerBinaryExpr(Kind kind, AST_IntegerExpr* leftOperand, AST_IntegerExpr* rightOperand);

  // ---------------- Private Fields
  //
  private:
  AST_IntegerExpr* m_left  = nullptr; // Left side of the binary expression
  AST_IntegerExpr* m_right = nullptr; // Right side of the binary expression
};
//
//  End of AST_IntegerBinaryExpr class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_INTEGERBINARYEXPR_H__47548788_7967_487A_F284_2F22B3FBAA5__INCLUDED_

//===========================================================================
// End of AST_IntegerBinaryExpr.hpp
//===========================================================================
