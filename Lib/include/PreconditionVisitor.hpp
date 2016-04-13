//===========================================================================
//                           PreconditionVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PreconditionVisitor.hpp
//!
//! Declares class PreconditionVisitor
//!
//===========================================================================


#ifndef PRECONDITIONVISITOR_H__D45EFD98_B18B_4C25_B3A8_9069DD48F5E__INCLUDED_
  #define PRECONDITIONVISITOR_H__D45EFD98_B18B_4C25_B3A8_9069DD48F5E__INCLUDED_

#include "SystemModelVisitor.hpp"

namespace mast
{
//! System model visitors for checking preconditions
//!
class PreconditionVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~PreconditionVisitor() = default;
  PreconditionVisitor()  = default;


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
//  End of PreconditionVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PRECONDITIONVISITOR_H__D45EFD98_B18B_4C25_B3A8_9069DD48F5E__INCLUDED_

//===========================================================================
// End of PreconditionVisitor.hpp
//===========================================================================
