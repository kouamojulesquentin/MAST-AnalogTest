//===========================================================================
//                           Empty_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_Player_TranslatorProtocol.cpp
//!
//! Implements class PDL_Player_TranslatorProtocol, which can be used as a model
//! for real translators with its own Factory registration. 
//! As it is, it only transmits requests and results with no modification and is therefore functionally transparent
//!
//===========================================================================

#include "PDL_Player_TranslatorProtocol.hpp"
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


BinaryVector PDL_Player_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{
  // This callback is called each time a RVFRequest arrived
  
  BinaryVector higher_level_result;
  std::string PDLPrimitive(UNDEFINED);
  ostringstream os;

   //Syncrhonisation
   if (current_request.CallbackId()==NO_MORE_PENDING)
   {
    //Finished, release parent Translator
    RVFRequest request(NO_MORE_PENDING);
    PushRequest(request);
    return higher_level_result;
   }

   //Process "current_request"
  //Identify supported callbacks
  if ((current_request.CallbackId() == CSU) || (current_request.CallbackId()=="SDR") || (current_request.CallbackId()=="SIR")) 
   PDLPrimitive = CSU;
  if ((current_request.CallbackId() == RUNLOOP) || (current_request.CallbackId()== RUNTEST))
   PDLPrimitive = RUNTEST;
  if (current_request.CallbackId() == TRST) 
   PDLPrimitive = TRST;
  if (IS_SERVICE_CALLBACK(current_request.CallbackId())) 
    PDLPrimitive = current_request.CallbackId();
  
  if (PDLPrimitive == UNDEFINED)
  LOG(INFO)<<"Callback "<< current_request.CallbackId() <<" is not supported by PDL Player, skipping it"; 
  
  //Generate PDL commands
   if (PDLPrimitive == TRST) 
       os <<"iReset;";
   if (PDLPrimitive == RUNLOOP)
    {
     uint32_t loopcount;
     loopcount = std::experimental::any_cast<uint32_t>(current_request.m_optionalData);
      os <<"iRunLoop("<<loopcount<<");";
    } 
   if (PDLPrimitive == CSU)
    {
     os <<"iScan ";
     if (current_request.CallbackId()=="SIR")
      os << "-ir ";
     os << m_TargetInterface << " "<< current_request.ToSutVector().BitsCount() << " -si 0b"
       << current_request.ToSutVector().DataAsBinaryString("", "") ;
         if (!current_request.ExpectedData().IsEmpty())
      {
       LOG(INFO)<< "PDL Player found expected data";
       auto so_string = current_request.ExpectedData().DataAsBinaryString("", "");
       auto so_mask = current_request.ExpectedMask().DataAsBinaryString("", "") ;
       for(std::string::size_type i = 0; i < so_string.size(); ++i) 
          //Apply Mask
	  {
          if (so_mask[i] == '0') so_string[i] = 'x';;
           }
	os <<" -so 0b" << so_string;
      }
  
     
     os  <<";";
     }  

   
   auto command = os.str();
   while (command.back() == '\n')
   {
    command.pop_back();
   }
   LOG(INFO) << command;
   m_ofs << command << '\n'; //Log command into PDL log
   m_ofs.flush();

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
//  End of: PDL_Player_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of PDL_Player_TranslatorProtocol.cpp
//===========================================================================
