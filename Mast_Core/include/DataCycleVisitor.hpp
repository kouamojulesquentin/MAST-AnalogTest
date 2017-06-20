//===========================================================================
//                           ConfigureVisitor.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DataCycleVisitor.hpp
//!
//! Declares class DataCycleVisitor
//!
//===========================================================================


#ifndef DATACYCLEVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_
  #define DATACYCLEVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_

#include "SystemModelVisitor.hpp"
#include "SystemModelManager_impl.hpp"

#include <memory>

namespace mast
{
//! System model tree visitor for Data Cycle triggering
//!
//!
class MAST_CORE_EXPORT DataCycleVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~DataCycleVisitor() = default;
  DataCycleVisitor(SystemModelManager_impl* current_smm)
  : m_current_smm (current_smm)
  {
  }

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        )             override;

  // ---------------- Private  Fields
  //
  private:
  SystemModelManager_impl*       m_current_smm;        //!< The current system model
};
//
//  End of ConfigureVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CONFIGUREVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_

//===========================================================================
// End of ConfigureVisitor.hpp
//===========================================================================
