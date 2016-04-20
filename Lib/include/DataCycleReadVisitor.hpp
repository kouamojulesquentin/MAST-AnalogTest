//===========================================================================
//                           DataCycleReadVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DataCycleReadVisitor.hpp
//!
//! Declares class DataCycleReadVisitor
//!
//===========================================================================


#ifndef DATACYCLEREADVISITOR_H__5BE9418F_DF46_43F6_6B9B_5EDE6F8EEC8B__INCLUDED_
  #define DATACYCLEREADVISITOR_H__5BE9418F_DF46_43F6_6B9B_5EDE6F8EEC8B__INCLUDED_

namespace mast
{
//! System model visitors for unpacking register bits shifted from SUT and updating
//! Register nodes.
//!
class DataCycleReadVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~DataCycleReadVisitor() = default;
  DataCycleReadVisitor()  = default;

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;
  virtual void VisitTap             (Tap&             tap)             override;

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
};
//
//  End of DataCycleReadVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DATACYCLEREADVISITOR_H__5BE9418F_DF46_43F6_6B9B_5EDE6F8EEC8B__INCLUDED_

//===========================================================================
// End of DataCycleReadVisitor.hpp
//===========================================================================
