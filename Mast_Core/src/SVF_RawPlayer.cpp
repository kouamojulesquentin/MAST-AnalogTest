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
#include "RVF.hpp"
#include "g3log/g3log.hpp"

#include <experimental/string_view>
#include <sstream>

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;


//! sends request for TRST,SIR and SDR callbacks and waits for response

BinaryVector SVF_RawPlayer::DoCallback (RVFRequest Request, uint32_t channelId)
{
  BinaryVector result;
  string svfFormattedData;
  BinaryVector callback_toSutData;
  
  std::string SVFCallback(UNDEFINED);

  auto FormatSVFData = [] (BinaryVector RawData)
  {
  //Prepare formatted SVF data
  ostringstream os;
  os << RawData.BitsCount() << " TDI(" << SVFVector(RawData).Data() << ");";
  return os.str();
  };

  auto FormatSVFExpectAndMask = [] (BinaryVector ExpectedData,BinaryVector ExpectedMask)
  {
  //Prepare formatted SVF data
  ostringstream os;
  os << ExpectedData.BitsCount() << " TDO(" << SVFVector(ExpectedData).Data() <<  " MASK(" << SVFVector(ExpectedMask).Data() <<");";
  return os.str();
  };

 //Identify supported callbacks
  if (Request.CallbackId() == CSU) 
   SVFCallback = CallbackForChannel(channelId);
  if (Request.CallbackId() == RUNLOOP) 
   SVFCallback = RUNTEST;
  if (Request.CallbackId() == TRST) 
   SVFCallback = TRST;
  if (IS_SERVICE_CALLBACK(Request.CallbackId())) 
    SVFCallback = Request.CallbackId();
  
  if (SVFCallback == UNDEFINED)
  LOG(ERROR_LVL)<<"Callback "<< Request.CallbackId() <<" is not supported by this AccessInterface"; 
   
  if (channelId != 0) //No data in the request dor Reset operation
      {
//      svfFormattedData = os.str();
      svfFormattedData = FormatSVFData(Request.ToSutVector());
      if (!Request.ExpectedData().IsEmpty())
       svfFormattedData.append(FormatSVFExpectAndMask(Request.ExpectedData(),Request.ExpectedMask()));
      callback_toSutData = Request.ToSutVector();
      }

  //Many Unit Test depend on FormattedData
  RVFRequest up_request(SVFCallback,callback_toSutData,svfFormattedData);
      if (!Request.ExpectedData().IsEmpty())
        { 
	 LOG(DEBUG)<<"SVF Player: RVF request has Expected data check"; 

	    up_request.SetExpectedData(Request.ExpectedData());
	    up_request.SetExpectedMask(Request.ExpectedMask());
	
	}
  up_request.m_optionalData = Request.m_optionalData;
  
  PushRequest(up_request);
  
                       /*NB: this is a BLOCKING call*/
     result = PopfromSut();
   

 //Update Cycle count
      switch (channelId){
       case 0: this->increaseElapsedCycles(nTRST_OVERHEAD_CYCLES); break; 
       case 1: this->increaseElapsedCycles(SIR_OVERHEAD_CYCLES+Request.ToSutVector().BitsCount()); break;
       case 2: this->increaseElapsedCycles(SDR_OVERHEAD_CYCLES+Request.ToSutVector().BitsCount()); break;
       default: ; //Should never arrive here, a runtime error would have been thrown before by "CallbackId"
      }

  return result;
}

//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void SVF_RawPlayer::DoReset(bool doSynchronousReset)
{
  if (doSynchronousReset){}; //Null operation, used to silence warning

  RVFRequest request(CallbackForChannel(0));
  PushRequest(request);
  PopfromSut();
}

//
//  End of: SVF_RawPlayer::DoReset
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_RawPlayer.cpp
//===========================================================================
