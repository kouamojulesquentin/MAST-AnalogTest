//===========================================================================
//                           Spy_SVF_Protocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_SVF_Protocol.cpp
//!
//! Implements class Spy_SVF_Protocol
//!
//===========================================================================

#include "Spy_SVF_Protocol.hpp"

using namespace mast;
using namespace test;


//! Spies content how binary vector to SUT is transformed to SVF command while returning the BinaryVector unchanged
//!
BinaryVector Spy_SVF_Protocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto command = CreateSVFCommand(derivationId, toSutData);
  m_commands.emplace_back(command);
  return toSutData;
}
//
//  End of: Spy_SVF_Protocol::DoAction
//---------------------------------------------------------------------------




//===========================================================================
// End of Spy_SVF_Protocol.cpp
//===========================================================================
