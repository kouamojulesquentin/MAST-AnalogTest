//===========================================================================
//                           I2C_RawPlayer.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file I2C_RawPlayer.hpp
//!
//! Declares I2C_RawPlayer class
//!
//===========================================================================


#ifndef I2C_RAWPLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_
  #define I2C_RAWPLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_

#include "AccessInterfaceRawProtocol.hpp"
#include "Mast_Core_export.hpp"

#include <string>
#include <vector>
#include <initializer_list>


namespace mast
{
//! Constructs I2C commands for I2C related AccessInterfaceProtocol
//!
class MAST_CORE_EXPORT I2C_RawPlayer final : public AccessInterfaceRawProtocol
{
 #define I2C_ADDRESSFRAME_CYCLES 	9
 #define I2C_DATAFRAME_CYCLES 		9
 #define I2C_STOP_CYCLES 		1
 
  // ---------------- Public  Methods
  //
  public:
  virtual ~I2C_RawPlayer() = default;
  I2C_RawPlayer() = delete;

  I2C_RawPlayer(std::vector<uint32_t> addresses);

  //! Initializes with addresses and optional prefix defined by a string
  //!
  I2C_RawPlayer(const std::string& parameters);

  //! Gets the number of channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two channels
  //!
  //! @return The number of supported channel (including pseudo channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return m_addresses.size(); }

   //! Does any callback required to transfer scan data to and from SUT
  //!
  //! @param channelId   Identifies the channel to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoCallback(RVFRequest Request, uint32_t channelId) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "I2C_RAW"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

 // ---------------- Protected Methods
  //
  protected:

  //! Creates an I2C command associated to channel identifier and BinaryVector to send to SUT
  //!
  std::string CreateI2CCommand(uint32_t channelId, const BinaryVector& toSutData);

  //! Returns address for specified channel
  //!
  uint32_t GetAddress(uint32_t channelId) const;

  // ---------------- Private  Fields
  //
  std::vector<uint32_t> m_addresses;     //!< Addresses associated with channel ids (at offset 1 for channel 1)
};
//
//  End of I2C_RawPlayer class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined I2C_RawPlayer_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_

//===========================================================================
// End of I2C_RawPlayer.hpp
//===========================================================================
