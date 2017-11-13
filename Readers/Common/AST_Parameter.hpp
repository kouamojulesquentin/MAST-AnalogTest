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
#include <vector>

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

  //! Parameter name
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of parameter
  //!
  std::string AsText() const override;

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Parameter)(Parsers::Kind&, std::string&&, std::string&&);
  MAKE_UNIQUE_AS_FRIEND(AST_Parameter)(Parsers::Kind&, std::string&&, std::vector<AST_SimpleNode*>&&);

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

  //! Initializes AST_Parameter with a number
  //!
  //! @param kind               Either Parameter or LocalParameter
  //! @param name               Parameter name
  //! @param stringsOrRefsValue Strings and/or parameter ref that define parameter value
  //!                           Should be AST_String and AST_ParameterRef
  //!
  AST_Parameter(Kind kind, std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue)
    : AST_SimpleNode (kind)
    , m_name         (std::move(name))
    , m_stringsValue (std::move(stringsOrRefsValue))
  {
  }

  // ---------------- Private Fields
  //
  private:
  const std::string            m_name;         //!< Parameter name
  const std::string            m_numbersValue; //!< Parameter value when defined as numbers
  std::vector<AST_SimpleNode*> m_stringsValue; //!< Parameter value when defined as strings and/or parameter reference
};
//
//  End of AST_Parameter class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_PARAMETER_H__6601FC1C_1BB0_4FE1_3DA7_E138C1507BC2__INCLUDED_
//===========================================================================
// End of AST_Parameter.hpp
//===========================================================================
