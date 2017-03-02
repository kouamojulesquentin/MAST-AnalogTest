//===========================================================================
//                           Remote_Loopback_Protocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Loopback_Protocol.cpp
//!
//! Implements class Remote_Loopback_Protocol
//!
//===========================================================================

#include "Remote_Loopback_Protocol.hpp"

using std::string;
using std::vector;
using std::pair;

using std::make_pair;
using namespace mast;

//! Destructor does nothing
Remote_Loopback_Protocol::~Remote_Loopback_Protocol ()
{
}


//! Constructor does nothing
Remote_Loopback_Protocol::Remote_Loopback_Protocol ()
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
Remote_Loopback_Protocol::DoAction (const string&          /* commandName */,
                                    uint32_t               bitsCount,
                                    const vector<uint8_t>& toSutScanVector)
{
  return make_pair(bitsCount, toSutScanVector);
}


//! Does nothing
void Remote_Loopback_Protocol::DoReset (bool /* doSynchronousReset */)
{
}



//===========================================================================
// End of Remote_Loopback_Protocol.cpp
//===========================================================================
