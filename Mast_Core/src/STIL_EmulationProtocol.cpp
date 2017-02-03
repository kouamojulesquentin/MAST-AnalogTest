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


//! Initializes a with number of derivations defined by a string
//!
//! @param nbDerivations  Number of derivation defined as C/C++ decimal,
//!                       hexadecimal or octal
//!
//! @note It is useful when parsing SystemModel from text file
//!
STIL_EmulationProtocol::STIL_EmulationProtocol (const string& nbDerivations)
  : STIL_Player(nbDerivations)
{
}
//
//  End of: STIL_EmulationProtocol::STIL_EmulationProtocol
//---------------------------------------------------------------------------




//! Loopbacks "to SUT data" logging STIL command(s) that would be issued if it was really an operating protocol
//!
BinaryVector STIL_EmulationProtocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  for (auto command : CreateSTILCommand(derivationId, toSutData) )
 {
   while (command.back() == '\n')
   {
     command.pop_back();
   }
   LOG(INFO) << command;
  }

  return toSutData;
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
