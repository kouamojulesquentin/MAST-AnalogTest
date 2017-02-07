//===========================================================================
//                           SPI_Protocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SPI_Protocol.hpp
//!
//! Declares SPI_Protocol class
//!
//===========================================================================


#ifndef SPI_PROTOCOL_H__E91437E1_A4B5_4314_9693_9EDCD5A6A951__INCLUDED_
  #define SPI_PROTOCOL_H__E91437E1_A4B5_4314_9693_9EDCD5A6A951__INCLUDED_

#include "SPI_Player.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

struct ftdi_context;
struct ftdispi_context;

namespace mast
{
//! Implement loopback AccessInterfaceProtocol while logging I2C commands that
//! a real I2C protocol would issue
//!
class DLL_EXPORT SPI_Protocol final : public SPI_Player
{
  // ---------------- Public  Methods
  //
  public:
  ~SPI_Protocol();
  SPI_Protocol() = delete;
  SPI_Protocol(std::vector<uint32_t>          chipSelectCommands,
               std::vector<uint32_t>          readCommands,
               std::vector<uint32_t>          writeCommands,
               std::experimental::string_view commandsPrefix = "",
               uint16_t                       usbDeviceID    = 0x6010u);

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SPI_FTDI"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  protected:
  #ifdef  USE_LIBFTDISPI
  ftdi_context*    m_ftdi_ctx    = nullptr;
  ftdispi_context* m_ftdispi_ctx = nullptr;
  #endif
};
//
//  End of SPI_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SPI_PROTOCOL_H__E91437E1_A4B5_4314_9693_9EDCD5A6A951__INCLUDED_

//===========================================================================
// End of SPI_Protocol.hpp
//===========================================================================
