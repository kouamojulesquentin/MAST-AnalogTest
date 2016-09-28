//===========================================================================
//                           SVF_EmulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_EmulationProtocol.cpp
//!
//! Implements class SVF_EmulationProtocol
//!
//===========================================================================

#include "SVF_EmulationProtocol.hpp"
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


//! Loopbacks "to SUT data" logging SVF command(s) that would be issued if it was really an operating protocol
//!
BinaryVector SVF_EmulationProtocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateSVFCommand(derivationId, toSutData);

  while (command.back() == '\n')
  {
    command.pop_back();
  }
  LOG(INFO) << command;

  return toSutData;
}


//! Logs SVF commands
//!
//! @param commands New line separated commands
//!
void SVF_EmulationProtocol::LogCommands (string_view commands)
{
  auto splitCommands = Utility::Split(commands, "\n");

  for (auto command : splitCommands)
  {
    LOG(INFO) << command;
  }
}
//
//  End of: SVF_EmulationProtocol::LogCommand
//---------------------------------------------------------------------------



//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void SVF_EmulationProtocol::DoReset(bool doSynchronousReset)
{
  LogCommands(CreateResetSVFCommand(doSynchronousReset));
}

//===========================================================================
// End of SVF_EmulationProtocol.cpp
//===========================================================================
