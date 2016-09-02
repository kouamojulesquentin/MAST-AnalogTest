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
//
//  End of: Spy_AccessInterfaceProtocols::DoAction
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_EmulationProtocol.cpp
//===========================================================================
