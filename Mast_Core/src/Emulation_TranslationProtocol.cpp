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
#include "RVF.hpp"

#include <experimental/string_view>
#include <sstream>

#include "g3log/g3log.hpp"

using namespace mast;
using std::ofstream;
using std::string;
using std::experimental::string_view;
using std::ostringstream;


//  os << commandType << " " << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");\n";

/*Command formatting functions:*/

  auto FormatSVFData = [] (BinaryVector RawData)
  {
  //Prepare formatted SVF data
  ostringstream os;
  os << RawData.BitsCount() << " TDI(" << SVFVector(RawData).Data() << ");";
  return os.str();
  };

  auto FormatI2CCommand = [] (RVFRequest current_request)
  {
  //Prepare formatted SVF data
  ostringstream os;
  os << "(0x" << std::hex<< *(uint32_t *) current_request.interfaceData()<<",0x"<< current_request.ToSutVector().DataAsHexString() << ");";
  return os.str();
  };


//! Loopbacks "to SUT data" logging SVF command(s) that would be issued if it was really an operating protocol
//! NOT VERIFIED YET!!! CHECK!!!!!!!

BinaryVector Emulation_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{

  ostringstream os;
  string toSutData;
  BinaryVector Emulated_FromSUT=current_request.ToSutVector();
  
  //Check for known commands to format
  if ((current_request.CallbackId()=="SDR") || (current_request.CallbackId()=="SIR"))
     toSutData = FormatSVFData(current_request.ToSutVector());
  else
   if ((current_request.CallbackId()=="I2C_READ") || (current_request.CallbackId()=="I2C_WRITE"))
     toSutData = FormatI2CCommand(current_request);
   else
    {
     //Unkown format: log it as bynary
     toSutData = current_request.ToSutVector().DataAsBinaryString("", "");
    } 
  os << current_request.CallbackId() << " " << toSutData <<"\n";

  auto command = os.str();

  while (command.back() == '\n')
  {
    command.pop_back();
  }
  LOG(INFO) << command;
  m_ofs << command << '\n'; //Log command into Emulation log
  m_ofs.flush();

  LOG(DEBUG) << "Emulation_TranslatorProtocol: Emulating loopback behaviour by returning input data";

  if (current_request.CallbackId()=="TPSP_CYCLE")
    //TPSP_CYCLE is assymmetric: it sends 2 bits (TDI and TMS) but receives only one
    Emulated_FromSUT=Emulated_FromSUT.Slice(0,1);
 //Implement loopback behaviour
return Emulated_FromSUT;
}

//
//  End of: Emulation_TranslationProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of SVF_RawPlayer.cpp
//===========================================================================
