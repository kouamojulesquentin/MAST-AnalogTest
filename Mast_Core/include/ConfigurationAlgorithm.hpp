//===========================================================================
//                           ConfigurationAlgorithm.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigurationAlgorithm.hpp
//!
//! Declares ConfigurationAlgorithm class
//!
//===========================================================================

#ifndef CONFIGURATIONALGORITHM_H__5DB75F2E_2048_4019_E9B3_3C893FB3DBC6__INCLUDED_
  #define CONFIGURATIONALGORITHM_H__5DB75F2E_2048_4019_E9B3_3C893FB3DBC6__INCLUDED_

#include "Mast_Core_export.hpp"
#include <cstdint>
#include <memory>

namespace mast
{
class Linker;
class SystemModelNode;

//! Defines interface for algorithms to select a Linker path (derivation)
//!
class MAST_CORE_EXPORT ConfigurationAlgorithm
{
  public:
  virtual ~ConfigurationAlgorithm() = default;

  //! Does what need to be done when ConfigureVisitor begins to process a new linker
  //!
  //! @note Processing of a linker is generally done while processing another linker
  //!       at a higher hierarchical level
  //!
  //! @param linker The linker currently configured
  //!
  virtual void StartLinkerProcessing(Linker& linker) = 0;

  //! Does what is necessary for a pending linker child
  //!
  //! @param linker         The linker currently configured
  //! @param pathIdentifier Pending child path identifier
  //! @param child          Pending child
  //!
  virtual void ProcessPending(Linker& linker, uint32_t pathIdentifier, std::shared_ptr<SystemModelNode> child) = 0;

  //! Does what need to be done when all linker children have been processed
  //!
  //! @note Must be called after linker pending state has been updated (this permit stateless algorithm)
  //! @note Post-condition: a selection choice must have done (by this method or by one call of ProcessPending for current linker)
  //!
  //! @param linker The linker currently configured
  //!
  virtual void ResolvePendings(Linker& linker) = 0;

  protected:
  ConfigurationAlgorithm() = default;
};
//
//  End of ConfigurationAlgorithm class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CONFIGURATIONALGORITHM_H__5DB75F2E_2048_4019_E9B3_3C893FB3DBC6__INCLUDED_

//===========================================================================
// End of ConfigurationAlgorithm.hpp
//===========================================================================
