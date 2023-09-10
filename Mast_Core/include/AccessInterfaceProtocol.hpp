//===========================================================================
//                           AccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocol.hpp
//!
//! Declares class AccessInterfaceProtocol
//!
//===========================================================================


#ifndef ACCESSINTERFACEPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define ACCESSINTERFACEPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "BinaryVector.hpp"
#include "RVF.hpp"
#include <experimental/string_view>

namespace mast
{
//! Defines interface for protocols used by AccessInterface
//!
class AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~AccessInterfaceProtocol() = default;

  //! Does any callback required to transfer scan data to and from SUT
  //!
  //! @param channelId   Identifies the channel to act for (zero based)
  //!                       Channel 0 is reserved for reset operation
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual RVFResponse DoCallback(RVFRequest Request, uint32_t channelId) = 0;

  //! Gets the number of channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two channels
  //!
  //! @return The number of supported channel (including pseudo channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const = 0;

  //! Returns readable type of protocol (I2C, SVF_Simu, OpenOCD, SPI...)
  //!
  virtual std::experimental::string_view KindName() const = 0;

  //! readable identifier for given channel
  //!
  std::string CallbackId(int Channel) const { 
         return KindName().to_string()+"_"+std::to_string(Channel); 
         };
  
  //! Cycles used so far by the interface to process all toVectors received so far
  uint64_t getElapsedCycles() {return m_InternalCycles;};

  //! Modify Elapsed Cycles to account for Interface activity (used by Callabacks)
  void setElapsedCycles(uint64_t newElapsedCycles) {m_InternalCycles=newElapsedCycles;};
 
  //! Increase Elapsed Cycles to account for Interface activity (used by Callabacks)
  void increaseElapsedCycles(uint64_t ElapsedCyclesIncrease) {m_InternalCycles+=ElapsedCyclesIncrease;};

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset) = 0;

  // ---------------- Protected Methods
  //
  protected:
  AccessInterfaceProtocol()          = default;
  uint64_t 			m_InternalCycles=0;   //! Cycles used by the interface to process all toVectors received so far

};
//
//  End of AccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACEPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of AccessInterfaceProtocol.hpp
//===========================================================================
