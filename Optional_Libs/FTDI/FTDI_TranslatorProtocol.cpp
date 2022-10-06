//===========================================================================
//                           FTDI_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FTDI_TranslatorProtocol.cpp
//!
//! Implements class FTDI_TranslatorProtocol, which uses the lifFTDI library
//! to performs transactions on a remote target trought an FTDI chip
//! 
//! Current support: JTAG SDR and SIR
//!
//!------------------------ Pin mapping ----------------------------------
//!
//!                JTAG
//!AD0 - CN2-07 --> TCK
//!AD1 - CN2-10 --> TDI
//!AD2 - CN2-09 --> TDO
//!AD3 - CN2-12 --> TMS
//!AD4 - CN2-14 --> TRST
//===========================================================================

#include "FTDI_TranslatorProtocol.hpp"
#include "Utility.hpp"
#include "RVF.hpp"

#include <experimental/string_view>
#include <sstream>

#include "g3log/g3log.hpp"

//Functions for JTAG to bit-banging
#include "function.h"

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
FTDI_TranslatorProtocol::FTDI_TranslatorProtocol ()
{
}
//
//  End of: FTDI_TranslatorProtocol::FTDI_TranslatorProtocol
//---------------------------------------------------------------------------


BinaryVector FTDI_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
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
//  End of: FTDI_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of FTDI_TranslatorProtocol.cpp
//===========================================================================
