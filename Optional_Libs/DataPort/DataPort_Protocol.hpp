//===========================================================================
//                           DataPort_Protocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DataPort_Protocol.hpp
//!
//! Declares DataPort_Protocol class
//!
//===========================================================================


#ifndef DataPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define Data_PROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "AccessInterfaceRawProtocol.hpp"
#include "Mast_Core_export.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback AccessInterfaceProtocol while logging Data commands that
//! a real Data protocol would issue
//!
class MAST_CORE_EXPORT DataPort_Protocol final: public AccessInterfaceRawProtocol
{

  // ---------------- Public  Methods
  //
  public:
  virtual ~DataPort_Protocol() = default;
  DataPort_Protocol() = delete;
  DataPort_Protocol(std::string portname);

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
  virtual uint32_t MaxSupportedChannels() const override { return 1; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "DataPort"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Protected Methods
  //
  protected:
  std::string m_portname; //Top Level (ex: ATE) identifier for the DataPort
};
//
//  End of DataPort_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined DataPort_Protocol_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of DataPort_Protocol.hpp
//===========================================================================
