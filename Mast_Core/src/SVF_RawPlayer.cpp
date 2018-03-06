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


//! sends request for TRST,SIR and SDR callbacks and waits for response

BinaryVector SVF_RawPlayer::DoCallback (uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  BinaryVector result;
  string svfFormattedData;
  BinaryVector callback_toSutData;
  
  //Prepapre formatted SVF data
  ostringstream os;
  os << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");";
  if (endpointId != 0) //No data in the request dor Reset operation
      {
      svfFormattedData = os.str();
      callback_toSutData = toSutData;
      }

  CallbackRequest request(CallbackId(endpointId),callback_toSutData,svfFormattedData);
  PushRequest(request);
  
                       /*NB: this is a BLOCKING call*/
     result = PopfromSut();

  return result;
}

//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void SVF_RawPlayer::DoReset(bool doSynchronousReset)
{
  if (doSynchronousReset){}; //Null operation, used to silence warning

  CallbackRequest request(CallbackId(0));
  PushRequest(request);
  PopfromSut();
}

//
//  End of: SVF_RawPlayer::DoReset
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_RawPlayer.cpp
//===========================================================================
