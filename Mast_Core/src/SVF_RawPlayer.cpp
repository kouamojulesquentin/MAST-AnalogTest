//===========================================================================
//                           SVF_RawPlayer.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_RawPlayer.cpp
//!
//! Implements class SVF_RawPlayer
//!
//===========================================================================

#include "SVF_RawPlayer.hpp"
#include "SVFVector.hpp"
#include "Utility.hpp"

#include <experimental/string_view>
#include <sstream>

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;


//! Creates an SVF command associated to endpoint identifier and BinaryVector to send to SUT
//!
string SVF_RawPlayer::CreateSVFCommand (uint32_t endpointId, const BinaryVector& toSutData) const
{
  string_view commandType;

  switch (endpointId)
  {
    case 0u:
      return CreateResetSVFCommand(false);
    case 1u:
      commandType = "SIR";
      break;
    case 2u:
      commandType = "SDR";
      break;
    default:
      THROW_INVALID_ARGUMENT("EndPointId must be '0' (for Reset), '1' (for SIR) or '2' (for SDR)");
      break;
  }

  ostringstream os;
  os << commandType << " " << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");\n";

  auto svfCommand = os.str();

  return svfCommand;
}
//
//  End of: SVF_RawPlayer::CreateSVFCommand
//---------------------------------------------------------------------------


//! Creates an SVF reset command
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
string SVF_RawPlayer::CreateResetSVFCommand (bool doSynchronousReset) const
{
  ostringstream os;

  if (SupportTRST() && !doSynchronousReset)
  {
    os << "TRST ON;\nTRST OFF;\n";
  }
  else
  {
    os << "STATE RESET;\n";
  }

  auto svfCommand = os.str();

  return svfCommand;
}

//! Loopbacks "to SUT data" logging SVF command(s) that would be issued if it was really an operating protocol
//!
BinaryVector SVF_RawPlayer::DoCallback (uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateSVFCommand(endpointId, toSutData);

  while (command.back() == '\n')
  {
    command.pop_back();
  }
//  LOG(INFO) << command;

  return toSutData;
}

//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void SVF_RawPlayer::DoReset(bool doSynchronousReset)
{
 // LogCommands(CreateResetSVFCommand(doSynchronousReset));
}

//
//  End of: SVF_RawPlayer::CreateSVFCommand
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_RawPlayer.cpp
//===========================================================================
