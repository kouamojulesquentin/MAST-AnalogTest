//===========================================================================
//                           AST_Parameter.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Parameter.hpp
//!
//! Declares AST_Parameter class
//!
//===========================================================================

#ifndef AST_PARAMETER_H__6601FC1C_1BB0_4FE1_3DA7_E138C1507BC2__INCLUDED_
  #define AST_PARAMETER_H__6601FC1C_1BB0_4FE1_3DA7_E138C1507BC2__INCLUDED_

#include "AST_SimpleNode.hpp"
#include <experimental/string_view>
#include <string>

namespace Parsers
{
//! Represents a parameter (for generic modules) or local parameter (for convenience)
//!
class AST_Parameter final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Parameter() = default;
  AST_Parameter()  = delete;

  //! Visited part of the Visitor pattern
  //!
//+  void Accept (AST_Visitor& visitor) override;

  //! Attribute name
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of parameter
  //!
  std::string AsText() const override;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                                   // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Parameter)(Parsers::Kind&, std::string&&, std::string&&); // AST currently uses make_unit<T>() to create nodes

  //! Initializes AST_Parameter with a number
  //!
  //! @param kind         Either Parameter or LocalParameter
  //! @param name         Parameter name
  //! @param numbersValue Numbers that define parameter value
  //!
  AST_Parameter(Kind kind, std::string&& name, std::string&& numbersValue)
    : AST_SimpleNode (kind)
    , m_name         (std::move(name))
    , m_numbersValue (std::move(numbersValue))
  {
  }

  // ---------------- Private Fields
  //
  private:
  const std::string        m_name;                   //!< Attribute name
  const std::string        m_numbersValue;           //!< Attribute value when defined as numbers
//+  AST_StringConcatenation* m_stringsValue = nullptr; //!< Attribute value when defined as strings
};
//
//  End of AST_Parameter class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_PARAMETER_H__6601FC1C_1BB0_4FE1_3DA7_E138C1507BC2__INCLUDED_
//===========================================================================
// End of AST_Parameter.hpp
//===========================================================================
