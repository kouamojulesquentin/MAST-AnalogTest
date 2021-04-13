//===========================================================================
//                           Simulation_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Simulation_TranslationProtocol.cpp
//!
//! Implements class Simulation_TranslationProtocol
//!
//===========================================================================

#include "Simulation_TranslatorProtocol.hpp"
#include "SVFVector.hpp"
#include "Utility.hpp"
#include "RVF.hpp"

#include <experimental/string_view>
#include <sstream>
#include <chrono>

#include "g3log/g3log.hpp"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;

using std::ofstream;
using std::ifstream;
using namespace std::chrono_literals;
using namespace std::string_literals;

//  os << commandType << " " << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");\n";

/*Command formatting functions:*/

  auto FormatSVFData = [] (BinaryVector RawData)
  {
  //Prepare formatted SVF data
  ostringstream os;
  os << RawData.BitsCount() << " TDI(" << SVFVector(RawData).Data() << ");";
  return os.str();
  };


//! Loopbacks "to SUT data" logging SVF command(s) that would be issued if it was really an operating protocol
//! NOT VERIFIED YET!!! CHECK!!!!!!!

BinaryVector Simulation_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{

  ostringstream os;
  string toSutData;
  
   //Check for known commands to format
  if ((current_request.CallbackId()=="SDR") || (current_request.CallbackId()=="SIR"))
     toSutData = FormatSVFData(current_request.ToSutVector());
  else
   {
    //Unkown format: log it as bynary
   if (current_request.FormattedData().empty())
    toSutData = current_request.ToSutVector().DataAsBinaryString("", "");
   else  
    toSutData = current_request.FormattedData();
   } 
  os << current_request.CallbackId() << " " << toSutData <<"\n";

  auto command = os.str();

  while (command.back() == '\n')
  {
    command.pop_back();
  }
  LOG(INFO) << command;
  LOG(DEBUG) << "Simulation_TranslatorProtocol: Send data to exchange file " << m_toSutFilePath;
  SendCommand(command);

  LOG(DEBUG) << "Simulation_TranslatorProtocol: Fetching data from exchange file " << m_fromSutFilePath;
  auto fromSutData = FetchDataFromSut();

  return fromSutData;

 //Implement loopback behaviour
return current_request.ToSutVector();
}

//
//  End of: Simulation_TranslationProtocol::TransformationCallback
//---------------------------------------------------------------------------


//! Deletes content of exchange files
//!
void Simulation_TranslatorProtocol::CleanUpFiles ()
{
  // ---------------- Output file
  //
  ofstream ofs(m_toSutFilePath, std::ios_base::trunc);

  if (!ofs.is_open())
  {
    THROW_RUNTIME_ERROR("Cannot open output file: " + m_toSutFilePath);
  }

  // ---------------- input file
  //
  ofstream ifs(m_fromSutFilePath, std::ios_base::trunc);

  if (!ifs.is_open())
  {
    THROW_RUNTIME_ERROR("Cannot open input file: " + m_fromSutFilePath);
  }
}
//
//  End of: Simulation_TranslatorProtocol::CleanUpFiles
//---------------------------------------------------------------------------



//! Retrieves bitstrem data from SUT
//!
BinaryVector Simulation_TranslatorProtocol::FetchDataFromSut ()
{
  // ---------------- Read til next newline
  //
  string   fromSutBitstream;
  auto     keepOnReading = true;

  // ---------------- Wait for file to exists
  //
  auto startTime = std::chrono::steady_clock::now();
  auto refTime   = startTime;

  // ---------------- Waiting time checker
  //
  auto checkWaitTime = [&refTime](auto message)
  {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = now - refTime;
    if (elapsed >= 5s)
    {
      refTime  = now;

      LOG(INFO) << message;
    }
  };

  while (!Utility::FileExists(m_fromSutFilePath))
  {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = now - startTime;

    if (elapsed >= m_fromSutTimeout)
    {
      THROW_RUNTIME_ERROR("File does not exist: " + m_fromSutFilePath);
    }

    checkWaitTime("Waiting for file: " + m_fromSutFilePath);

    std::this_thread::sleep_for(m_fromSutWait);
  }

  ifstream ifs(m_fromSutFilePath);

  if (!ifs.is_open())
  {
    THROW_RUNTIME_ERROR("Cannot open input file: "s + m_fromSutFilePath);
  }

  ifs.seekg(m_lastPos);

  do
  {
    // ---------------- Seek to where we left last time
    //              (or at the beginning for the first time)
    //
    ifstream::char_type nextChar = '\0';
    ifs.get(nextChar);
    keepOnReading = true;

    if (ifs.eof())
    {
      ifs.close();

      std::this_thread::sleep_for(std::chrono::milliseconds(1));

      ifs.open(m_fromSutFilePath);
      ifs.seekg(m_lastPos);
    }
    else
    {
      m_lastPos = ifs.tellg();
      if (nextChar == '\n')
      {
        break;
      }

      fromSutBitstream += nextChar;
    }

    checkWaitTime("Waiting for data from SUT... ");
  } while (keepOnReading);

  // ---------------- Create binary from read line
  //
  auto fromSutVector = BinaryVector::CreateFromBinaryString(fromSutBitstream);

  return fromSutVector;
}
//
//  End of: Simulation_TranslatorProtocol::FetchDataFromSut
//---------------------------------------------------------------------------



//! Sends SVF command to SUT
//!
void Simulation_TranslatorProtocol::SendCommand (string_view command)
{
  ofstream ofs(m_toSutFilePath, std::ios_base::app);

  if (!ofs.is_open())
  {
    THROW_RUNTIME_ERROR("Cannot open output file: "s + m_toSutFilePath);
  }

  ofs << command <<'\n'; 
  ofs.flush();
  ofs.close();
}
//
//  End of: Simulation_TranslatorProtocol::SendCommand
//---------------------------------------------------------------------------

//===========================================================================
// End of SVF_RawPlayer.cpp
//===========================================================================
