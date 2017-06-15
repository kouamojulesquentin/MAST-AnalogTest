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


//! Saves address values that will further be used to build SPI commands
//!
SPI_Player::SPI_Player(vector<uint32_t> chipSelectCommands,
                       vector<uint32_t> readCommands,
                       vector<uint32_t> writeCommands,
                       string_view      commandsPrefix)
  : m_commandPrefix 			(commandsPrefix)
  , m_chipSelectCommands  (std::move(chipSelectCommands))
  , m_readCommands        (std::move(readCommands))
  , m_writeCommands       (std::move(writeCommands))
{
  CHECK_PARAMETER_GTE(m_chipSelectCommands.size(), 2, "Chips select commands must have at least two elements");
  CHECK_PARAMETER_GTE(m_readCommands.size(),       2, "Read commands must have at least two elements");
  CHECK_PARAMETER_GTE(m_chipSelectCommands.size(), 2, "Write commands must have at least two elements");
}
//
//  End of: SPI_Player::SPI_Player
//---------------------------------------------------------------------------



//! Creates an SPI command associated to endpoint identifier and BinaryVector to send to SUT
//!
//! @param addresses        Array of SPI commands for managed endpoints (value at offset 0 is reserved)
//! @param commandsPrefix   Optional text that will be prepended to actual SPI command
//!
string SPI_Player::CreateSPICommand (uint32_t endpointId, const BinaryVector& toSutData)
{
  ostringstream os;

  if (endpointId == 0)
  {
    os << m_commandPrefix << "SPI_RESET()\n";
  }
  else
  {
    auto chipSelectCommand = GetChipSelectCommand(endpointId);
    auto readCommand       = GetReadCommand(endpointId);
    auto writeCommand      = GetWriteCommand(endpointId);

    os << m_commandPrefix << "SPI_READ(0x"  << std::hex << readCommand  << ", " << std::hex << chipSelectCommand << ")\n";
    os << m_commandPrefix << "SPI_WRITE(0x" << std::hex << writeCommand << ", " << toSutData.DataAsMixString() << ", " << std::hex << chipSelectCommand << ")\n";
  }

  auto SPICommand = os.str();

  return SPICommand;
}
//
//  End of: SPI_Player::CreateSPICommand
//---------------------------------------------------------------------------

//! Returns Chip Select command for a specified endpoint
//!
//! @param endpointId   EndPoint identifier [1..N]
//!
uint32_t SPI_Player::GetChipSelectCommand (uint32_t endpointId) const
{
  if (endpointId >= m_chipSelectCommands.size())
  {
    THROW_INVALID_ARGUMENT("EndPointId must be '0' for Reset or '1' to "s + std::to_string(m_chipSelectCommands.size() - 1));
  }

  auto command = m_chipSelectCommands[endpointId];
  return command;
}


//! Returns read command for a specified endpoint
//!
//! @param endpointId   EndPoint identifier [1..N]
//!
uint32_t SPI_Player::GetReadCommand (uint32_t endpointId) const
{
  if (endpointId >= m_readCommands.size())
  {
    THROW_INVALID_ARGUMENT("EndPointId must be '0' for Reset or '1' to "s + std::to_string(m_readCommands.size() - 1));
  }

  auto command = m_readCommands[endpointId];
  return command;
}
//
//  End of: SPI_Player::GetReadCommand
//---------------------------------------------------------------------------


//! Returns write command for a specified endpoint
//!
//! @param endpointId   EndPoint identifier [1..N]
//!
uint32_t SPI_Player::GetWriteCommand (uint32_t endpointId) const
{
  if (endpointId >= m_writeCommands.size())
  {
    THROW_INVALID_ARGUMENT("EndPointId must be '0' for Reset or '1' to "s + std::to_string(m_writeCommands.size() - 1));
  }

  auto command = m_writeCommands[endpointId];
  return command;
}
//
//  End of: SPI_Player::GetWriteCommand
//---------------------------------------------------------------------------


//===========================================================================
// End of SPI_Player.cpp
//===========================================================================
