//===========================================================================
//                           STIL_EmulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file STIL_EmulationProtocol.cpp
//!
//! Implements class STIL_EmulationProtocol
//!
//===========================================================================

#include "STIL_EmulationProtocol.hpp"
#include "g3log/g3log.hpp"

#include "Utility.hpp"
#include <thread>
#include <chrono>

using std::experimental::string_view;
using std::string;
using std::ofstream;
using std::ifstream;
using namespace mast;
using namespace std::string_literals;

STIL_EmulationProtocol::STIL_EmulationProtocol(uint32_t n_chains)
  : STIL_Player(n_chains)
{
}


//! Initializes a with number of Channels defined by a string
//!
//! @param nbChannels  Number of Channels defined as C/C++ decimal,
//!                       hexadecimal or octal
//!
//! @note It is useful when parsing SystemModel from text file
//!
STIL_EmulationProtocol::STIL_EmulationProtocol (const string& nbChannels)
  : STIL_Player(nbChannels)
{
}
//
//  End of: STIL_EmulationProtocol::STIL_EmulationProtocol
//---------------------------------------------------------------------------




//! Loopbacks "to SUT data" logging STIL command(s) that would be issued if it was really an operating protocol
//!
BinaryVector STIL_EmulationProtocol::DoCallback (RVFRequest Request, uint32_t channelId)
{
  for (auto command : CreateSTILCommand(channelId, Request.ToSutVector()) )
 {
   while (command.back() == '\n')
   {
     command.pop_back();
   }
   LOG(INFO) << command;
  }

  return Request.ToSutVector();
}


//! Logs STIL commands
//!
//! @param commands New line separated commands
//!
void STIL_EmulationProtocol::LogCommands (string_view commands)
{
  auto splitCommands = Utility::Split(commands, "\n");

  for (auto command : splitCommands)
  {
    LOG(INFO) << command;
  }
}
//
//  End of: STIL_EmulationProtocol::LogCommand
//---------------------------------------------------------------------------



//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void STIL_EmulationProtocol::DoReset(bool doSynchronousReset)
{
  LogCommands(CreateResetSTILCommand(doSynchronousReset));
}

//===========================================================================
// End of STIL_EmulationProtocol.cpp
//===========================================================================
