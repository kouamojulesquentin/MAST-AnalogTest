//===========================================================================
//                           I2C_EmulationProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file I2C_EmulationProtocol.hpp
//!
//! Declares I2C_EmulationProtocol class
//!
//===========================================================================


#ifndef I2C_EMULATIONPROTOCOL_H__E91437E1_A4B5_4314_9693_9EDCD5A6A951__INCLUDED_
  #define I2C_EMULATIONPROTOCOL_H__E91437E1_A4B5_4314_9693_9EDCD5A6A951__INCLUDED_

#include "I2C_Player.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback AccessInterfaceProtocol while logging I2C commands that
//! a real I2C protocol would issue
//!
class DLL_EXPORT I2C_EmulationProtocol final : public I2C_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~I2C_EmulationProtocol() = default;
  I2C_EmulationProtocol() = delete;
  I2C_EmulationProtocol(std::initializer_list<uint32_t> addresses, std::experimental::string_view commandsPrefix = "");
  I2C_EmulationProtocol(std::vector<uint32_t>           addresses, std::experimental::string_view commandsPrefix = "");

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
  virtual std::experimental::string_view KindName() const override { return "I2C_Emulation"; }
};
//
//  End of I2C_EmulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined I2C_EMULATIONPROTOCOL_H__E91437E1_A4B5_4314_9693_9EDCD5A6A951__INCLUDED_

//===========================================================================
// End of I2C_EmulationProtocol.hpp
//===========================================================================
