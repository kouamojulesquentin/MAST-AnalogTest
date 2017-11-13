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

#include "AST_SimpleNode.hpp"
#include <experimental/string_view>
#include <string>

namespace Parsers
{
//! Represents a parameter reference
//!
class AST_ParameterRef final : public AST_SimpleNode
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
    : AST_SimpleNode (Kind::Parameter_ref)
    , m_name         (std::move(name))
  {
  }

  // ---------------- Private Fields
  //
  private:
  const std::string m_name; //!< Refered parameter name
};
//
//  End of AST_ParameterRef class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_PARAMETERREF_H__9FB827F9_9F9_4611_D983_8ED086F9B995__INCLUDED_
//===========================================================================
// End of AST_ParameterRef.hpp
//===========================================================================
