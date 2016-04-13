//===========================================================================
//                           DataCycleWriteVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DataCycleWriteVisitor.hpp
//!
//! Declares class DataCycleWriteVisitor
//!
//===========================================================================


#ifndef DATACYCLEWRITEVISITOR_H__690A42A6_C51E_45E8_EA8B_6D8DF98F8FA8__INCLUDED_
  #define DATACYCLEWRITEVISITOR_H__690A42A6_C51E_45E8_EA8B_6D8DF98F8FA8__INCLUDED_

#include "SystemModelVisitor.hpp"

namespace mast
{
//! System model visitors for packing register bits in order to latter shift them into SUT
//!
class DataCycleWriteVisitor final : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~DataCycleWriteVisitor() = default;
  DataCycleWriteVisitor()  = default;


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
//  End of DataCycleWriteVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DATACYCLEWRITEVISITOR_H__690A42A6_C51E_45E8_EA8B_6D8DF98F8FA8__INCLUDED_

//===========================================================================
// End of DataCycleWriteVisitor.hpp
//===========================================================================
