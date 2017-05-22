//===========================================================================
//                           ConfigureAlgorithm_Last_Lazy.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithm_Last_Lazy.hpp
//!
//! Declares ConfigureAlgorithm_Last_Lazy class
//!
//===========================================================================

#ifndef CONFIGUREALGORITHM_LAST_LAZY_H__EC19B64F_D34D_4A92_C8BD_2BDA4BC9A33__INCLUDED_
  #define CONFIGUREALGORITHM_LAST_LAZY_H__EC19B64F_D34D_4A92_C8BD_2BDA4BC9A33__INCLUDED_

#include "ConfigurationAlgorithm.hpp"
#include "Mast_Core_export.hpp"

namespace mast
{
//! Implements algorithm to select appropriate paths during configuration
//!
//! @note This algorithm selects last (with highest path identifier) pending path that is
//!       not already selected
//!
//! @note This is appropriate when registers may be modified between data cycles
//!
class MAST_CORE_EXPORT ConfigureAlgorithm_Last_Lazy final : public ConfigurationAlgorithm
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~ConfigureAlgorithm_Last_Lazy() = default;
  ConfigureAlgorithm_Last_Lazy()  = default;

  //! Does nothing when ConfigureVisitor begins to process a new linker
  //!
  //! @param linker The linker currently configured
  //!
  virtual void StartLinkerProcessing(Linker& /* linker */) override {}

  //! Selects specified child if pending (and not active yet)
  //!
  //! @param linker         The linker currently configured
  //! @param pathIdentifier Pending child path identifier
  //! @param child          Pending child
  //!
  virtual void ProcessPending(Linker& linker, uint32_t pathIdentifier, std::shared_ptr<SystemModelNode> child) override;

  //! Does nothing more when all linker children have been processed
  //!
  //! @param linker The linker currently configured
  //!
  virtual void ResolvePendings(Linker& /* linker */) override {}
};
//
//  End of ConfigureAlgorithm_Last_Lazy class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined CONFIGUREALGORITHM_LAST_LAZY_H__EC19B64F_D34D_4A92_C8BD_2BDA4BC9A33__INCLUDED_
//===========================================================================
// End of ConfigureAlgorithm_Last_Lazy.hpp
//===========================================================================
