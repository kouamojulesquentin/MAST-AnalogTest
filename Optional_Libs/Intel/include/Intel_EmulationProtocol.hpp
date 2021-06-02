//===========================================================================
//                           Intel_EmulationProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Intel_EmulationProtocol.hpp
//!
//! Declares Intel_EmulationProtocol class
//!
//===========================================================================


#ifndef Intel_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define Intel_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "Intel_Packet_Player.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback AccessInterfaceProtocol while logging Intel commands that
//! a real Intel protocol would issue
//!
class Intel_EmulationProtocol final : public Intel_Packet_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Intel_EmulationProtocol() = default;
  Intel_EmulationProtocol() = delete;
  Intel_EmulationProtocol(std::vector<uint32_t> Region_addresses);
  Intel_EmulationProtocol(std::string initialisation_string);

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param channelId   Identifies the channel to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoCallback(uint32_t channelId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Gets the number of channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two channels
  //!
  //! @return The number of supported channel (including pseudo channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return m_n_chains; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Intel_Emulation"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Protected Methods
  //
};
//
//  End of Intel_EmulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined Intel_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of Intel_EmulationProtocol.hpp
//===========================================================================
