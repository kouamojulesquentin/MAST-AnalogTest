//===========================================================================
//                           SystemModelReseter.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelReseter.hpp
//!
//! Declares SystemModelReseter class
//!
//===========================================================================


#ifndef SYSTEMMODELRESETER_H__29F98A19_457C_4C39_688E_5D1C187D8728__INCLUDED_
  #define SYSTEMMODELRESETER_H__29F98A19_457C_4C39_688E_5D1C187D8728__INCLUDED_

#include "SystemModelVisitor.hpp"
#include <cstdint>

namespace mast
{
//! Resets all SystemModelNodes and issue a Reset action on AccessInterfaces
//!
class SystemModelReseter final : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelReseter() = default;
  SystemModelReseter(bool doSynchronousReset) : m_doSynchronousReset(doSynchronousReset) {}

  // Implement SystemModelVisitor interface
  //
  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  // ---------------- Private  Methods
  //
  void VisitParent(ParentNode& parent);

  // ---------------- Private  Fields
  //
  private:
  bool m_doSynchronousReset = false;  //!< When true, reset on AccessInterface will be done by issuing a synchronous reset sequence
};
//
//  End of SystemModelReseter class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined SYSTEMMODELRESETER_H__29F98A19_457C_4C39_688E_5D1C187D8728__INCLUDED_

//===========================================================================
// End of SystemModelReseter.hpp
//===========================================================================
