//===========================================================================
//                           Emulation_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Emulation_TranslationProtocol.cpp
//!
//! Implements class Emulation_TranslationProtocol
//!
//===========================================================================

#include "JTAG_BitBang_TranslatorProtocol.hpp"
#include "Utility.hpp"
#include "CallbackRequest.hpp"

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



BinaryVector JTAG_BitBang_TranslatorProtocol::PDL_translator( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector)
{
return ToSutVector;
}                   

BinaryVector JTAG_BitBang_TranslatorProtocol::TransformationCallback(CallbackRequest current_request) 
{
  BinaryVector result;
  string i2c_FormattedData;
  BinaryVector callback_toSutData;
  auto toSutData = current_request.ToSutVector();
  int32_t endpointId=-1;
  
 if (!Translator_Running())
   Start_Translator();
   
  if (current_request.CallbackId()==TRST)
    endpointId = 0;
  if (current_request.CallbackId()==SIR)
    endpointId = 1;
  if (current_request.CallbackId()==SDR)
    endpointId = 2;
  if (current_request.CallbackId()==NO_MORE_PENDING)
   {
    //Finished, terminate T-2-E translator thread
    CallbackRequest request(NO_MORE_PENDING);
    PushRequest(request);
    return result;
   }
  CHECK_PARAMETER_GTE(endpointId,0,"Error: unsupported Callback "+current_request.CallbackId());
  
  //Push request to PDL translator thread
   PushRequest(current_request);

  //Wait for result from PDL translator thread
  result = PopfromSut();
  
  return result;
  
}
//
//  End of: JTAG_BitBang_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of JTAG_BitBang_TranslatorProtocol.cpp
//===========================================================================
