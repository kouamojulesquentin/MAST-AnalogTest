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
#include <experimental/string_view>

struct command_context;

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
  ~OpenOCDProtocol();
  OpenOCDProtocol() = delete;
  OpenOCDProtocol(std::experimental::string_view configFilePath, std::experimental::string_view designName, int iIrLength);

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Gets the number of derivations supported by the specific protocol
  //!
  //! @note Derivation id 0 is reserved for reset operation, so protocol must support a least two derivations
  //!
  //! @return The number of supported derivation (including pseudo derivation 0 for reset)
  //!
  virtual uint32_t MaxSupportedDerivations() const override { return 3u; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "OpenOCD"; }


  // ---------------- Private  Fields
  //
  private:
  #ifndef _WIN32
  command_context* m_cmd_ctx;          //!< OpenOCD structure containing the whole context of the current OpenOCD session configuration.

  //! Some adapters and JTAG TAP do not provide a TRST pin.
  //! If jtag_get_reset_config() attribute has the RESET_HAS_TRST flag set, we do it by hardware, otherwise we pass by the state machine.
  bool             m_supportTrst = false;

  #endif
};
//
//  End of OpenOCDProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined OPENOCDPROTOCOL_H__1670F41A_CA14_4C39_E89B_D855CFF1A020__INCLUDED_

//===========================================================================
// End of OpenOCDProtocol.hpp
//===========================================================================
