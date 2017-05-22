//===========================================================================
//                           MismatchesCollector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MismatchesCollector.hpp
//!
//! Declares MismatchesCollector class
//!
//===========================================================================

#ifndef MISMATCHESCOLLECTOR_H__C06208AD_5DD_4B7C_C494_3A7E979A167F__INCLUDED_
  #define MISMATCHESCOLLECTOR_H__C06208AD_5DD_4B7C_C494_3A7E979A167F__INCLUDED_

#include "SystemModelVisitor.hpp"
#include <cstdint>

namespace mast
{
//! Collects mismatches count along a SystemModel sub-tree
//!
class MismatchesCollector final : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~MismatchesCollector() = default;
  MismatchesCollector()  = default;
  MismatchesCollector(bool clearCounter) : m_clearCounter (clearCounter) {}

  // Implement SystemModelVisitor interface
  //
  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  // Getters
  uint32_t MismatchesCount() const { return m_mismatchesCount; }  //!< Returns cumulated mismatches count
  bool     ClearCounter()    const { return m_clearCounter;    }  //!< Returns whether mismatches count should be cleared once accumulated

  // Setters
  void MismatchesCount (uint32_t mismatchesCount) { m_mismatchesCount = mismatchesCount; }  //!< Forces current mismatches count
  void ClearCounter    (bool     clearCounter)    { m_clearCounter    = clearCounter;    }  //!< Sets whether mismatches count should be cleared once accumulated

  // ---------------- Private Fields
  //
  public:
  bool     m_clearCounter    = false; //!< When true, registers mismatches count is clear just after having collecting its mismatches count
  uint32_t m_mismatchesCount = 0;     //!< Number of mismatchesCount collected so far
};
//
//  End of MismatchesCollector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined MISMATCHESCOLLECTOR_H__C06208AD_5DD_4B7C_C494_3A7E979A167F__INCLUDED_

//===========================================================================
// End of MismatchesCollector.hpp
//===========================================================================
