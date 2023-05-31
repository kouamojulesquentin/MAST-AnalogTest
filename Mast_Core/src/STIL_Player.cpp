//===========================================================================
//                           STIL_Player.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file STIL_Player.cpp
//!
//! Implements class STIL_Player
//!
//===========================================================================

#include "STIL_Player.hpp"
#include "SVFVector.hpp"
#include "Utility.hpp"

#include <experimental/string_view>
#include <sstream>

using std::string;
using std::experimental::string_view;
using std::ostringstream;

using namespace std::experimental::literals::string_view_literals;
using namespace mast;


//! Initializes a with number of Channels defined by a string
//!
STIL_Player::STIL_Player (const std::string& nbChannels)
{
  try
  {
    auto number = std::stoull(string(nbChannels), nullptr, 0);

    CHECK_PARAMETER_LTE(number, UINT32_MAX, nbChannels + ", is out of range for a 32 bit number");

    m_nbChannels = static_cast<uint32_t>(number);
  }
  catch(std::out_of_range& exc)     // Conversion to number is not possible ==> it must be the prefix
  {
    THROW_INVALID_ARGUMENT("Parameter: "sv + nbChannels + ", is out of range for a 32 bit number");
  }
  catch(std::exception& exc)
  {
    THROW_INVALID_ARGUMENT("Parameter: \""sv + nbChannels + "\", is not a number (to define number of channels)");
  }
}
//
//  End of: STIL_Player::STIL_Player
//---------------------------------------------------------------------------



//! Creates an STIL command associated to ChannelId identifier and BinaryVector to send to SUT
//!
std::vector <string>STIL_Player::CreateSTILCommand (uint32_t channelId, const BinaryVector& toSutData) const
{
  std::vector <string>STIL_commands;

  ostringstream os;
  BinaryVector select;

  if (channelId == 0)
  {
    STIL_commands.push_back(CreateResetSTILCommand(false));
    return STIL_commands;
  }

  if (channelId > m_nbChannels)
    THROW_INVALID_ARGUMENT("ChannelId must be comprised between '0' (for Reset) and tha total number of chains");

  select.Append(channelId, m_nbChannels);

  os << "V{ CHAIN = " << select.DataAsBinaryString("", "") << "010-T}\n"; //Capture Cycle
  STIL_commands.push_back(os.str());
  os.str("");
  os.clear();

  auto toSutData_string = toSutData.DataAsBinaryString("", "");

  for (auto bit = --toSutData_string.end() ; bit >= toSutData_string.begin() ; bit--) //Shift cycle
  {
    os << "V{ CHAIN = " << select.DataAsBinaryString("", "") << "100";
    os << *bit;
    os << "T}\n";
    STIL_commands.push_back(os.str());
    os.str("");
    os.clear();
  }

  os << "V{ CHAIN = " << select.DataAsBinaryString("", "") << "001-T}\n"; //Update Cycle
  STIL_commands.push_back(os.str());
  os.str("");
  os.clear();

  return STIL_commands;
}
//
//  End of: STIL_Player::CreateSTILCommand
//---------------------------------------------------------------------------


//! Creates an STIL reset command
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
string STIL_Player::CreateResetSTILCommand (bool doSynchronousReset) const
{
  ostringstream os;

  if (SupportTRST() && !doSynchronousReset)
  {
    os << "V{ reset = 1; clock = T}\n"; //Toggle clk
    os << "V{ reset = 0; clock = T}\n"; //Toggle clk
  }
  else
  {
    os << "V{ reset = T; clock = -}\n"; //Toggle reset
  }

  auto STILCommand = os.str();

  return STILCommand;
}
//
//  End of: STIL_Player::CreateSTILCommand
//---------------------------------------------------------------------------

/*
Ex STIL for a TAP:
          T means Toggling
 signalgroups "JTAG" = "TRSTN" + "TMS" + "TDI" + "TCK";


  DR cycle (shiftbits):
     //Starts at RTI
 V { JTAG = 11-T; } //RTI->SelectDR
 V { JTAG = 10-T; } //SelectDR->CaptureDR
 V { JTAG = 10-T; } //CaptureDR->ShiftDR

Loop <shiftbits-1>:
 V { JTAG = 10dT; } //ShiftDR->ShiftDR

 V { JTAG = 11dT; } //ShiftDR->Exit1-DR

 V { JTAG = 11-T; } //Exit1-DR->UpdateDR
 V { JTAG = 10-T; } //UpdateDR->RTI


  IR cycle(shiftbits):
     //Starts at RTI
 V { JTAG = 11-T; } //RTI->SelectDR
 V { JTAG = 11-T; } //SelectDR->SelectIR
 V { JTAG = 10-T; } //SelectIR->CaptureIR
 V { JTAG = 10-T; } //CaptureIR->ShiftIR

Loop <shiftbits-1>:
 V { JTAG = 10aT; } //ShiftIR->ShiftIR

 V { JTAG = 11aT; } //ShiftIR->Exit1-IR

 V { JTAG = 11-T; } //Exit1-IR->UpdateIR
 V { JTAG = 10-T; } //UpdateIR->RTI

Ex STIL for a non-wrapper scan chain:
          T means Toggling
 signalgroups "CHAIN" = "Sel0" + "Sel1" + ... + "Seln" + "ShiftEN" + "CaptureEN" + "UpdateEN" + "TDI"  "CLK";
 signalgroups "Select" = ;

 CSU sequence:
  V{ CHAIN = SEL010 - T} //Capture
Loop <shiftbits>:
  V{ CHAIN = SEL100 d T} //Shift
  V{ CHAIN = SEL001 - T} //Update

*/

//===========================================================================
// End of STIL_Player.cpp
//===========================================================================
