//===========================================================================
//                           LoopbackAccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file LoopbackAccessInterfaceProtocol.hpp
//!
//! Declares LoopbackAccessInterfaceProtocol class
//!
//===========================================================================

#ifndef LOOPBACKACCESSINTERFACEPROTOCOL_H__7EA95A6E_19D8_4B68_B28A_6C501BCEC0FA__INCLUDED_
  #define LOOPBACKACCESSINTERFACEPROTOCOL_H__7EA95A6E_19D8_4B68_B28A_6C501BCEC0FA__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "Utility.hpp"

namespace mast
{
//! Implements AccessInterfaceProtocol that only loop back.
//!
//! @note This is mainly provided for tests purposes
//!
class MAST_CORE_EXPORT LoopbackAccessInterfaceProtocol : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~LoopbackAccessInterfaceProtocol() = default;
  LoopbackAccessInterfaceProtocol()  = default;

  //! Does any callback required to transfer scan data to and from SUT
  //!
  //! @param ChannelId   Identifies the Channel to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual BinaryVector DoCallback(std::string CallbackId, uint32_t channelId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Gets the number of Channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two Channels
  //!
  //! @return The number of supported Channels (including pseudo Channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return UINT32_MAX; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Loopback"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset) override;
};
//
//  End of LoopbackAccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined LOOPBACKACCESSINTERFACEPROTOCOL_H__7EA95A6E_19D8_4B68_B28A_6C501BCEC0FA__INCLUDED_

//===========================================================================
// End of LoopbackAccessInterfaceProtocol.hpp
//===========================================================================
