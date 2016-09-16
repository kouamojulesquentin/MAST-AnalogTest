//===========================================================================
//                           SPI_Player.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SPI_Player.cpp
//!
//! Implements class SPI_Player
//!
//===========================================================================

#include "SPI_Player.hpp"
#include "Utility.hpp"

#include <sstream>

using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::vector;
using std::initializer_list;

using namespace mast;
using namespace std::string_literals;

//! Constructor from initializer_list
//!
SPI_Player::SPI_Player(std::initializer_list<uint32_t> chipSelectCommands, std::initializer_list<uint32_t> readCommands, std::initializer_list<uint32_t> writeCommands, std::experimental::string_view commandsPrefix)
  : m_commandPrefix 			(commandsPrefix)
	, m_chipSelectCommands 	(chipSelectCommands)
  , m_readCommands  			(readCommands)
  , m_writeCommands 			(writeCommands)
{
  if ((chipSelectCommands.size() < 2)
			|| (readCommands.size() < 2)
      || (writeCommands.size() < 2))
  {
    THROW_INVALID_ARGUMENT("Chip select, Read and write SPI commands must have at least two entries each");
  }
}
//
//  End of: SPI_Player::SPI_Player
//---------------------------------------------------------------------------


SPI_Player::SPI_Player(std::vector<uint32_t> chipSelectCommands, std::vector<uint32_t> readCommands, std::vector<uint32_t>           writeCommands, std::experimental::string_view commandsPrefix)
  : m_commandPrefix 			(commandsPrefix)
	, m_chipSelectCommands 	(std::move(chipSelectCommands))
  , m_readCommands  			(std::move(readCommands))
  , m_writeCommands 			(std::move(writeCommands))
{
  if ((chipSelectCommands.size() < 2)
			|| (readCommands.size() < 2)
      || (writeCommands.size() < 2))
  {
    THROW_INVALID_ARGUMENT("Chip select, Read and write SPI commands must have at least two entries each");
  }
}
//
//  End of: SPI_Player::SPI_Player
//---------------------------------------------------------------------------



//! Creates an SPI command associated to derivation identifier and BinaryVector to send to SUT
//!
//! @param addresses        Array of SPI commands for managed derivations (value at offset 0 is reserved)
//! @param commandsPrefix   Optional text that will be prepended to actual SPI command
//!
string SPI_Player::CreateSPICommand (uint32_t derivationId, const BinaryVector& toSutData)
{
  ostringstream os;
  string_view commandType;

  if (derivationId == 0)
  {
    os << m_commandPrefix << "SPI_RESET()\n";
  }
  else
  {
		auto chipSelectCommand = GetChipSelectCommand(derivationId);
    auto readCommand = GetReadCommand(derivationId);
    auto writeCommand = GetWriteCommand(derivationId);

    os << m_commandPrefix << "SPI_READ(0x"  << std::hex << readCommand << ", " << std::hex << chipSelectCommand << ")\n";
    os << m_commandPrefix << "SPI_WRITE(0x" << std::hex << writeCommand << ", " << toSutData.DataAsMixString() << ", " << std::hex << chipSelectCommand << ")\n";
  }

  auto SPICommand = os.str();

  return SPICommand;
}
//
//  End of: SPI_Player::CreateSPICommand
//---------------------------------------------------------------------------

//! Returns Chip Select command for a specified derivation
//!
//! @param derivationId   Derivation identifier [1..N]
//!
uint32_t SPI_Player::GetChipSelectCommand (uint32_t derivationId) const
{
  if (derivationId >= m_chipSelectCommands.size())
  {
    THROW_INVALID_ARGUMENT("DerivationId must be '0' for Reset or '1' to "s + std::to_string(m_chipSelectCommands.size() - 1));
  }

  auto command = m_chipSelectCommands[derivationId];
  return command;
}


//! Returns read command for a specified derivation
//!
//! @param derivationId   Derivation identifier [1..N]
//!
uint32_t SPI_Player::GetReadCommand (uint32_t derivationId) const
{
  if (derivationId >= m_readCommands.size())
  {
    THROW_INVALID_ARGUMENT("DerivationId must be '0' for Reset or '1' to "s + std::to_string(m_readCommands.size() - 1));
  }

  auto command = m_readCommands[derivationId];
  return command;
}
//
//  End of: SPI_Player::GetReadCommand
//---------------------------------------------------------------------------


//! Returns write command for a specified derivation
//!
//! @param derivationId   Derivation identifier [1..N]
//!
uint32_t SPI_Player::GetWriteCommand (uint32_t derivationId) const
{
  if (derivationId >= m_writeCommands.size())
  {
    THROW_INVALID_ARGUMENT("DerivationId must be '0' for Reset or '1' to "s + std::to_string(m_writeCommands.size() - 1));
  }

  auto command = m_writeCommands[derivationId];
  return command;
}
//
//  End of: SPI_Player::GetWriteCommand
//---------------------------------------------------------------------------


//===========================================================================
// End of SPI_Player.cpp
//===========================================================================
