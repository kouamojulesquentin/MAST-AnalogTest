//===========================================================================
//                           ConfigureAlgorithm_LastOrDefault_Greedy.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithm_LastOrDefault_Greedy.hpp
//!
//! Declares ConfigureAlgorithm_LastOrDefault_Greedy class
//!
//===========================================================================

#ifndef CONFIGUREALGORITHM_LASTORDEFAULT_GREEDY_H__94D5C729_3AD4_4FD5_EEA4_7613390B5BCC__INCLUDED_
  #define CONFIGUREALGORITHM_LASTORDEFAULT_GREEDY_H__94D5C729_3AD4_4FD5_EEA4_7613390B5BCC__INCLUDED_

#include "ConfigurationAlgorithm.hpp"
#include "Mast_Core_export.hpp"

namespace mast
{
//! Implements algorithm to select last pending endpoint or endpoint '0' when all endpoint
//! are bound to be "served"
//!
//! @note This algorithm selects last (with highest path identifier) pending path that is
//!       not already selected (that way pending paths will be streamed as fast as possible)
//!
//! @note This is appropriate when registers are not modified between data cycles
//!
class MAST_CORE_EXPORT ConfigureAlgorithm_LastOrDefault_Greedy final : public ConfigurationAlgorithm
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~ConfigureAlgorithm_LastOrDefault_Greedy() = default;
  ConfigureAlgorithm_LastOrDefault_Greedy()  = default;

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

//! Selects path 0 when linker is not pending, otherwise if there is just one
//! pending, select path 0 if currently selected endpoint is already active
//!
//! @note Must be called after linker pending count has been updated
//! @note Post-condition: a selection choice must have been done (by this method
//!       or by one call of ProcessPending for current linker)
//!
//! @param linker The linker currently configured
//!
  virtual void ResolvePendings(Linker& linker) override;
};
//
//  End of ConfigureAlgorithm_LastOrDefault_Greedy class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined CONFIGUREALGORITHM_LASTORDEFAULT_GREEDY_H__94D5C729_3AD4_4FD5_EEA4_7613390B5BCC__INCLUDED_
//===========================================================================
// End of ConfigureAlgorithm_LastOrDefault_Greedy.hpp
//===========================================================================
