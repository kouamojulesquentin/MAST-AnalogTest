//===========================================================================
//                           ConfigureVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureVisitor.hpp
//!
//! Declares class ConfigureVisitor
//!
//===========================================================================


#ifndef CONFIGUREVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_
  #define CONFIGUREVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_

#include "SystemModelVisitor.hpp"

namespace mast
{
//! System model tree visitor for configuration
//!
//!
class ConfigureVisitor : SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~ConfigureVisitor() = default;
  ConfigureVisitor()  = default;

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

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
//  End of ConfigureVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CONFIGUREVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_

//===========================================================================
// End of ConfigureVisitor.hpp
//===========================================================================
