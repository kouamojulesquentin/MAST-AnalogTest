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

using namespace mast;
using namespace test;


//! Spies content how binary vector to SUT is transformed to I2C command while returning the BinaryVector unchanged
//!
BinaryVector Spy_I2C_Protocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateI2CCommand(derivationId, toSutData);
  m_commands.emplace_back(command);
  return toSutData;
}
//
//  End of: Spy_I2C_Protocol::DoAction
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
