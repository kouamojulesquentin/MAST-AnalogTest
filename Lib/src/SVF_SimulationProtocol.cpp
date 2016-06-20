//===========================================================================
//                           SVF_SimulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_SimulationProtocol.cpp
//!
//! Implements class SVF_SimulationProtocol
//!
//===========================================================================

#include "SVF_SimulationProtocol.hpp"
#include "Utility.hpp"
#include <thread>
#include <chrono>

using std::experimental::string_view;
using std::string;
using std::ofstream;
using std::ifstream;
using namespace mast;
using namespace std::string_literals;


//! Spies content of parameter toSutData and return it unchanged
//!
BinaryVector SVF_SimulationProtocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateSVFCommand(derivationId, toSutData);
  SendCommand(command);

  auto fromSutData = FetchDataFromSut();
  return fromSutData;
}
//
//  End of: Spy_AccessInterfaceProtocols::DoAction
//---------------------------------------------------------------------------



//! Retrieves bitstrem data from SUT
//!
BinaryVector SVF_SimulationProtocol::FetchDataFromSut ()
{
  // ---------------- Read til next newline
  //
  string   fromSutBitstream;
  auto     keepOnReading = true;
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
      std::this_thread::sleep_for(std::chrono::milliseconds(0));
      ifs.open(m_fromSutFilePath);
      ifs.seekg(m_lastPos);
    }
    else
    {
      m_lastPos = ifs.tellg();
      if (nextChar == '\n')
      {
        keepOnReading = false;
        break;
      }
      fromSutBitstream += nextChar;
    }
  } while (keepOnReading);

  // ---------------- Create binary from read line
  //
  auto fromSutVector = BinaryVector::CreateFromBinaryString(fromSutBitstream);

  return fromSutVector;
}
//
//  End of: SVF_SimulationProtocol::FetchDataFromSut
//---------------------------------------------------------------------------



//! Sends SVF command to SUT
//!
void SVF_SimulationProtocol::SendCommand (string_view command)
{
  ofstream ofs(m_toSutFilePath, std::ios_base::app);

  if (!ofs.is_open())
  {
    THROW_RUNTIME_ERROR("Cannot open output file: "s + m_toSutFilePath);
  }

  ofs << command; // Command already include '\n'
  ofs.flush();
  ofs.close();
}
//
//  End of: SVF_SimulationProtocol::SendCommand
//---------------------------------------------------------------------------




//===========================================================================
// End of SVF_SimulationProtocol.cpp
//===========================================================================
