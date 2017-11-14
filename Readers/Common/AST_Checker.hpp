//===========================================================================
//                           AST_Checker.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Checker.hpp
//!
//! Declares AST_Checker class
//!
//===========================================================================

#ifndef AST_CHECKER_H__B4636CB0_96B6_48EC_EAA4_418DCE61C33A__INCLUDED_
  #define AST_CHECKER_H__B4636CB0_96B6_48EC_EAA4_418DCE61C33A__INCLUDED_

#include "AST_Visitor.hpp"
#include "Checker.hpp"
#include "CheckResult.hpp"


namespace Parsers
{
//! Checks parsers AST
//!
//! @note It is currently a sanity check and ICL based semantic check
//!
class AST_Checker final : public mast::Checker, public AST_Visitor
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Checker();
  AST_Checker() = delete;
  AST_Checker(AST_Network* network);

  virtual void Visit_AccessLink    (AST_AccessLink*    instance)      override;
  virtual void Visit_Instance      (AST_Instance*      instance)      override;
  virtual void Visit_Network       (AST_Network*       network)       override;
  virtual void Visit_Module        (AST_Module*        module)        override;
  virtual void Visit_Port          (AST_Port*          port)          override;
  virtual void Visit_ScanInterface (AST_ScanInterface* scanInterface) override;
  virtual void Visit_ScanMux       (AST_ScanMux*       scanMux)       override;
  virtual void Visit_ScanRegister  (AST_ScanRegister*  scanRegister)  override;
  virtual void Visit_Source        (AST_Source*        source)        override;

  //! Checks SystemModel consistency
  //!
  //! @see CheckIdentifiers and CheckTree
  //!
  static mast::CheckResult Check(AST_Network* network) { return AST_Checker(network).Check(); };

  //! Checks basic AST consistency
  //!
  virtual mast::CheckResult Check() override;

  // ---------------- Private Methods
  //
  private:

  // ---------------- Private Fields
  //
  private:
  AST_Network* m_network = nullptr; //!< Test network AST being checked
};
//
//  End of AST_Checker class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_CHECKER_H__B4636CB0_96B6_48EC_EAA4_418DCE61C33A__INCLUDED_
//===========================================================================
// End of AST_Checker.hpp
//===========================================================================
