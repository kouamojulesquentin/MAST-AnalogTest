//===========================================================================
//                           Fake_SVF_Simulator.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Fake_SVF_Simulator.cpp
//!
//! Implements class Fake_SVF_Simulator
//!
//===========================================================================


#include "Fake_SVF_Simulator.hpp"
#include "BinaryVector.hpp"
#include "Utility.hpp"

#include <cxxtest/TestSuite.h>

using std::string;
using std::experimental::string_view;
using std::ofstream;
using std::ifstream;

using namespace std::string_literals;
using namespace mast;
using namespace test;



//! Extract bitstream from SVF command
//!
//! @note SVF command are formatted as this example: SDR 49 TDI(006C6DB0B0B0B0);\n
SVFVector Fake_SVF_Simulator::ExtractSVFVectorFromSVFCommand (string_view svfCommand)
{
  auto pos_numberOfBits     = svfCommand.find_first_of("123456789", 3u);
  auto pos_openParenthesis  = svfCommand.find('(');
  auto pos_closeParenthesis = svfCommand.find(')', pos_openParenthesis);

  if (   (pos_numberOfBits     == string_view::npos)
      || (pos_openParenthesis  == string_view::npos)
      || (pos_closeParenthesis == string_view::npos)
     )
  {
    THROW_LOGIC_ERROR("Unexpected non SVF command: '"s + string(svfCommand) + "'");
  }

  auto numberOfBits = stoul(string(svfCommand.substr(pos_numberOfBits)));
  auto bits         = svfCommand.substr(pos_openParenthesis + 1, pos_closeParenthesis - pos_openParenthesis - 1);
  auto bitsVector   = SVFVector(bits, numberOfBits);

  return bitsVector;
}
//
//  End of: UT_SVF_SimulationProtocol::ExtractSVFVectorFromSVFCommand
//---------------------------------------------------------------------------


//! Writes data to file "from SUT"
//!
void Fake_SVF_Simulator::Produce (string_view binaryData)
{
  ofstream ofs(m_fromSutFilePath, std::ios_base::app);

  TSM_ASSERT_TRUE ("Opening file: "s + m_fromSutFilePath, ofs.is_open());

  ofs << binaryData << "\n";
  ofs.flush();
  ofs.close();
}
//
//  End of: Fake_SVF_Simulator::Produce
//---------------------------------------------------------------------------


//! Writes SVFVector formatted data to file "from SUT"
//!
//! @note Data are first converted to BinaryVector organized data
void Fake_SVF_Simulator::Produce (const SVFVector& svfVector)
{
  auto binaryVector = svfVector.ToBinaryVector();
  Produce(binaryVector.DataAsBinaryString("", ""));
}
//
//  End of: Fake_SVF_Simulator::Produce
//---------------------------------------------------------------------------


//! Clears files content
//!
void Fake_SVF_Simulator::Reset ()
{
  ofstream ofs1(m_fromSutFilePath, std::ios_base::trunc);
  ofstream ofs2(m_toSutFilePath,   std::ios_base::trunc);
}
//
//  End of: Fake_SVF_Simulator::Reset
//---------------------------------------------------------------------------


//! Starts thread that reads data from "To SUT" file, converts it to binary bitstream and writes result into "From SUT" file
//!
void Fake_SVF_Simulator::Start ()
{
  auto loopback = [this]()
  {
    ifstream ifs(m_toSutFilePath);

    std::ifstream::pos_type lastPos = 0; //!< Position of last command line to SUT
    string toSutCommand;

    while (m_run)
    {
      toSutCommand.clear();

      auto keepOnReading = false;

      // ---------------- Read til next newline
      //
      do
      {
        keepOnReading = true;

        // ---------------- Seek to where we left last time
        //              (or at the beginning for the first time)
        //
        ifstream::char_type nextChar = '\0';
        ifs.get(nextChar);

        if (ifs.eof())
        {
          // ---------------- Force ifstream to detect file changes
          //
          ifs.close();
          std::this_thread::sleep_for(std::chrono::milliseconds(0));
          ifs.open(m_toSutFilePath);
          ifs.seekg(lastPos);
        }
        else
        {
          lastPos = ifs.tellg();
          if (nextChar == '\n')
          {
            keepOnReading = false;
            break;
          }
          toSutCommand += nextChar;
        }
      } while (keepOnReading & m_run);

      if (!toSutCommand.empty())
      {
        // ---------------- Do loopback (converting commands to binary data)
        //
        TS_ASSERT_THROWS_NOTHING (
          auto svfVector = ExtractSVFVectorFromSVFCommand(toSutCommand);
          Produce(svfVector);
        );
      }
    }
  };

  // ---------------- Setup thread
  //
  m_run            = true;
  m_loopbackThread = std::thread(loopback);
}
//
//  End of: Fake_SVF_Simulator::Start
//---------------------------------------------------------------------------


//! Stops loopback thread
//!
void Fake_SVF_Simulator::Stop ()
{
  m_run = false;
  if (m_loopbackThread.joinable())
  {
    m_loopbackThread.join();
  }
}
//
//  End of: Fake_SVF_Simulator::Stop
//---------------------------------------------------------------------------




//===========================================================================
// End of Fake_SVF_Simulator.cpp
//===========================================================================
