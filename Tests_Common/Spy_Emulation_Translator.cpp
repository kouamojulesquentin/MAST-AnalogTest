//===========================================================================
//                           Spy_Emulation_Translator.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_Emulation_Translator.cpp
//!
//! Implements class Spy_Emulation_Translator
//!
//===========================================================================

#include "Spy_Emulation_Translator.hpp"
#include "SVFVector.hpp"
#include "Utility.hpp"
#include "CallbackRequest.hpp"
#include <sstream>

using std::experimental::string_view;
using std::string;
using std::ostringstream;
using namespace mast;
using namespace test;


//! Spies content how binary vector to SUT is transformed to SVF command while returning the BinaryVector unchanged
//!
BinaryVector Spy_Emulation_Translator::TransformationCallback(CallbackRequest current_request) 
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

  SaveCommands(command);

  return current_request.ToSutVector();
}
//
//  End of: Spy_Emulation_Translator::DoCallback
//---------------------------------------------------------------------------


//! Saves SVF commands
//!
//! @param commands New line separated commands
//!
void Spy_Emulation_Translator::SaveCommands (string_view commands)
{
  auto splitCommands = Utility::Split(commands, "\n");

  for (auto command : splitCommands)
  {
    if (!command.empty())
    {
      m_spiedCommands->SaveCommand(string(command));
    }
  }
}
//
//  End of: Spy_Emulation_Translator::SaveCommands
//---------------------------------------------------------------------------



//===========================================================================
// End of Spy_Emulation_Translator.cpp
//===========================================================================
