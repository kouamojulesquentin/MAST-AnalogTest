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
I2C_EmulationProtocol::I2C_EmulationProtocol (initializer_list<uint32_t> addresses, string_view commandsPrefix)
  : I2C_Player(addresses, commandsPrefix)
{
}
//
//  End of: I2C_EmulationProtocol::I2C_EmulationProtocol
//---------------------------------------------------------------------------


//! Constructor from vector
//!
//! @param addresses        Array of I2C addresses for managed derivations (value at offset 0 is reserved for reset)
//! @param commandsPrefix   Optional text that will be prepended to actual I2C command
//!
I2C_EmulationProtocol::I2C_EmulationProtocol (vector<uint32_t> addresses, string_view commandsPrefix)
  : I2C_Player(addresses, commandsPrefix)
{
}
//
//  End of: I2C_EmulationProtocol::I2C_EmulationProtocol
//---------------------------------------------------------------------------


//! Loopbacks "to SUT data" logging I2C command(s) that would be issued if it was really an operating protocol
//!
BinaryVector I2C_EmulationProtocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateI2CCommand(derivationId, toSutData);

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
//  End of: Spy_AccessInterfaceProtocols::DoAction
//---------------------------------------------------------------------------


//===========================================================================
// End of I2C_EmulationProtocol.cpp
//===========================================================================
