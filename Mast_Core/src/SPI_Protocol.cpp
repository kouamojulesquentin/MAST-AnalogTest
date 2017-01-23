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
using namespace mast;
using namespace std::string_literals;


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
  : SPI_Player(std::move(chipSelectCommands),
               std::move(readCommands),
               std::move(writeCommands),
               commandsPrefix)
{

	// Here we initialize libftdi-related structures (used later by libftdispi)
  m_ftdi_ctx = static_cast<ftdi_context*>(malloc(sizeof(*m_ftdi_ctx)));
  if (ftdi_init(m_ftdi_ctx) < 0) {
	fprintf(stderr, "ftdi_init failed\n");
  }
  
  /*Trying to open channel B, reverting to ANY if error*/
  if (ftdi_set_interface(m_ftdi_ctx,INTERFACE_B)>0)
    {
     fprintf(stdout, "ftdi open on Channel B\n");
    }
    else
     {
     ftdi_set_interface(m_ftdi_ctx,INTERFACE_ANY);
     fprintf(stdout, "ftdi open on Channel A\n");
     }
  
  int ret = ftdi_usb_open(m_ftdi_ctx, 0x0403, usbDeviceID);

  if (ret < 0 && ret != -5) {
	fprintf(stderr, "OPEN: %s\n", ftdi_get_error_string(m_ftdi_ctx));
		exit(-1);
	}

	// Now we initialize libftdispi configuration structures
  m_ftdispi_ctx = static_cast<ftdispi_context*>(malloc(sizeof(*m_ftdispi_ctx)));
	ftdispi_open(m_ftdispi_ctx, m_ftdi_ctx, INTERFACE_A);		// We use previously generated libftdi config structure, and say we want to use it on INTERFACE_A.
	ftdispi_setmode(m_ftdispi_ctx, 1, 0, 0, 0, 0, 0); // CPOL and CPHA are both set to zero.
	ftdispi_setclock(m_ftdispi_ctx, 200000);					 // Here we request a 200kHz bus speed
	ftdispi_setloopback(m_ftdispi_ctx, 0);
}
#else
SPI_Protocol::SPI_Protocol (vector<uint32_t> chipSelectCommands,
                            vector<uint32_t> readCommands,
                            vector<uint32_t> writeCommands,
                            string_view      commandsPrefix,
                            uint16_t         /* usbDeviceID */)
  : SPI_Player(std::move(chipSelectCommands),
               std::move(readCommands),
               std::move(writeCommands),
               commandsPrefix)
{
}
#endif
//
//  End of: SPI_Protocol::SPI_Protocol
//---------------------------------------------------------------------------

SPI_Protocol::~SPI_Protocol()
{
	#ifdef USE_LIBFTDISPI
	ftdispi_close(m_ftdispi_ctx, 1);
  free(m_ftdispi_ctx);
	free(m_ftdi_ctx);
	#endif
}


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

  auto toSutDataBuffer		=	toSutData.DataRightAligned();
	//auto fromSutDataBuffer	= vector<uint8_t>(toSutData.BytesCount());

  spiBufferRead.insert  (spiBufferRead.begin(),  readCommand);		// Adding the read command at the beginning of the read packet.
  spiBufferWrite.insert (spiBufferWrite.begin(), writeCommand);		// Adding the write command at the beginning of the read packet.
  spiBufferWrite.insert (spiBufferWrite.end(),   toSutDataBuffer.begin(), toSutDataBuffer.end());

  ftdispi_read(m_ftdispi_ctx, spiBufferRead.data(), spiBufferLength, chipSelectCommand);		// Read request with libftdispi
	LOG(INFO) << "SPI_WRITE(" << toSutData.DataAsMixString() << ")";
  ftdispi_write(m_ftdispi_ctx, spiBufferWrite.data(), spiBufferLength, chipSelectCommand);  // Write request with libftdispi

	vector<uint8_t> fromSutDataBuffer(spiBufferRead.begin()+1, spiBufferRead.end());					// Removing the first byte: dummy value due to the SPI command

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
