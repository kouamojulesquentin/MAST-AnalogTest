//===========================================================================
//                           Spy_STIL_Protocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_STIL_Protocol.cpp
//!
//! Implements class Spy_STIL_Protocol
//!
//===========================================================================

#include "Spy_STIL_Protocol.hpp"
#include "Utility.hpp"

using std::experimental::string_view;
using std::string;
using namespace mast;
using namespace test;


//! Spies content how binary vector to SUT is transformed to STIL command while returning the BinaryVector unchanged
//!
BinaryVector Spy_STIL_Protocol::DoCallback (RVFRequest Request, uint32_t channelId)
{
  for (auto command : CreateSTILCommand(channelId, Request.ToSutVector()) )
   {
   SaveCommands(command);
   }

  return Request.ToSutVector();
}
//
//  End of: Spy_STIL_Protocol::DoCallback
//---------------------------------------------------------------------------


//! Saves STIL commands
//!
//! @param commands New line separated commands
//!
void Spy_STIL_Protocol::SaveCommands (string_view commands)
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
//! Resets STIL commands list
//!
//! @param commands New line separated commands
//!
void Spy_STIL_Protocol::resetCommands ()
{
 m_spiedCommands = std::make_shared<SpiedProtocolsCommands>();
}
//
//  End of: Spy_STIL_Protocol::SaveCommands
//---------------------------------------------------------------------------


//! Spies Reset command
//!
void Spy_STIL_Protocol::DoReset (bool doSynchronousReset)
{
  auto commands = CreateResetSTILCommand(doSynchronousReset);

  SaveCommands(commands);
}
//
//  End of: Spy_AccessInterfaceProtocols::Reset
//---------------------------------------------------------------------------



//===========================================================================
// End of Spy_STIL_Protocol.cpp
//===========================================================================
