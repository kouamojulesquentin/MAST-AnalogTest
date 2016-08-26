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

extern "C" {
    #include <openocd/config.h>    
    #include <jtag/driver.h>
    #include <jtag/jtag.h>
    #include <transport/transport.h>
    #include <helper/ioutil.h>
    #include <helper/util.h>
    #include <helper/configuration.h>
    #include <flash/nor/core.h>
    #include <flash/nand/core.h>
    #include <pld/pld.h>
    #include <flash/mflash.h>

    #include <server/server.h>
    #include <server/gdb_server.h>
}
#include "Utility.hpp"

static const char openocd_startup_tcl[] = {
#include "startup_tcl.inc"
0 /* Terminate with zero */
};

extern "C" struct command_context *setup_command_handler(Jim_Interp *interp);


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
  OpenOCDProtocol()          = delete;
  OpenOCDProtocol(std::experimental::string_view configFilePath,
                    std::experimental::string_view designName, int iIrLength);

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
    struct command_context *cmd_ctx;
    

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
