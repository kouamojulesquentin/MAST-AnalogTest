//===========================================================================
//                           AST_IntegerExprRef.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerExprRef.hpp
//!
//! Declares AST_IntegerExprRef class
//!
//===========================================================================

#ifndef AST_INTEGEREXPRREF_H__F3AF6DB9_73C8_4E8D_3082_B11F2A09B37C__INCLUDED_
  #define AST_INTEGEREXPRREF_H__F3AF6DB9_73C8_4E8D_3082_B11F2A09B37C__INCLUDED_

#include "AST_IntegerExpr.hpp"

namespace Parsers
{
class AST_Builder;
class AST_ParameterRef;
class AST_ConcatNumber;

//! Represents either an unary integer expression or a concatenated number through a parameter reference.
//!
//! @note It derives from AST_IntegerExpr in order to be easily used in integer expressions but can instead be a concatenated number
//!       or even simply a "based" number (when the concatenated number has only one element).
//!       So, it is more a number reference, but by design a number can be the result of integer expression but not a part of it.
//!       This is because a "based" number is a naturally a number but cannot take part of integer expression.
//!
class AST_IntegerExprRef final : public AST_IntegerExpr
{
  // ---------------- Public Methods
  //
  public:
  ~AST_IntegerExprRef() = default;
  AST_IntegerExprRef()  = delete;

  std::string AsText()   const override; //!< Build text representation of integer expression
  uint32_t    Evaluate() const override; //!< Evaluates expression

  //! Returns true when it is still a parameter reference to integer expression
  //!
  bool HasParameterRef () const override;

  //! Returns uniquified clone
  //!
  AST_IntegerExprRef* UniquifiedClone(AST_Builder& astBuilder) const override;

  //! Replaces parameter reference by its actual integer expression
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters) override;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                       // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_IntegerExprRef)(Parsers::AST_ParameterRef*&); // AST currently uses make_unit<T>() to create nodes

  //! Initializes with parameter reference
  //!
  explicit AST_IntegerExprRef(AST_ParameterRef* parameterRef);

  // ---------------- Private Fields
  //
  private:
  AST_ParameterRef* m_parameterRef = nullptr; //!< Parameter reference until it is resolved
  AST_IntegerExpr*  m_integerExpr  = nullptr; //!< Represented integer expression when the Parameter Reference is resolved
  AST_ConcatNumber* m_concatNumber = nullptr; //!< Represented concatenated number when the Parameter Reference is resolved (and not an integer expression)
};
//
//  End of AST_IntegerExprRef class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_INTEGEREXPRREF_H__F3AF6DB9_73C8_4E8D_3082_B11F2A09B37C__INCLUDED_
//===========================================================================
// End of AST_IntegerExprRef.hpp
//===========================================================================
