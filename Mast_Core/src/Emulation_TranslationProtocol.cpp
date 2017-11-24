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

#include "Emulation_TranslatorProtocol.hpp"
#include "SVFVector.hpp"
#include "Utility.hpp"
#include "CallbackRequest.hpp"

#include <experimental/string_view>
#include <sstream>

#include "g3log/g3log.hpp"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;


//  os << commandType << " " << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");\n";

//! Loopbacks "to SUT data" logging SVF command(s) that would be issued if it was really an operating protocol
//! NOT VERIFIED YET!!! CHECK!!!!!!!

BinaryVector Emulation_TranslatorProtocol::TransformationCallback(CallbackRequest current_request) 
{

  ostringstream os;
  string toSutData;
  
  if (current_request.FormattedData().empty())
    toSutData = current_request.ToSutVector().DataAsBinaryString("", "");
  else  
    toSutData = current_request.FormattedData();
  os << current_request.CallbackId() << " " << toSutData <<"\n";

  auto command = os.str();

  while (command.back() == '\n')
  {
    command.pop_back();
  }
  LOG(INFO) << command;

 //Implement loopback behaviour
return current_request.ToSutVector();
}

//
//  End of: Emulation_TranslationProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_RawPlayer.cpp
//===========================================================================
