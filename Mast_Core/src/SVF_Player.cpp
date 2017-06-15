//===========================================================================
//                           SVF_Player.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_Player.cpp
//!
//! Implements class SVF_Player
//!
//===========================================================================

#include "SVF_Player.hpp"
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
string SVF_Player::CreateSVFCommand (uint32_t endpointId, const BinaryVector& toSutData) const
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
//  End of: SVF_Player::CreateSVFCommand
//---------------------------------------------------------------------------


//! Creates an SVF reset command
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
string SVF_Player::CreateResetSVFCommand (bool doSynchronousReset) const
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
//
//  End of: SVF_Player::CreateSVFCommand
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_Player.cpp
//===========================================================================
