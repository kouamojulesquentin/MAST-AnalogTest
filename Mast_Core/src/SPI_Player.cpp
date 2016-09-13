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
SPI_Player::SPI_Player(std::initializer_list<uint32_t> readCommands, std::initializer_list<uint32_t> writeCommands, std::experimental::string_view commandsPrefix)
  : m_commandPrefix (commandsPrefix)
  , m_readCommands  (readCommands)
  , m_writeCommands (writeCommands)
{
  if ((readCommands.size() <= 2)
      || (writeCommands.size() <= 2))
  {
    THROW_INVALID_ARGUMENT("Read and write SPI commands must have at least two entries each");
  }
}
//
//  End of: SPI_Player::SPI_Player
//---------------------------------------------------------------------------


SPI_Player::SPI_Player(std::vector<uint32_t> readCommands, std::vector<uint32_t>           writeCommands, std::experimental::string_view commandsPrefix)
  : m_commandPrefix (commandsPrefix)
  , m_readCommands  (std::move(readCommands))
  , m_writeCommands (std::move(writeCommands))
{
  if ((readCommands.size() <= 2)
      || (writeCommands.size() <= 2))
  {
    THROW_INVALID_ARGUMENT("Read and write SPI commands must have at least two entries each");
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
    auto readCommand = GetReadCommand(derivationId);
    auto writeCommand = GetWriteCommand(derivationId);

    os << m_commandPrefix << "SPI_READ(0x"  << std::hex << readCommand << ")\n";
    os << m_commandPrefix << "SPI_WRITE(0x" << std::hex << writeCommand << ", " << toSutData.DataAsMixString() << ")\n";
  }

  auto SPICommand = os.str();

  return SPICommand;
}
//
//  End of: SPI_Player::CreateSPICommand
//---------------------------------------------------------------------------


//! Returns read command for specified derivation
//!
//! @param derivationId   Derivation identifier [1..N]
//!
uint32_t SPI_Player::GetReadCommand (uint32_t derivationId) const
{
  if (derivationId >= m_readCommands.size())
  {
    THROW_INVALID_ARGUMENT("DerivationId must be '0' for Reset or '1' to "s + std::to_string(m_readCommands.size() - 1));
  }

  auto address = m_readCommands[derivationId];
  return address;
}
//
//  End of: SPI_Player::GetReadCommand
//---------------------------------------------------------------------------


//! Returns write command for specified derivation
//!
//! @param derivationId   Derivation identifier [1..N]
//!
uint32_t SPI_Player::GetWriteCommand (uint32_t derivationId) const
{
  if (derivationId >= m_writeCommands.size())
  {
    THROW_INVALID_ARGUMENT("DerivationId must be '0' for Reset or '1' to "s + std::to_string(m_writeCommands.size() - 1));
  }

  auto address = m_writeCommands[derivationId];
  return address;
}
//
//  End of: SPI_Player::GetWriteCommand
//---------------------------------------------------------------------------


//===========================================================================
// End of SPI_Player.cpp
//===========================================================================
