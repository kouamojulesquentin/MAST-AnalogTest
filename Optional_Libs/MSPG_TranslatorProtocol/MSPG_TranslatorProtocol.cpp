//===========================================================================
//                           Emulation_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MSPG_TranslatorProtocol.cpp
//!
//! Implements class MSPG_TranslatorProtocol, which can be used as a model
//! for real translators with its own Factory registration. 
//! As it is, it only transmits requests and results with no modification and is therefore functionally transparent
//!
//===========================================================================

#include "MSPG_TranslatorProtocol.hpp"
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

//
//  End of: MSPG_TranslatorProtocol::MSPG_TranslatorProtocol
//---------------------------------------------------------------------------

  auto FormatSVFData = [] (BinaryVector RawData)
  {
  //Prepare formatted SVF data
  ostringstream os;
  os << RawData.BitsCount() << " TDI(" << /*SVFVector(RawData).Data() <<*/ ");";
  return os.str();
  };

  auto FormatI2CCommand = [] (RVFRequest current_request)
  {
  //Prepare formatted SVF data
  ostringstream os;
  os << "(0x" << std::hex<< *(uint32_t *) current_request.interfaceData()<<",0x"<< current_request.ToSutVector().DataAsHexString() << ");";
  LOG(DEBUG)<<"I2C data is : " << os.str();
  return os.str();
  };


BinaryVector MSPG_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{
  // This callback is called each time a RVFRequest arrived
  
  ostringstream os;
  string toSutData;
  BinaryVector Emulated_FromSUT=current_request.ToSutVector();
  
  string commandData;
  
  os << "MSPG.";
  
  //Check for known commands to format
   if (current_request.CallbackId()=="I2C_READ")
     {
     os << "DIG.Read.I2C";
     commandData = FormatI2CCommand(current_request);
     }
   else
     if (current_request.CallbackId()=="I2C_WRITE")
     {
     os << "DIG.Write.I2C";
     commandData = FormatI2CCommand(current_request);
     }
   else
      if (current_request.CallbackId()=="iMeasureVoltage")
     {
     os << "DIG.DVI.Strobe.";
     commandData = '\"';
     commandData += (char *) current_request.interfaceData();
     commandData += '\"';
     }
   else
    if (current_request.CallbackId()==DATAPORTREAD)
     {
     os << "DIG.DVI.Pin.Read ";
     commandData = '\"';
     commandData += (char *) current_request.interfaceData();
     commandData += '\"';
     }
   else
    {
     //Unkown format: log it as bynary
     commandData = current_request.ToSutVector().DataAsBinaryString("", "");
    }
  LOG(DEBUG)<<"commandData is : " << commandData;
  os << commandData <<"\n";

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
//  End of: MSPG_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of MSPG_TranslatorProtocol.cpp
//===========================================================================
