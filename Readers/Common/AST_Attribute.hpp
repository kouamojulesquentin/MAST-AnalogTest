//===========================================================================
//                           AST_Attribute.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Attribute.hpp
//!
//! Declares AST_Attribute class
//!
//===========================================================================

#ifndef AST_ATTRIBUTE_H__5F066B0A_8C66_4E96_D1BE_D6C341DAE98__INCLUDED_
  #define AST_ATTRIBUTE_H__5F066B0A_8C66_4E96_D1BE_D6C341DAE98__INCLUDED_

#include "AST_Node.hpp"
#include <experimental/string_view>
#include <string>

namespace Parsers
{
//! Represents an attribute
//!
class AST_Attribute final : public AST_Node
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Attribute() = default;
  AST_Attribute()  = delete;

  //! Visited part of the Visitor pattern
  //!
//+  void Accept (AST_Visitor& visitor) override;

  //! Attribute name
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of attribute
  //!
  std::string AsText() const;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                               // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Attribute)(string&&); // AST currently uses make_unit<T>() to create nodes

  AST_Attribute(std::string&& name)
    : AST_Node (Kind::Attribute)
    , m_name   (std::move(name))
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
//  End of AST_Attribute class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers





#endif  // not defined AST_ATTRIBUTE_H__5F066B0A_8C66_4E96_D1BE_D6C341DAE98__INCLUDED_
//===========================================================================
// End of AST_Attribute.hpp
//===========================================================================
