//===========================================================================
//                           I2C_Player.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file I2C_Player.cpp
//!
//! Implements class I2C_Player
//!
//===========================================================================

#include "I2C_Player.hpp"
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
I2C_Player::I2C_Player (initializer_list<uint32_t> addresses, string_view commandsPrefix)
  : m_commandPrefix (commandsPrefix)
  , m_addresses     (addresses)
{
  if (addresses.size() <= 2)
  {
    THROW_INVALID_ARGUMENT("I2C Addresses must have at least two entries");
  }
}
//
//  End of: I2C_Player::I2C_Player
//---------------------------------------------------------------------------


//! Constructor from vector
//!
//! @param addresses        Array of I2C addresses for managed derivations (value at offset 0 is reserved)
//! @param commandsPrefix   Optional text that will be prepended to actual I2C command
//!
I2C_Player::I2C_Player (vector<uint32_t> addresses, string_view commandsPrefix)
  : m_commandPrefix (commandsPrefix)
  , m_addresses     (std::move(addresses))
{
  if (addresses.size() <= 2)
  {
    THROW_INVALID_ARGUMENT("I2C Addresses must have at least two entries");
  }
}
//
//  End of: I2C_Player::I2C_Player
//---------------------------------------------------------------------------



//! Creates an I2C command associated to derivation identifier and BinaryVector to send to SUT
//!
//! @param addresses        Array of I2C addresses for managed derivations (value at offset 0 is reserved)
//! @param commandsPrefix   Optional text that will be prepended to actual I2C command
//!
string I2C_Player::CreateI2CCommand (uint32_t derivationId, const BinaryVector& toSutData)
{
  ostringstream os;
  string_view commandType;

  if (derivationId == 0)
  {
    os << m_commandPrefix << "I2C_RESET()\n";
  }
  else
  {
    auto address = GetAddress(derivationId);

    os << m_commandPrefix << "I2C_READ(0x"  << std::hex << address << ")\n";
    os << m_commandPrefix << "I2C_WRITE(0x" << std::hex << address << ", " << toSutData.DataAsMixString() << ")\n";
  }

  auto i2cCommand = os.str();

  return i2cCommand;
}
//
//  End of: I2C_Player::CreateI2CCommand
//---------------------------------------------------------------------------


//! Returns address for specified derivation
//!
//! @param derivationId   Derivation identifier [1..N]
//!
uint32_t I2C_Player::GetAddress (uint32_t derivationId) const
{
  if (derivationId >= m_addresses.size())
  {
    THROW_INVALID_ARGUMENT("DerivationId must be '0' for Reset or '1' to "s + std::to_string(m_addresses.size() - 1));
  }

  auto address = m_addresses[derivationId];
  return address;
}
//
//  End of: I2C_Player::GetAddress
//---------------------------------------------------------------------------




//===========================================================================
// End of I2C_Player.cpp
//===========================================================================
