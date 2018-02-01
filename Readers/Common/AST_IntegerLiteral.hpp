//===========================================================================
//                           AST_IntegerLiteral.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_IntegerLiteral.hpp
//!
//! Declares AST_IntegerLiteral class
//!
//===========================================================================

#ifndef AST_INTEGERLITERAL_H__5A2D2041_37A5_441F_7494_239A50188624__INCLUDED_
  #define AST_INTEGERLITERAL_H__5A2D2041_37A5_441F_7494_239A50188624__INCLUDED_

#include "AST_IntegerExpr.hpp"

namespace Parsers
{
//! Represents an unary integer expression
//!
class AST_IntegerLiteral final : public AST_IntegerExpr
{
  // ---------------- Public Methods
  //
  public:
  ~AST_IntegerLiteral() = default;
  AST_IntegerLiteral()  = delete;

  std::string AsText()   const override;                    //!< Build text representation of integer expression
  uint32_t    Evaluate() const override { return m_value; } //!< Evaluates expression

  //! Returns uniquified clone
  //!
  AST_IntegerLiteral* UniquifiedClone(AST_Builder& astBuilder) const override;

  //! No parameter to resolve for literals
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters) override {  };

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                           // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_IntegerLiteral)(std::string&&);   // AST currently uses make_unit<T>() to create nodes
  MAKE_UNIQUE_AS_FRIEND(AST_IntegerLiteral)(uint32_t& value); // AST currently uses make_unit<T>() to create nodes

  //! Initializes a literal expression text
  //!
  explicit AST_IntegerLiteral(std::string&& integerText);

  //! Initializes a literal expression text
  //!
  explicit AST_IntegerLiteral(uint32_t value)
    : AST_IntegerExpr (Kind::IntegerLiteral)
    , m_value         (value)
  {
  }

  // ---------------- Private Fields
  //
  private:
  uint32_t m_value = 0; //!< Literal value
};
//
//  End of AST_IntegerLiteral class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_INTEGERLITERAL_H__5A2D2041_37A5_441F_7494_239A50188624__INCLUDED_
//===========================================================================
// End of AST_IntegerLiteral.hpp
//===========================================================================
