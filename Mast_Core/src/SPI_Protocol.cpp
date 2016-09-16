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



//! Constructor from initializer_list
//!
SPI_Protocol::SPI_Protocol(std::initializer_list<uint32_t> chipSelectCommands, std::initializer_list<uint32_t> readCommands, std::initializer_list<uint32_t> writeCommands, std::experimental::string_view commandsPrefix)
  : SPI_Player(chipSelectCommands, readCommands, writeCommands, commandsPrefix)
{
	#ifdef USE_LIBFTDISPI
  m_ftdi_ctx = static_cast<ftdi_context*>(malloc(sizeof(*m_ftdi_ctx)));
  if (ftdi_init(m_ftdi_ctx) < 0) {
	fprintf(stderr, "ftdi_init failed\n");
  }

  int ret = ftdi_usb_open(m_ftdi_ctx, 0x0403, 0x6010);

  if (ret < 0 && ret != -5) {
	fprintf(stderr, "OPEN: %s\n", ftdi_get_error_string(m_ftdi_ctx));
		exit(-1);
	}

  m_ftdispi_ctx = static_cast<ftdispi_context*>(malloc(sizeof(*m_ftdispi_ctx)));
	ftdispi_open(m_ftdispi_ctx, m_ftdi_ctx, INTERFACE_A);
	ftdispi_setmode(m_ftdispi_ctx, 1, 0, 0, 0, 0, 0); // CPOL and CPHA are both set to zero.
	ftdispi_setclock(m_ftdispi_ctx, 200000);					 // Here we request a 200kHz bus speed
	ftdispi_setloopback(m_ftdispi_ctx, 0);

	#endif

}
//
//  End of: SPI_Protocol::SPI_Protocol
//---------------------------------------------------------------------------


//! Constructor from vector
//!
//! @param addresses        Array of SPI addresses for managed derivations (value at offset 0 is reserved for reset)
//! @param commandsPrefix   Optional text that will be prepended to actual SPI command
//!
SPI_Protocol::SPI_Protocol (std::vector<uint32_t>           chipSelectCommands, std::vector<uint32_t>           readCommands, std::vector<uint32_t>           writeCommands, std::experimental::string_view commandsPrefix)
  : SPI_Player(chipSelectCommands, readCommands, writeCommands, commandsPrefix)
{
	#ifdef USE_LIBFTDISPI
  m_ftdi_ctx = static_cast<ftdi_context*>(malloc(sizeof(*m_ftdi_ctx)));
  if (ftdi_init(m_ftdi_ctx) < 0) {
	fprintf(stderr, "ftdi_init failed\n");
  }

  int ret = ftdi_usb_open(m_ftdi_ctx, 0x0403, 0x6011);

  if (ret < 0 && ret != -5) {
	fprintf(stderr, "OPEN: %s\n", ftdi_get_error_string(m_ftdi_ctx));
		exit(-1);
	}

  m_ftdispi_ctx = static_cast<ftdispi_context*>(malloc(sizeof(*m_ftdispi_ctx)));
	ftdispi_open(m_ftdispi_ctx, m_ftdi_ctx, INTERFACE_A);
	ftdispi_setmode(m_ftdispi_ctx, 1, 0, 0, 0, 0, 0); // CPOL and CPHA are both set to zero.
	ftdispi_setclock(m_ftdispi_ctx, 200000);					 // Here we request a 200kHz bus speed
	ftdispi_setloopback(m_ftdispi_ctx, 0);

	#endif
}
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

#ifndef USE_LIBFTDISPI

//! Loopbacks "to SUT data" logging SPI command(s) that would have been issued if libFTDIspi was installed.
//!
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
	auto chipSelectCommand = GetChipSelectCommand(derivationId);
	auto readCommand 	= GetReadCommand(derivationId);
  auto writeCommand = GetWriteCommand(derivationId);

  auto bitsCount	 		= toSutData.BitsCount();
  auto bytesCount 		= toSutData.BytesCount();
	auto spiBufferLength = bytesCount+1u;									 // +1 byte is needed to host command.
	auto spiBufferRead 	= vector<uint8_t>(bytesCount+1u);
	auto spiBufferWrite	= vector<uint8_t>();

  auto toSutDataBuffer		=	toSutData.DataRightAligned();
	//auto fromSutDataBuffer	= vector<uint8_t>(toSutData.BytesCount());

  spiBufferRead[0] 		= readCommand;

  spiBufferWrite[0] 	= writeCommand;

  spiBufferWrite.insert(spiBufferWrite.end(), toSutDataBuffer.begin(), toSutDataBuffer.end());

  ftdispi_read(m_ftdispi_ctx, spiBufferRead.data(), spiBufferLength, chipSelectCommand);
  ftdispi_write(m_ftdispi_ctx, spiBufferWrite.data(), spiBufferLength, chipSelectCommand);
  
	vector<uint8_t> fromSutDataBuffer(&spiBufferRead[1], &spiBufferRead[bytesCount]);

  auto fromSutData = BinaryVector::CreateFromRightAlignedBuffer(std::move(fromSutDataBuffer), bitsCount);

  return fromSutData;
}
#endif
//
//  End of: Spy_AccessInterfaceProtocols::DoAction
//---------------------------------------------------------------------------


//===========================================================================
// End of SPI_Protocol.cpp
//===========================================================================
