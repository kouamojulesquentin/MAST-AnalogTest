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

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //!                       Derivation 0 is reserved for reset operation
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) = 0;

  //! Gets the number of derivations supported by the specific protocol
  //!
  //! @note Derivation id 0 is reserved for reset operation, so protocol must support a least two derivations
  //!
  //! @return The number of supported derivation (including pseudo derivation 0 for reset)
  //!
  virtual uint32_t MaxSupportedDerivations() const = 0;

  //! Returns readable type of protocol (I2C, SVF_Simu, OpenOCD, SPI...)
  //!
  virtual std::experimental::string_view KindName() const = 0;

  //! readable identifier for given endpoint
  //!
  std::string CallbackId(int EndPoint) { 
         return AccessInterfaceProtocol::KindName().to_string()+"_"+std::to_string(EndPoint); 
	 };


  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset) = 0;

  // ---------------- Protected Methods
  //
  protected:
  AccessInterfaceProtocol()          = default;
};
//
//  End of AccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACEPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of AccessInterfaceProtocol.hpp
//===========================================================================
