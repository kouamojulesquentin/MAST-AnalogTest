//===========================================================================
//                           Spy_I2C_Protocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_I2C_Protocol.cpp
//!
//! Implements class Spy_I2C_Protocol
//!
//===========================================================================

#include "Spy_I2C_Protocol.hpp"
#include "Utility.hpp"

#include <string>
#include <experimental/string_view>

using std::string;
using std::experimental::string_view;

using namespace mast;
using namespace test;


//! Spies content how binary vector to SUT is transformed to I2C command while returning the BinaryVector unchanged
//!
BinaryVector Spy_I2C_Protocol::DoCallback (std::string /*CallbackId*/, uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto commands = CreateI2CCommand(endpointId, toSutData);
  SaveCommands(commands);
  return toSutData;
}
//
//  End of: Spy_I2C_Protocol::DoCallback
//---------------------------------------------------------------------------


//! Saves I2C commands
//!
//! @param commands New line separated commands
//!
void Spy_I2C_Protocol::SaveCommands (string_view commands)
{
  auto splitCommands = Utility::Split(commands, "\n");

  for (auto command : splitCommands)
  {
    if (!command.empty())
    {
      m_spiedCommands->SaveCommand(string(command));
    }
  }
}
//
//  End of: Spy_I2C_Protocol::SaveCommands
//---------------------------------------------------------------------------



//! Tally number of call to Reset
//!
void Spy_I2C_Protocol::DoReset (bool /* doSynchronousReset */)
{
  ++m_resetCount;
}
//
//  End of: Spy_AccessInterfaceProtocols::Reset
//---------------------------------------------------------------------------


//===========================================================================
// End of Spy_I2C_Protocol.cpp
//===========================================================================
