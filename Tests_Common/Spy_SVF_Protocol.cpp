//===========================================================================
//                           Spy_SVF_Protocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_SVF_Protocol.cpp
//!
//! Implements class Spy_SVF_Protocol
//!
//===========================================================================

#include "Spy_SVF_Protocol.hpp"
#include "Utility.hpp"

using std::experimental::string_view;
using std::string;
using namespace mast;
using namespace test;


//! Spies content how binary vector to SUT is transformed to SVF command while returning the BinaryVector unchanged
//!
BinaryVector Spy_SVF_Protocol::DoCallback (std::string /*CallbackId*/, uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateSVFCommand(endpointId, toSutData);

  SaveCommands(command);

  return toSutData;
}
//
//  End of: Spy_SVF_Protocol::DoCallback
//---------------------------------------------------------------------------


//! Saves SVF commands
//!
//! @param commands New line separated commands
//!
void Spy_SVF_Protocol::SaveCommands (string_view commands)
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
//  End of: Spy_SVF_Protocol::SaveCommands
//---------------------------------------------------------------------------


//! Spies Reset command
//!
void Spy_SVF_Protocol::DoReset (bool doSynchronousReset)
{
  auto commands = CreateResetSVFCommand(doSynchronousReset);

  SaveCommands(commands);
}
//
//  End of: Spy_AccessInterfaceProtocols::Reset
//---------------------------------------------------------------------------



//===========================================================================
// End of Spy_SVF_Protocol.cpp
//===========================================================================
