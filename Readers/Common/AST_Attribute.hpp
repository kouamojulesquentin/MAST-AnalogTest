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

#include "AST_SimpleNode.hpp"
#include <experimental/string_view>
#include <string>
#include <vector>

namespace Parsers
{
class AST_Parameter;
class AST_Builder;

//! Represents an attribute
//!
class AST_Attribute final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Attribute() = default;
  AST_Attribute()  = delete;

  //! Attribute name
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of attribute
  //!
  std::string AsText() const override;

  //! Text representation of attribute value
  //!
  std::string ValueAsText() const;

  //! Returns true when Attribute is defined using Parameter reference(s)
  //!
  bool HasParameterRef () const override;

  //! Replaces parameter references with their actual value, then resolve value expressions
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters);

  //! Returns uniquified clone of value or string expression
  //!
  AST_Attribute* UniquifiedClone (AST_Builder& astBuilder) const;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                               // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Attribute)(std::string&&);
  MAKE_UNIQUE_AS_FRIEND(AST_Attribute)(std::string&&, std::string&&);
  MAKE_UNIQUE_AS_FRIEND(AST_Attribute)(std::string&&, std::vector<AST_SimpleNode*>&&);

  //! Initializes AST_Attribute
  //!
  //! @param name   Attribute name
  //!
  AST_Attribute(std::string&& name)
    : AST_SimpleNode (Kind::Attribute)
    , m_name         (std::move(name))
  {
  }


  //! Initializes AST_Attribute with a number
  //!
  //! @param name         Attribute name
  //! @param numbersValue Numbers that define attribute value
  //!
  AST_Attribute(std::string&& name, std::string&& numbersValue)
    : AST_SimpleNode (Kind::Attribute)
    , m_name         (std::move(name))
    , m_numbersValue (std::move(numbersValue))
  {
  }
  //! Initializes AST_Attribute with a number
  //!
  //! @param kind               Either Attribute or LocalAttribute
  //! @param name               Attribute name
  //! @param stringsOrRefsValue Strings and/or parameter ref that define attribute value
  //!                           Should be AST_String and AST_AttributeRef
  //!
  AST_Attribute(std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue)
    : AST_SimpleNode (Kind::Attribute)
    , m_name         (std::move(name))
    , m_stringsValue (std::move(stringsOrRefsValue))
  {
  }

  // ---------------- Private Fields
  //
  private:
  const std::string            m_name;         //!< Attribute name
  const std::string            m_numbersValue; //!< Attribute value when defined as numbers
  std::vector<AST_SimpleNode*> m_stringsValue; //!< Attribute value when defined as strings and/or parameter reference
};
//
//  End of AST_Attribute class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers





#endif  // not defined AST_ATTRIBUTE_H__5F066B0A_8C66_4E96_D1BE_D6C341DAE98__INCLUDED_
//===========================================================================
// End of AST_Attribute.hpp
//===========================================================================
