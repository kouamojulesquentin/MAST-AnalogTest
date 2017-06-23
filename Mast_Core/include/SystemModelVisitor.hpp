//===========================================================================
//                           SystemModelVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelVisitor.hpp
//!
//! Declares interface SystemModelVisitor
//===========================================================================

#ifndef SYSTEMMODELVISITOR_H__D20D9DFE_5339_4A93_CAAB_34F1335416B2__INCLUDED_
  #define SYSTEMMODELVISITOR_H__D20D9DFE_5339_4A93_CAAB_34F1335416B2__INCLUDED_

#include "Mast_Core_export.hpp"

namespace mast
{
class SystemModelNode;
class ParentNode;
class AccessInterface;
class Chain;
class Linker;
class Register;
class AccessInterfaceTranslator;

//! System model visitors interface
//!
//! @note In fact, this is an abstract base class as it provides 3 helpers to ease visits of parents children
//!
class MAST_CORE_EXPORT SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual void VisitAccessInterface (AccessInterface& accessInterface) = 0;
  virtual void VisitChain           (Chain&           chain)           = 0;
  virtual void VisitLinker          (Linker&          linker)          = 0;
  virtual void VisitRegister        (Register&        reg)             = 0;

  //! Helper to visit only active children of a Linker node
  //!
  virtual void VisitActiveRegisters (const Linker& linker);

  //! Helper to visit only pending children
  //!
  virtual void VisitPendingChildren (const ParentNode& parentNode);

  //! Helper to visit all children of a parent node
  //!
  virtual void VisitChildren (const ParentNode& parentNode);

  // ---------------- Protected Methods
  //
  protected:
  virtual ~SystemModelVisitor() = default;
  SystemModelVisitor() = default;
};
//
//  End of SystemModelVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SYSTEMMODELVISITOR_H__D20D9DFE_5339_4A93_CAAB_34F1335416B2__INCLUDED_

//===========================================================================
// End of SystemModelVisitor.hpp
//===========================================================================
