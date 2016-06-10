//===========================================================================
//                           DefaultConfigurationAlgorithm.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultConfigurationAlgorithm.hpp
//!
//! Declares DefaultConfigurationAlgorithm class
//!
//===========================================================================

#ifndef DEFAULTCONFIGURATIONALGORITHM_H__EB531F2D_8842_4ECC_719A_71B6B792AF65__INCLUDED_
  #define DEFAULTCONFIGURATIONALGORITHM_H__EB531F2D_8842_4ECC_719A_71B6B792AF65__INCLUDED_

#include "ConfigurationAlgorithm.hpp"
#include "Platform.hpp"

namespace mast
{
//! Implements defaults algorithm to select appropriate paths while during configuration
//!
//! @note This algorithm selects last (with highest path identifier) pending path that is
//!       not already selected (that way pending paths will be streamed as fast as possible)
//!
//! @note This is appropriate when registers are not modified between data cycles
class DLL_EXPORT DefaultConfigurationAlgorithm final : public ConfigurationAlgorithm
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~DefaultConfigurationAlgorithm() = default;
  DefaultConfigurationAlgorithm()  = default;

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
//  End of DefaultConfigurationAlgorithm class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DEFAULTCONFIGURATIONALGORITHM_H__EB531F2D_8842_4ECC_719A_71B6B792AF65__INCLUDED_

//===========================================================================
// End of DefaultConfigurationAlgorithm.hpp
//===========================================================================
