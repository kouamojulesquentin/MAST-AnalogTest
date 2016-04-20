//===========================================================================
//                           GmlPrinterVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file GmlPrinterVisitor.hpp
//!
//! Declares class GmlPrinterVisitor
//!
//===========================================================================


#ifndef GMLPRINTERVISITOR_H__2DA805E1_7CD4_4D2D_2AB8_5A6B17EEEB3D__INCLUDED_
  #define GMLPRINTERVISITOR_H__2DA805E1_7CD4_4D2D_2AB8_5A6B17EEEB3D__INCLUDED_

#include "SystemModelVisitor.hpp"

namespace mast
{
//! System model visitors for creation of a GML formated representation of the
//! system mode tree
//!
class GmlPrinterVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~GmlPrinterVisitor() = default;
  GmlPrinterVisitor()  = default;

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
//  End of GmlPrinterVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined GMLPRINTERVISITOR_H__2DA805E1_7CD4_4D2D_2AB8_5A6B17EEEB3D__INCLUDED_

//===========================================================================
// End of GmlPrinterVisitor.hpp
//===========================================================================
