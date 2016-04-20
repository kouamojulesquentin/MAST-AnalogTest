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

namespace mast
{
class AccessInterface;
class Chain;
class Linker;
class Register;
class Tap;

//! System model visitors interface
//!
class SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual void VisitAccessInterface (AccessInterface& accessInterface) = 0;
  virtual void VisitChain           (Chain&           chain)           = 0;
  virtual void VisitLinker          (Linker&          linker)          = 0;
  virtual void VisitRegister        (Register&        reg)             = 0;
  virtual void VisitTap             (Tap&             tap)             = 0;

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
