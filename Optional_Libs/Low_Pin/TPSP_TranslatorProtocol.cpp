//===========================================================================
//                           Emulation_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TPSP_TranslatorProtocol.cpp
//!
//! Implements class TPSP_TranslatorProtocol, which can be used as a model
//! for real translators with its own Factory registration. 
//! As it is, it only transmits requests and results with no modification and is therefore functionally transparent
//!
//===========================================================================

#include "TPSP_TranslatorProtocol.hpp"
#include "Utility.hpp"
#include "RVF.hpp"

#include <experimental/string_view>
#include <sstream>

#include "g3log/g3log.hpp"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;

using std::vector;
using std::initializer_list;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

//! Initializeation with no parameters
//!
TPSP_TranslatorProtocol::TPSP_TranslatorProtocol ()
{
}
//
//  End of: TPSP_TranslatorProtocol::TPSP_TranslatorProtocol
//---------------------------------------------------------------------------


BinaryVector TPSP_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{
  // This callback is called each time a RVFRequest arrived
  BinaryVector result;
  BinaryVector tpsp_tdo_bit;
  string TPSP_FormattedData;
  auto toSutData = current_request.ToSutVector();
  
  
   LOG(INFO) << " Protocol " <<this ->KindName()  <<  ":RVF Request "<< current_request.CallbackId() <<" received at internal cycle " << 
     this->getElapsedCycles();

if (current_request.CallbackId()==NO_MORE_PENDING)
   {
  LOG(DEBUG) << "TPSP Callback: closing down";
    //Finished, release parent Translator
    RVFRequest request(NO_MORE_PENDING);
    PushRequest(request);
    return result;
   }

 /*TPSP generates a 3- bit transaction TDO-TDI-TMS that big-bangs the TAP*/
 /*Request if composed by TDI-TMS, TSO is in the Response*/
  
  /*Standard resquests to change state without having to generate them each time*/
  RVFRequest TPSP_TMS_0("TPSP_CYCLE",BinaryVector::CreateFromBinaryString("00"));
  RVFRequest TPSP_TMS_1("TPSP_CYCLE",BinaryVector::CreateFromBinaryString("01"));
                   

  if ((current_request.CallbackId()=="SDR") || (current_request.CallbackId()=="SIR"))
   {
    /*Preamble: go to select-DR from RTI*/ 
    PushRequest(TPSP_TMS_1);
    PopfromSut(); //Need to remove from queue, but return data in not useful
    increaseElapsedCycles(TPSP_CYCLES);
    if (current_request.CallbackId()=="SIR") //Go to IR branch if needed
           {
	      PushRequest(TPSP_TMS_1);
              PopfromSut(); //Need to remove from queue, but return data in not useful
             }
	     
    /*go to Capture*/ 
    PushRequest(TPSP_TMS_0);
    PopfromSut(); //Need to remove from queue, but return data in not useful

    /*go to Shift*/ 
    PushRequest(TPSP_TMS_0);
    PopfromSut(); //Need to remove from queue, but return data in not useful
    
    /*Shift Data*/
    auto TDI_string = toSutData.DataAsBinaryString();
    LOG(DEBUG)<<"TPSP data string is "<<TDI_string;
        for (int cur_bit=toSutData.BitsCount()-1 ;cur_bit>=0;cur_bit--)
     {
      PushRequest(RVFRequest("TPSP_CYCLE",toSutData.Slice(cur_bit,1)+BinaryVector::CreateFromBinaryString("0")));
      tpsp_tdo_bit= PopfromSut(); //Collect TDO bit
      result.Append(tpsp_tdo_bit);
      }
    /*Depending in TPSP semantics, "result" has to be inversed*/
    result=result.ReverseSlice(0,result.BitsCount());
    
    /*go to Exit*/ 
    PushRequest(TPSP_TMS_1);
    PopfromSut(); //Need to remove from queue, but return data in not useful

    /*go to Update*/ 
    PushRequest(TPSP_TMS_1);
    PopfromSut(); //Need to remove from queue, but return data in not useful

    /*back to RTI*/ 
    PushRequest(TPSP_TMS_0);
    PopfromSut(); //Need to remove from queue, but return data in not useful

   }
  
   LOG(INFO) << " Protocol " <<this ->KindName()  <<  ":RVF Request "<< current_request.CallbackId() <<" finished at internal cycle " << 
     this->getElapsedCycles();
  return result;

}
//
//  End of: TPSP_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of TPSP_TranslatorProtocol.cpp
//===========================================================================
