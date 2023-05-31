//===========================================================================
//                           AccessInterfaceTranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceTranslatorProtocol.hpp
//!
//! Declares class AccessInterfaceTranslatorProtocol
//!
//===========================================================================


#ifndef ACCESSINTERFACETRANSLATORPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define ACCESSINTERFACETRANSLATORPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "BinaryVector.hpp"
#include "CallbackIds.hpp"
#include "RVF.hpp"
#include <experimental/string_view>

namespace mast
{
//! Defines interface for protocols used by AccessInterface
//!
class AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~AccessInterfaceTranslatorProtocol() = default;

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual RVFResponse TransformationCallback(RVFRequest current_request) = 0;

  //! Returns readable type of Transformationprotocol (Emulation, JTAG_to_I2C, ...)
  //!
  virtual std::experimental::string_view KindName() const = 0;

  //! Cycles used so far by the interface to process all toVectors received so far
  uint64_t getElapsedCycles() {return m_InternalCycles;};

  //! Modify Elapsed Cycles to account for Interface activity (used by Callabacks)
  void setElapsedCycles(uint64_t newElapsedCycles) {m_InternalCycles=newElapsedCycles;};
 
  //! Increase Elapsed Cycles to account for Interface activity (used by Callabacks)
  void increaseElapsedCycles(uint64_t ElapsedCyclesIncrease) {m_InternalCycles+=ElapsedCyclesIncrease;};

  // ---------------- Protected Methods
  //
  protected:
  AccessInterfaceTranslatorProtocol()          = default;
  uint64_t 			m_InternalCycles=0;   //! Cycles used by the interface to process all toVectors received so far
};
//
//  End of AccessInterfaceTranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACETRANSLATORPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of AccessInterfaceTranslatorProtocol.hpp
//===========================================================================
