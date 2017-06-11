//===========================================================================
//                           Advantest_SmartRDI_Protocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Advantest_SmartRDI_Protocol.cpp
//!
//! Implements class Advantest_SmartRDI_Protocol
//!
//===========================================================================

#include "Advantest_SmartRDI_Protocol.hpp"
#include "Advantest_JTAG.hpp"
#include "rdi.hpp"

#include <iostream>

using std::string;
using std::vector;
using std::pair;

using std::make_pair;
using namespace mast;

//! Destructor does nothing
Advantest_SmartRDI_Protocol::~Advantest_SmartRDI_Protocol ()
{
}


//! Constructor resets pattern count
Advantest_SmartRDI_Protocol::Advantest_SmartRDI_Protocol ()
{
	  RDI_INIT();
	  pattern_count=0;
}

bool is_space(unsigned char const c ){ return std::isspace(c);};

//! Returns toSutScanVector untouched
//!
//! @param commandName  Command name (SIR, SDR, RST...)
//! @param bitsCount    Number of valid bits in scan vector
//! @param scanVector   Binary data to send to SUT (default is right aligned)
//!
//! @return data scanned out from SUT
pair<uint32_t, vector<uint8_t> >
Advantest_SmartRDI_Protocol::DoAction (const string&          commandName,
                                       uint32_t               bitsCount,
                                       const vector<uint8_t>& toSutScanVector)
{
  //typedef void (*JtagFunction_t)(int, const string&);
  typedef void (*JtagFunction_t)(const vector<uint8_t>&, uint32_t , const std::string& );

  uint8_t bytesCount = toSutScanVector.size();
//  vector<uint8_t> fromSutScanVector;

  std::cout << "Advantest_SmartRDI_Protocol::DoAction\n";

  if ((bitsCount == 0) || (bytesCount == 0))
  {
    throw std::runtime_error("DoAction: Invalid due to no bits to send to SUT");
  }

  if (((bitsCount + 7u) / 8u) != bytesCount)
  {
    throw std::runtime_error("DoAction: Invalid due to incoherence between bits count and data vector bytes count");
  }

  JtagFunction_t jtagFunc = NULL;

  string local_commandName = commandName;
  local_commandName.erase(std::remove_if(local_commandName.begin(), local_commandName.end(), is_space), local_commandName.end());
  if      (local_commandName == "SIR")
  {
    jtagFunc = jtag_shift_ir_cycle;
  }
  else if (local_commandName == "SDR")
  {
    jtagFunc = jtag_shift_dr_cycle;
  }

  if (!jtagFunc)
  {
    string message = string("DoAction: Got not supported command name \"").append(local_commandName).append("\"");
    throw std::runtime_error(message);
  }


  ON_FIRST_INVOCATION_BEGIN();
      CONNECT();                // make sure device is connected if debugging a single test suite
      ostringstream count_stream;
      string pattern_name=PATTERN_BASE_NAME;

      count_stream<<pattern_count++;
      pattern_name += count_stream.str();
      std::cout << "Calling RDI Wrapper for pattern "<<pattern_name<<endl;
      jtagFunc(toSutScanVector, bitsCount, pattern_name);

  ON_FIRST_INVOCATION_END();
  DISCONNECT(); // make sure device is disconnected if debugging a single test suite

  //+ (JFC March/15/2017): How to extract stuff from ARRAY_LL into fromSutScanVector ?
//  ARRAY_LL dataFromSut = rdi.id(commandName).getReadData();


  /*Make a loopback for debug purposes*/
//  fromSutScanVector = toSutScanVector;

  return make_pair(bitsCount, toSutScanVector);
}


//! Always does a synchronous reset
//!
//! @param doSynchronousReset  Ignored (reset is always synchronous)
//!
//! @note It is based upon a JTAG interface
void Advantest_SmartRDI_Protocol::DoReset (bool /* doSynchronousReset */)
{
  jtag_synch_reset();
}



//===========================================================================
// End of Advantest_SmartRDI_Protocol.cpp
//===========================================================================
