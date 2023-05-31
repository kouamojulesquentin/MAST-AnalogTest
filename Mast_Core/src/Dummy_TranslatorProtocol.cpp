//===========================================================================
//                           Emulation_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dummy_TranslatorProtocol.cpp
//!
//! Implements class Dummy_TranslatorProtocol, which can be used as a model
//! for real translators. As it is, it only transmits requests and results with no modification
//!
//===========================================================================

#include "Dummy_TranslatorProtocol.hpp"
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

//! Initializes with addresses defined by a string to be parsed
//!
Dummy_TranslatorProtocol::Dummy_TranslatorProtocol (const std::string& parameters)
{
}
//
//  End of: Dummy_TranslatorProtocol::Dummy_TranslatorProtocol
//---------------------------------------------------------------------------


BinaryVector Dummy_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{
  // This callback is called each time a RVFRequest arrived
  
  BinaryVector higher_level_result;
  
   //Syncrhonisation
   if (current_request.CallbackId()==NO_MORE_PENDING)
   {
    //Finished, release parent Translator
    RVFRequest request(NO_MORE_PENDING);
    PushRequest(request);
    return higher_level_result;
   }

   //Process "current_request"
  // Prepare a request to the higher-level interface: 
 
  //Just pass request without modifying it
  RVFRequest higher_level_request=current_request;
  
  //Push request to higher level
  PushRequest(higher_level_request);
  
//wait for Result from higher level; it is a BLOCKING call
   higher_level_result = PopfromSut(); 
  
  //Process "result"
  auto lower_level_result=higher_level_result;
  
  //Return Callback result to lower level
  return lower_level_result;
}
//
//  End of: Dummy_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of Dummy_TranslatorProtocol.cpp
//===========================================================================
