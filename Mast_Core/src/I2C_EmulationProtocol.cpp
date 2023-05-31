//===========================================================================
//                           I2C_EmulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file I2C_EmulationProtocol.cpp
//!
//! Implements class I2C_EmulationProtocol
//!
//===========================================================================

#include "I2C_EmulationProtocol.hpp"
#include "g3log/g3log.hpp"

#include "Utility.hpp"
#include <thread>
#include <chrono>

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
I2C_EmulationProtocol::I2C_EmulationProtocol (initializer_list<uint32_t> addresses, string commandsPrefix)
  : I2C_Player(addresses, std::move(commandsPrefix))
{
}
//
//  End of: I2C_EmulationProtocol::I2C_EmulationProtocol
//---------------------------------------------------------------------------


//! Constructor from vector
//!
//! @param addresses        Array of I2C addresses for managed channels (value at offset 0 is reserved for reset)
//! @param commandsPrefix   Optional text that will be prepended to actual I2C command
//!
I2C_EmulationProtocol::I2C_EmulationProtocol (vector<uint32_t> addresses, string commandsPrefix)
  : I2C_Player(addresses, std::move(commandsPrefix))
{
}


//! Initializes with addresses and optional prefix defined by a string
//!
//! @note Expected (comma separated) parameters are:
//!   - addresses        Array of I2C addresses for managed channels (value at offset 0 is reserved for reset)
//!   - commandsPrefix   Optional text that will be prepended to actual I2C command
//!
//! @see I2C_Player
//!
I2C_EmulationProtocol::I2C_EmulationProtocol (const string& parameters)
  : I2C_Player(parameters)
{
}
//
//  End of: I2C_EmulationProtocol::I2C_EmulationProtocol
//---------------------------------------------------------------------------




//! Loopbacks "to SUT data" logging I2C command(s) that would be issued if it was really an operating protocol
//!
BinaryVector I2C_EmulationProtocol::DoCallback (uint32_t channelId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateI2CCommand(channelId, toSutData);

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


//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void I2C_EmulationProtocol::DoReset(bool doSynchronousReset)
{
  LOG(INFO) << "I2C_RESET(" << std::boolalpha << doSynchronousReset << ")";
}

//===========================================================================
// End of I2C_EmulationProtocol.cpp
//===========================================================================
