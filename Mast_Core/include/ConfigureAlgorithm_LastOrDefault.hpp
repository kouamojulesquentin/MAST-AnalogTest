//===========================================================================
//                           ConfigureAlgorithm_LastOrDefault.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithm_LastOrDefault.hpp
//!
//! Declares ConfigureAlgorithm_LastOrDefault class
//!
//===========================================================================

#ifndef CONFIGUREALGORITHM_LASTORDEFAULT_H__E633E366_59FF_447A_1283_B30BD9795B06__INCLUDED_
  #define CONFIGUREALGORITHM_LASTORDEFAULT_H__E633E366_59FF_447A_1283_B30BD9795B06__INCLUDED_

#include "ConfigurationAlgorithm.hpp"
#include "Mast_Core_export.hpp"

namespace mast
{
//! Implements defaults algorithm to select appropriate paths while during configuration
//!
//! @note This algorithm selects last (with highest path identifier) pending path that is
//!       not already selected (that way pending paths will be streamed as fast as possible)
//!
//! @note This is appropriate when registers are not modified between data cycles
class MAST_CORE_EXPORT ConfigureAlgorithm_LastOrDefault final : public ConfigurationAlgorithm
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~ConfigureAlgorithm_LastOrDefault() = default;
  ConfigureAlgorithm_LastOrDefault()  = default;

  //! Does what need to be done when ConfigureVisitor begins to process a new linker
  //!
  //! @note Processing of a linker is generally done while processing another linker
  //!       at a higher hierarchical level
  //!
  //! @param linker The linker currently configured
  //!
  virtual void StartLinkerProcessing(Linker& /* linker */) override {}

  //! Does what is necessary for a pending linker child
  //!
  //! @param linker         The linker currently configured
  //! @param pathIdentifier Pending child path identifier
  //! @param child          Pending child
  //!
  virtual void ProcessPending(Linker& linker, uint32_t pathIdentifier, std::shared_ptr<SystemModelNode> child) override;

  //! Does what need to be done when all linker children have been processed
  //!
  //! @note Must be called after linker pending state has been updated (this permit stateless algorithm)
  //! @note Post-condition: a selection choice must have done (by this method or by one call of ProcessPending for current linker)
  //!
  //! @param linker The linker currently configured
  //!
  virtual void ResolvePendings(Linker& linker) override;
};
//
//  End of ConfigureAlgorithm_LastOrDefault class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined CONFIGUREALGORITHM_LASTORDEFAULT_H__E633E366_59FF_447A_1283_B30BD9795B06__INCLUDED_
//===========================================================================
// End of ConfigureAlgorithm_LastOrDefault.hpp
//===========================================================================
