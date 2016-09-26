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
#include "g3log/g3log.hpp"
#include <thread>
#include <chrono>

using std::experimental::string_view;
using std::string;
using std::ofstream;
using std::ifstream;
using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace mast;


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


//! Deletes content of exchange files
//!
void SVF_SimulationProtocol::CleanUpFiles ()
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
//  End of: SVF_SimulationProtocol::CleanUpFiles
//---------------------------------------------------------------------------



//! Retrieves bitstrem data from SUT
//!
BinaryVector SVF_SimulationProtocol::FetchDataFromSut ()
{
  // ---------------- Read til next newline
  //
  string   fromSutBitstream;
  auto     keepOnReading = true;

  // ---------------- Wait for file to exists
  //
  auto startTime = std::chrono::steady_clock::now();
  auto refTime   = startTime;
  while (!Utility::FileExists(m_fromSutFilePath))
  {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = now - startTime;

    if (elapsed >= m_fromSutTimeout)
    {
      THROW_RUNTIME_ERROR("File does not exist: " + m_fromSutFilePath);
    }

    elapsed = now - refTime;
    if (elapsed >= 1s)
    {
      refTime = now;
      LOG(INFO) << "Waiting for file: " << m_fromSutFilePath;
    }
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
