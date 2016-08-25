//===========================================================================
//                           OpenOCDProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file OpenOCDProtocol.hpp
//!
//! Declares OpenOCDProtocol class
//!
//===========================================================================


#ifndef OPENOCDPROTOCOL_H__1670F41A_CA14_4C39_E89B_D855CFF1A020__INCLUDED_
  #define OPENOCDPROTOCOL_H__1670F41A_CA14_4C39_E89B_D855CFF1A020__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement AccessInterfaceProtocol by sending SVF commands to file and receiving
//! binary digits that represents bit values shifted from SUT
//!
//!
class DLL_EXPORT OpenOCDProtocol final : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~OpenOCDProtocol() = default;
  OpenOCDProtocol()          = delete;
  OpenOCDProtocol(std::experimental::string_view configFilePath);

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
  virtual std::experimental::string_view KindName() const override { return "OpenOCD"; }

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
};
//
//  End of OpenOCDProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined OPENOCDPROTOCOL_H__1670F41A_CA14_4C39_E89B_D855CFF1A020__INCLUDED_

//===========================================================================
// End of OpenOCDProtocol.hpp
//===========================================================================
