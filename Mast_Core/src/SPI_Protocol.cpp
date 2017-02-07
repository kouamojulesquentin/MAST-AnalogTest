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

#ifdef USE_LIBFTDISPI
  #include <ftdispi.h>
#endif

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
  #ifdef USE_LIBFTDISPI
  if (m_ftdispi_ctx)
  {
    ftdispi_close(m_ftdispi_ctx.get(), 1);
  }
  #endif
}

//! Initializes ftdi spi library
//!
//! @note AccessInterface derivations ids start from 1 (0 is reserved for reset command)
//!
//! @param chipSelectCommands   SPI chip select addresses for each derivation id
//! @param readCommands         SPI read addresses for each derivation id
//! @param writeCommands        SPI write addresses for each derivation id
//! @param commandsPrefix       Optional text that will be prepended to actual SPI command
//! @param usbDeviceID          Optional (machine specific) USB device identifier
//!
#ifdef USE_LIBFTDISPI
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
#else
SPI_Protocol::SPI_Protocol (vector<uint32_t> chipSelectCommands,
                            vector<uint32_t> readCommands,
                            vector<uint32_t> writeCommands,
                            string_view      commandsPrefix,
                            uint16_t         /* usbDeviceID */)
  : SPI_Player(std::move(chipSelectCommands), std::move(readCommands), std::move(writeCommands), commandsPrefix)
{
  LOG(ERROR_LVL) << "SPI_Protocol is not supported by this built ==> DON'T USE IT";

  // Emulate error we get on platform supporting SPI FTDI when cannot open USB
  // (this is to get similar behaviour in unit tests)
  THROW_RUNTIME_ERROR("OPEN: "s + "usb_find_busses() failed");
}
#endif
//
//  End of: SPI_Protocol::SPI_Protocol
//---------------------------------------------------------------------------




//! Loopbacks "to SUT data" logging SPI command(s) that would have been issued if libFTDIspi was installed.
//!
#ifndef USE_LIBFTDISPI
BinaryVector SPI_Protocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateSPICommand(derivationId, toSutData);

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
#else
BinaryVector SPI_Protocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  // Getting chip-select, and read and write commands for the given derivationId.
  auto chipSelectCommand = GetChipSelectCommand(derivationId);
  auto readCommand       = GetReadCommand(derivationId);
  auto writeCommand      = GetWriteCommand(derivationId);

  auto bitsCount       = toSutData.BitsCount();
  auto bytesCount      = toSutData.BytesCount();
  auto spiBufferLength = bytesCount+1u;                  // +1 byte is needed to host command.
  auto spiBufferRead   = vector<uint8_t>(bytesCount+1u);
  auto spiBufferWrite  = vector<uint8_t>();

  auto toSutDataBuffer    = toSutData.DataRightAligned();
  //auto fromSutDataBuffer  = vector<uint8_t>(toSutData.BytesCount());

  spiBufferRead.insert  (spiBufferRead.begin(),  readCommand);    // Adding the read command at the beginning of the read packet.
  spiBufferWrite.insert (spiBufferWrite.begin(), writeCommand);   // Adding the write command at the beginning of the read packet.
  spiBufferWrite.insert (spiBufferWrite.end(),   toSutDataBuffer.begin(), toSutDataBuffer.end());

  ftdispi_read  (m_ftdispi_ctx.get(), spiBufferRead.data(), spiBufferLength, chipSelectCommand);    // Read request with libftdispi
  LOG(INFO) << "SPI_WRITE(" << toSutData.DataAsMixString() << ")";
  ftdispi_write (m_ftdispi_ctx.get(), spiBufferWrite.data(), spiBufferLength, chipSelectCommand);  // Write request with libftdispi

  vector<uint8_t> fromSutDataBuffer(spiBufferRead.begin()+1, spiBufferRead.end());          // Removing the first byte: dummy value due to the SPI command

  auto fromSutData = BinaryVector::CreateFromRightAlignedBuffer(std::move(fromSutDataBuffer), bitsCount);

  return fromSutData;
}
#endif
//
//  End of: SPI_Protocol::DoAction
//---------------------------------------------------------------------------


//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   Is ignored
//!
void SPI_Protocol::DoReset(bool /* doSynchronousReset */)
{
  #ifndef USE_LIBFTDISPI
  LOG(INFO) << "SPI_RESET()";
  #else
  LOG(WARNING) << "SPI_RESET() ==> Not Yet Implemented";
  #endif
}


//===========================================================================
// End of SPI_Protocol.cpp
//===========================================================================
