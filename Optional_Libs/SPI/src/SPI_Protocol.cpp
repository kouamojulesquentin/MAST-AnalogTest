//===========================================================================
//                           SPI_Protocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SPI_Protocol.cpp
//!
//! Implements class SPI_Protocol
//!
//===========================================================================

#include "SPI_Protocol.hpp"
#include "g3log/g3log.hpp"

#include "Utility.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <ftdispi.h>

using std::experimental::string_view;
using std::string;
using std::ofstream;
using std::ifstream;
using std::initializer_list;
using std::vector;
using std::make_unique;
using namespace mast;
using namespace std::string_literals;


//! Free allocated resources
//!
SPI_Protocol::~SPI_Protocol()
{
  if (m_ftdispi_ctx)
  {
    ftdispi_close(m_ftdispi_ctx.get(), 1);
  }
}

//! Initializes ftdi spi library
//!
//! @note AccessInterface endpoints ids start from 1 (0 is reserved for reset command)
//!
//! @param chipSelectCommands   SPI chip select addresses for each endpoint id
//! @param readCommands         SPI read addresses for each endpoint id
//! @param writeCommands        SPI write addresses for each endpoint id
//! @param commandsPrefix       Optional text that will be prepended to actual SPI command
//! @param usbDeviceID          Optional (machine specific) USB device identifier
//!
SPI_Protocol::SPI_Protocol (vector<uint32_t> chipSelectCommands,
                            vector<uint32_t> readCommands,
                            vector<uint32_t> writeCommands,
                            string_view      commandsPrefix,
                            uint16_t         usbDeviceID)
  : SPI_Player(std::move(chipSelectCommands), std::move(readCommands), std::move(writeCommands), commandsPrefix)
{
  // Here we initialize libftdi-related structures (used later by libftdispi)
  m_ftdi_ctx = make_unique<ftdi_context>();

  if (ftdi_init(m_ftdi_ctx.get()) < 0)
  {
    LOG(ERROR_LVL) << "ftdi_init failed";
  }

  // Try to open channel B, reverting to ANY if error
  if (ftdi_set_interface(m_ftdi_ctx.get(), INTERFACE_B) > 0)
  {
    LOG(INFO) << "ftdi open on Channel B";
  }
  else
  {
    ftdi_set_interface(m_ftdi_ctx.get(), INTERFACE_ANY);
    LOG(INFO) << "ftdi open on Channel A";
  }

  int ret = ftdi_usb_open(m_ftdi_ctx.get(), 0x0403, usbDeviceID);

  if (ret < 0 && ret != -5)
  {
    THROW_RUNTIME_ERROR("OPEN: "s + ftdi_get_error_string(m_ftdi_ctx.get()));
  }

  // Now we initialize libftdispi configuration structures
  m_ftdispi_ctx = make_unique<ftdispi_context>();

  ftdispi_open        (m_ftdispi_ctx.get(), m_ftdi_ctx.get(), INTERFACE_A); // We use previously generated libftdi config structure, and say we want to use it on INTERFACE_A.
  ftdispi_setmode     (m_ftdispi_ctx.get(), 1, 0, 0, 0, 0, 0);        // CPOL and CPHA are both set to zero.
  ftdispi_setclock    (m_ftdispi_ctx.get(), 200000);                  // Here we request a 200kHz bus speed
  ftdispi_setloopback (m_ftdispi_ctx.get(), 0);
}
//
//  End of: SPI_Protocol::SPI_Protocol
//---------------------------------------------------------------------------




//! Loopbacks "to SUT data" logging SPI command(s) that would have been issued if libFTDIspi was installed.
//!
BinaryVector SPI_Protocol::DoCallback (uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateSPICommand(endpointId, toSutData);

  // ---------------- Split command to be more "compatible" with logger
  //
  auto chunks = Utility::Split(command, "\n");
  for (auto chunk : chunks)
  {
    if (!chunk.empty())
    {
      LOG(INFO) << chunk;
    }
  }

  return toSutData;
}
//
//  End of: SPI_Protocol::DoCallback
//---------------------------------------------------------------------------


//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   Is ignored
//!
void SPI_Protocol::DoReset(bool /* doSynchronousReset */)
{
  LOG(INFO) << "SPI_RESET()";
}


//===========================================================================
// End of SPI_Protocol.cpp
//===========================================================================
