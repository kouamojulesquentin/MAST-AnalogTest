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
#include "CallbackRequest.hpp"

#include <experimental/string_view>
#include <sstream>

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;


//  os << commandType << " " << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");\n";

//! Loopbacks "to SUT data" logging SVF command(s) that would be issued if it was really an operating protocol
//!
BinaryVector SVF_RawPlayer::DoCallback (uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  
  auto request = new CallbackRequest(CallbackId(endpointId),toSutData);
  ParentInterface()->PushRequest(*request);
  
  /*NB: this is a BLOCKING call*/
  auto result = ParentInterface()->PopResult();
  
  return result;
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
