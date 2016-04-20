//===========================================================================
//                           PostConditionVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PostConditionVisitor.hpp
//!
//! Declares class PostConditionVisitor
//!
//===========================================================================


#ifndef POSTCONDITIONVISITOR_H__7935041E_F5D4_45C8_DFA1_D0EF215F9EF9__INCLUDED_
  #define POSTCONDITIONVISITOR_H__7935041E_F5D4_45C8_DFA1_D0EF215F9EF9__INCLUDED_

#include "SystemModelVisitor.hpp"

namespace mast
{
//! System model visitors for checking preconditions
//!
class PostConditionVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~PostConditionVisitor() = default;
  PostConditionVisitor()  = default;

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
//  End of PostConditionVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined POSTCONDITIONVISITOR_H__7935041E_F5D4_45C8_DFA1_D0EF215F9EF9__INCLUDED_

//===========================================================================
// End of PostConditionVisitor.hpp
//===========================================================================
