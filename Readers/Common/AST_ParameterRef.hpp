//===========================================================================
//                           AST_ParameterRef.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ParameterRef.hpp
//!
//! Declares AST_ParameterRef class
//!
//===========================================================================

#ifndef AST_PARAMETERREF_H__9FB827F9_9F9_4611_D983_8ED086F9B995__INCLUDED_
  #define AST_PARAMETERREF_H__9FB827F9_9F9_4611_D983_8ED086F9B995__INCLUDED_

#include "AST_IntegerExpr.hpp"
#include <experimental/string_view>
#include <string>

namespace Parsers
{
//! Represents a parameter reference
//!
class AST_ParameterRef final : public AST_IntegerExpr
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ParameterRef() = default;
  AST_ParameterRef()  = delete;

  //! Parameter name
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of parameter
  //!
  std::string AsText() const override { return std::string("$").append(m_name); }

  //! Returns referred to parameter integer expression evaluation
  //!
  uint32_t    Evaluate() const override { CHECK_FAILED("Houps: Parameter Reference must be replaced by Parameter Expression before evaluation"); };

  //! Always returns true as this is a Parameter reference
  //!
  virtual bool HasParameterRef () const { return true; };

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_ParameterRef)(std::string&&);

  //! Initializes AST_ParameterRef with a number
  //!
  //! @param name  Refered parameter name
  //!
  AST_ParameterRef(std::string&& name)
    : AST_IntegerExpr (Kind::Parameter_ref)
    , m_name          (std::move(name))
  {
  }

  // ---------------- Private Fields
  //
  private:
  const std::string m_name; //!< Referred parameter name
};
//
//  End of AST_ParameterRef class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_PARAMETERREF_H__9FB827F9_9F9_4611_D983_8ED086F9B995__INCLUDED_
//===========================================================================
// End of AST_ParameterRef.hpp
//===========================================================================
