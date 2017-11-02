//===========================================================================
//                           AST_ScanRegister.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanRegister.hpp
//!
//! Declares AST_ScanRegister class
//!
//===========================================================================

#ifndef AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_
  #define AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_

#include "AST_ParentNode.hpp"
#include "AST_VectorIdentifier.hpp"

#include <memory>
#include <vector>
#include <string>
#include <experimental/string_view>

namespace Parsers
{
class AST_Value;
class AST_Source;

//! Represents a parsed test network, instiable, module
//!
class AST_ScanRegister final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ScanRegister() = default;
  AST_ScanRegister()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! ScanRegister name
  //!
  std::string Name() const override { return m_identifier->AsText(); }

  std::experimental::string_view RangeLeft()  const;
  std::experimental::string_view RangeRight() const;


  const AST_Source* ScanInSource() const { return m_scanInSource; } //!< Scan register input source
  const AST_Value*  ResetValue()   const { return m_resetValue;   } //!< Scan register reset value


  // ---------------- Private Methods
  //
  private:

  friend class AST;                                                         // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ScanRegister)(Parsers::AST_VectorIdentifier*&,
                                          std::vector<AST_Node*>&&);        // AST currently uses make_unit<T>() to create nodes

  AST_ScanRegister(AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children)
    : AST_ParentNode (Kind::ScanRegister, std::move(children))
    , m_identifier   (identifier)
  {
    DispatchChildren();
  }

  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  const AST_VectorIdentifier* m_identifier   = nullptr; //!< Scan register name
  const AST_Value*            m_resetValue   = nullptr; //!< Scan register reset value expression
  const AST_Source*           m_scanInSource = nullptr; //!< Scan register input source
};
//
//  End of AST_ScanRegister class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers

#endif  // not defined AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_

//===========================================================================
// End of AST_ScanRegister.hpp
//===========================================================================
