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

namespace mast
{
class Register;
}

namespace Parsers
{
class AST_ConcatNumber;
class AST_Source;
class AST_Builder;
class AST_Parameter;

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

  //! ScanRegister identifier
  //!
  const AST_VectorIdentifier* Identifier() const { return m_identifier; }

  //! ScanRegister name
  //!
  std::string Name() const override { return m_identifier->AsText(); }

  //! ScanRegister base name (without indices)
  //!
  const std::string& BaseName() const { return m_identifier->BaseName(); }

  //! Returns scan register left index (when not defined as a scalar identifier)
  //!
  uint32_t RangeLeft()  const;

  //! Returns scan register right index (when not defined as a scalar identifier)
  //!
  uint32_t RangeRight() const;

  //! ScanRegister source base name (without indices)
  //!
  const std::string& SourceBaseName() const;

  size_t BitsCount () const;  //!< Returns ScanRegister bits count


  const AST_Source*       CaptureSource() const { return m_captureSource; } //!< Scan register capture source
  const AST_Source*       ScanInSource()  const { return m_scanInSource;  } //!< Scan register input source
  const AST_ConcatNumber* ResetValue()    const { return m_resetValue;    } //!< Scan register reset value

  std::shared_ptr<mast::Register> AssociatedRegister() { return m_associatedModelRegister; }  //!< Mast SystemModel associated register (when not null)

  //! Sets Mast SystemModel associated register (when creating SystemModel)
  //!
  void  AssociatedRegister (std::shared_ptr<mast::Register> associatedRegister) { m_associatedModelRegister = associatedRegister; };

  //! Tells if ScanRegister is already associated with a SystemModel Register
  //!
  bool HasAssociatedRegister() const { return m_associatedModelRegister ? true : false; }

  //! Returns uniquified clone of value or string expression
  //!
  AST_ScanRegister* UniquifiedClone (AST_Builder& astBuilder) const;

  //! Replaces parameter references with their actual value, then resolve value expressions
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters);

  // ---------------- Private Methods
  //
  private:

  friend class AST;                                                         // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ScanRegister)(Parsers::AST_VectorIdentifier*&,
                                          std::vector<AST_Node*>&&);         // AST currently uses make_unit<T>() to create nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ScanRegister)(const Parsers::AST_ScanRegister&); // For clones

  AST_ScanRegister(AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children)
    : AST_ParentNode (Kind::ScanRegister, std::move(children))
    , m_identifier   (identifier)
  {
    DispatchChildren();
    CleanupChildren();
  }

  AST_ScanRegister(const AST_ScanRegister&) = default;


  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;



  // ---------------- Private Fields
  //
  private:
  AST_VectorIdentifier*           m_identifier       = nullptr; //!< Scan register name
  AST_ConcatNumber*               m_resetValue       = nullptr; //!< Scan register reset value expression
  AST_ConcatNumber*               m_defaultLoadValue = nullptr; //!< Scan register default load value expression
  AST_Source*                     m_captureSource    = nullptr; //!< Scan register capture source
  AST_Source*                     m_scanInSource     = nullptr; //!< Scan register input source
  std::shared_ptr<mast::Register> m_associatedModelRegister;    //!< Associated register once mast model is created (during SystemModel creation)
};
//
//  End of AST_ScanRegister class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers

#endif  // not defined AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_

//===========================================================================
// End of AST_ScanRegister.hpp
//===========================================================================
