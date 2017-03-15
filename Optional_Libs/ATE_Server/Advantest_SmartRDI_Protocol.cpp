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


//! Constructor does nothing
Advantest_SmartRDI_Protocol::Advantest_SmartRDI_Protocol ()
{
}



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
  typedef void (*JtagFunction_t)(int, const string&);
  uint8_t bytesCount = toSutScanVector.size();

  if ((bitsCount == 0) || (bytesCount == 0))
  {
    throw std::runtime_error("DoAction: Invalid due to no bits to send to SUT");
  }

  if (((bitsCount + 7u) / 8u) != bytesCount)
  {
    throw std::runtime_error("DoAction: Invalid due to incoherence between bits count and data vector bytes count");
  }

  if ((bitsCount %32u) != 0)
  {
    throw std::runtime_error("DoAction: Do not support yet bits count not a multiple of 32");
  }

  JtagFunction_t jtagFunc = NULL;
  if      (commandName == "SIR")
  {
    jtagFunc = jtag_access_ir;
  }
  else if (commandName == "SDR")
  {
    jtagFunc = jtag_access_dr;
  }

  if (!jtagFunc)
  {
    string message = string("DoAction: Got not supported command name \"").append(commandName).append("\"");
    throw std::runtime_error(message);
  }

  RDI_INIT();

  ON_FIRST_INVOCATION_BEGIN();
      CONNECT();                // make sure device is connected if debugging a single test suite
      RDI_BEGIN();

      //! @todo [JFC]-[March/15/2017]: In DoAction(): Consider refactoring that to specialize versions of
      //!                              jtag_access_xx for vector of data
      //!
      uint32_t        remainingBitsCount = bitsCount;
      vector<uint8_t> fromSutScanVector;
      uint32_t        byteOffset         = toSutScanVector.size() - 1u;

      // ---------------- Send data with rightmost first
      //
      while (remainingBitsCount != 0)
      {
        uint8_t byte_0 = toSutScanVector[byteOffset--];
        uint8_t byte_1 = toSutScanVector[byteOffset--];
        uint8_t byte_2 = toSutScanVector[byteOffset--];
        uint8_t byte_3 = toSutScanVector[byteOffset--];

        //! @todo [JFC]-[March/15/2017]: Byte order must be checked !
        //!
        uint32_t data =   (byte_3 << 24)
                        + (byte_2 << 16)
                        + (byte_1 << 8)
                        +  byte_0;

        jtagFunc(data, commandName);

        remainingBitsCount = (remainingBitsCount >= 32) ? remainingBitsCount - 32u : 0u;
      }

      RDI_END();
  ON_FIRST_INVOCATION_END();

  //+ (JFC March/15/2017): How to extract stuff from ARRAY_LL into fromSutScanVector ?
  ARRAY_LL dataFromSut = rdi.id(commandName).getReadData();


  DISCONNECT(); // make sure device is disconnected if debugging a single test suite

  return make_pair(bitsCount, fromSutScanVector);
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
