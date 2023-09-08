//===========================================================================
//                           Spy_AccessInterfaceProtocols.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_AccessInterfaceProtocols.cpp
//!
//! Implements class Spy_AccessInterfaceProtocols
//!
//===========================================================================

#include "Spy_AccessInterfaceProtocols.hpp"

using namespace mast;
using namespace test;


//! Spies content of parameter toSutData and return it unchanged
BinaryVector Spy_AccessInterfaceProtocols::DoCallback (std::string /*CallbackId*/, uint32_t /* endpointId */, void* /* interfaceData */, const BinaryVector& toSutData)
{
  m_toSutVectors.emplace_back(toSutData);
  return toSutData;
}
//
//  End of: Spy_AccessInterfaceProtocols::DoCallback
//---------------------------------------------------------------------------



//! Tally number of call to Reset
//!
void Spy_AccessInterfaceProtocols::DoReset (bool /* doSynchronousReset */)
{
  ++m_resetCount;
}
//
//  End of: Spy_AccessInterfaceProtocols::Reset
//---------------------------------------------------------------------------




//===========================================================================
// End of Spy_AccessInterfaceProtocols.cpp
//===========================================================================
