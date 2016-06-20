//===========================================================================
//                           Fake_SVF_Simulator.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Fake_SVF_Simulator.hpp
//!
//! Declares Fake_SVF_Simulator class
//!
//===========================================================================


#ifndef FAKE_SVF_SIMULATOR_H__4A97A73D_7000_4AA6_491_A17C26432A89__INCLUDED_
  #define FAKE_SVF_SIMULATOR_H__4A97A73D_7000_4AA6_491_A17C26432A89__INCLUDED_

#include "SVFVector.hpp"

#include <string>
#include <experimental/string_view>
#include <fstream>
#include <thread>
#include <atomic>

namespace test
{
//! Acts as a "loopback" SVF compatible simulator
//!
//! @note It consumes last line of an SVF command formatted file (one command per line)
//!       It produces last line of an ASCII binary bitstream (one "from SUT" bitstream per line)
//!
class Fake_SVF_Simulator final
{
  // ---------------- Public  Methods
  //
  public:
  ~Fake_SVF_Simulator() { Stop(); }
  Fake_SVF_Simulator(std::experimental::string_view toSutFilePath, std::experimental::string_view fromSutFilePath)
    : m_toSutFilePath   (toSutFilePath)
    , m_fromSutFilePath (fromSutFilePath)
  {
    Reset();
    m_run = false;
  }

  static mast::SVFVector ExtractSVFVectorFromSVFCommand (std::experimental::string_view svfCommand);

  void Produce(std::experimental::string_view binaryData);  //!< Add a line in file "from SUT"
  void Produce(const mast::SVFVector& svfVector);           //!< Writes SVFVector formatted data to file "from SUT"

  void Reset(); //!< Clears files content
  void Start(); //!< Starts thread that reads data from "To SUT" file, converts it to binary bitstream and writes result into "From SUT" file
  void Stop();  //!< Stops loopback thread

  // ---------------- Private  Fields
  //
  private:
  std::string             m_toSutFilePath   = "./to_RTL/data_to_rtl.svf";     //!< File used to "receive" SVF commands
  std::string             m_fromSutFilePath = "./from_RTL/data_from_rtl.dat"; //!< File used to "transmit" from SUT bitstream
  std::ifstream::pos_type m_lastPos         = 0;                              //!< Position of last SVF comman "received" from MAST
  std::thread             m_loopbackThread;
  std::atomic_bool        m_run;                          //!< Flags to run/stop the loopback thread
};
//
//  End of Fake_SVF_Simulator class declaration
//---------------------------------------------------------------------------
} // End of namespace test




#endif  // not defined FAKE_SVF_SIMULATOR_H__4A97A73D_7000_4AA6_491_A17C26432A89__INCLUDED_

//===========================================================================
// End of Fake_SVF_Simulator.hpp
//===========================================================================
