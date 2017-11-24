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
//! NOT VERIFIED YET!!! CHECK!!!!!!!

BinaryVector SVF_RawPlayer::DoCallback (uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  BinaryVector result;
  
  //Prepapre formatted SVF data
  ostringstream os;
  os << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");\n";
  auto svfFormattedData = os.str();

  auto request = new CallbackRequest(CallbackId(endpointId),toSutData,svfFormattedData);
  ParentInterface()->PushRequest(*request);
  
  if (endpointId != 0) //check for result only if not reset
                       /*NB: this is a BLOCKING call*/
     result = ParentInterface()->PopfromSut();
  
  return result;
}

//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void SVF_RawPlayer::DoReset(bool doSynchronousReset)
{
  if (doSynchronousReset){}; //Null operation, used to silence warning

  auto request = new CallbackRequest(CallbackId(0));
  ParentInterface()->PushRequest(*request);
}

//
//  End of: SVF_RawPlayer::DoReset
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_RawPlayer.cpp
//===========================================================================
