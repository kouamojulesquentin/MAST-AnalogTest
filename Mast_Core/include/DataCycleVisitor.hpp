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

#ifndef DATACYCLEVISITOR_H__24D9D68B_89A7_4534_A995_73BC5F29EBDB__INCLUDED_
  #define DATACYCLEVISITOR_H__24D9D68B_89A7_4534_A995_73BC5F29EBDB__INCLUDED_

#include "SystemModelVisitor.hpp"

#include <memory>

namespace mast
{
class SystemModelManager_impl;

//! System model tree visitor for Data Cycle triggering
//!
//!
class MAST_CORE_EXPORT DataCycleVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~DataCycleVisitor() = default;
  DataCycleVisitor(SystemModelManager_impl* manager)
    : m_manager (manager)
  {
  }

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  // ---------------- Private  Fields
  //
  private:
  SystemModelManager_impl* m_manager = nullptr; //!< The current system model
};
//
//  End of ConfigureVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined DATACYCLEVISITOR_H__24D9D68B_89A7_4534_A995_73BC5F29EBDB__INCLUDED_

//===========================================================================
// End of ConfigureVisitor.hpp
//===========================================================================
