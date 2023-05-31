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
#include "g3log/g3log.hpp"

#include <sstream>

using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::vector;
using std::initializer_list;

using namespace mast;
using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;


//! Constructor from vector
//!
//! @param addresses        Array of I2C addresses for managed channels (value at offset 0 is reserved for reset)
//! @param commandsPrefix   Optional text that will be prepended to actual I2C command
//!
I2C_Player::I2C_Player (vector<uint32_t> addresses, string commandsPrefix)
  : m_commandPrefix (std::move(commandsPrefix))
  , m_addresses     (std::move(addresses))
{
  CHECK_PARAMETER_GTE(m_addresses.size(), 2u, "I2C Addresses must have at least two entries");
}
//
//  End of: I2C_Player::I2C_Player
//---------------------------------------------------------------------------


//! Initializes with addresses and optional prefix defined by a string
//!
//! @note Expected (comma separated) parameters are:
//!   - addresses        Array of I2C addresses for managed channels (value at offset 0 is reserved for reset)
//!   - commandsPrefix   Optional text that will be prepended to actual I2C command ; MUST NOT BEEN A VALID NUMBER!
//!
//! @note Example of parameter:
//!    0x30, 0x31, 0x32
//!    0x30u, 0x31u
//!    48, 49, 0x32, [I2C]
//!
I2C_Player::I2C_Player (const std::string& parameters)
{
  auto parts = Utility::Split(parameters, ",");

  CHECK_PARAMETER_GTE(parts.size(), 2u, "Parameters must defined at least two I2C Addresses, got: \""sv + parameters + "\""sv);

  auto afterAddressIndex = 0u;  // This is to be able to know if there is a valid command prefix

  auto makeParameterMessage = [&parts](auto partIndex)
  {
    auto part = parts[partIndex];
    Utility::TrimBoth(part);

    string message;
    message.append("Parameter: ").append(std::to_string(partIndex));
    message.append(" \"").append(part.cbegin(), part.cend()).append("\"");
    return message;
  };

  // ---------------- Get Adresses
  //
  try
  {
    for (auto part : parts)
    {
      auto number = std::stoull(string(part), nullptr, 0);
      CHECK_PARAMETER_LTE(number, UINT32_MAX, makeParameterMessage(afterAddressIndex).append(", is out of range for a 32 bit number"));

      m_addresses.push_back(static_cast<uint32_t>(number));
      ++afterAddressIndex;
    }
  }
  catch(std::invalid_argument& exc) // Conversion to number is not possible ==> it must be the prefix
  {
  }
  catch(std::out_of_range& exc)     // Conversion to number is not possible ==> it must be the prefix
  {
    THROW_INVALID_ARGUMENT(makeParameterMessage(afterAddressIndex).append(", is out of range for a 32 bit number"));
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    LOG(ERROR_LVL) << "Got std::exception: " << exc.what();
    throw;
  }
  catch (...)
  {
    LOG(ERROR_LVL) << "Caught unexpected, unknown exception while parsing parameters for I2C_Player";
    throw;   // Unexpected exception are rethrown
  }

  CHECK_PARAMETER_GTE(m_addresses.size(), 2u, "I2C Addresses must have at least two entries");

  if (afterAddressIndex == (parts.size() - 1u))
  {
    m_commandPrefix = string(parts[afterAddressIndex]);
  }
  else if (parts.size() > afterAddressIndex)
  {
    THROW_INVALID_ARGUMENT(makeParameterMessage(afterAddressIndex).append(", is not a number (for address) or not the last one (for a prefix)"));
  }
}
//
//  End of: I2C_Player::I2C_Player
//---------------------------------------------------------------------------




//! Creates an I2C command associated to channel identifier and BinaryVector to send to SUT
//!
//! @param addresses        Array of I2C addresses for managed channels (value at offset 0 is reserved)
//! @param commandsPrefix   Optional text that will be prepended to actual I2C command
//!
string I2C_Player::CreateI2CCommand (uint32_t channelId, const BinaryVector& toSutData)
{
  ostringstream os;
  string_view commandType;

  if (channelId == 0)
  {
    os << m_commandPrefix << "I2C_RESET()\n";
  }
  else
  {
    auto address = GetAddress(channelId);

    os << m_commandPrefix << "I2C_READ(0x"  << std::hex << address << ")\n";
    os << m_commandPrefix << "I2C_WRITE(0x" << std::hex << address << ", " << toSutData.DataAsMixString() << ")\n";
  }

  auto i2cCommand = os.str();

  return i2cCommand;
}
//
//  End of: I2C_Player::CreateI2CCommand
//---------------------------------------------------------------------------


//! Returns address for specified channel
//!
//! @param channelId   Channel identifier [1..N]
//!
uint32_t I2C_Player::GetAddress (uint32_t channelId) const
{
  if (channelId >= m_addresses.size())
  {
    THROW_INVALID_ARGUMENT("ChannelId must be '0' for Reset or '1' to "s + std::to_string(m_addresses.size() - 1));
  }

  auto address = m_addresses[channelId];
  return address;
}
//
//  End of: I2C_Player::GetAddress
//---------------------------------------------------------------------------




//===========================================================================
// End of I2C_Player.cpp
//===========================================================================
