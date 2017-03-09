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
Advantest_SmartRDI_Protocol::DoAction (const string&          /* commandName */,
                                    uint32_t               bitsCount,
                                    const vector<uint8_t>& toSutScanVector)
{
  return make_pair(bitsCount, toSutScanVector);
}


//! Does nothing
void Advantest_SmartRDI_Protocol::DoReset (bool /* doSynchronousReset */)
{
}



//===========================================================================
// End of Advantest_SmartRDI_Protocol.cpp
//===========================================================================
