//===========================================================================
//                           UT_Remote_Loopback_Protocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Remote_Loopback_Protocol.cpp
//!
//! Implements test fixture for testing Remote_Loopback_Protocol
//!
//===========================================================================

#include "UT_Remote_Loopback_Protocol.hpp"
#include "Remote_Loopback_Protocol.hpp"

#include <string>
#include <vector>
#include <cxxtest/ValueTraits.h>

using std::string;
using std::vector;

using mast::Remote_Protocol;
using mast::Remote_Loopback_Protocol;



//! Checks Remote_Loopback_Protocol constructor
//!
void UT_Remote_Loopback_Protocol::test_Constructor ()
{
  TS_ASSERT_THROWS_NOTHING (Remote_Loopback_Protocol sut);
}


//! Checks Remote_Loopback_Protocol::DoReset()
//!
void UT_Remote_Loopback_Protocol::test_DoReset ()
{
  // ---------------- Setup
  //
  Remote_Loopback_Protocol sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoReset(true));
}

//! Checks Remote_Loopback_Protocol::DoCallback()
//!
void UT_Remote_Loopback_Protocol::test_DoCallback ()
{
  // ---------------- Setup
  //
  string          commandName("SIR");
  uint32_t        bitsCount = 11;
  vector<uint8_t> toSutData{0x01, 0xC3, 0xFA};

  Remote_Protocol::DoCallbackReturn_t doActionResult;

  Remote_Loopback_Protocol sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (doActionResult = sut.DoCallback(commandName, bitsCount, toSutData));

  // ---------------- Verify
  //
  const auto& fromSutBitscount = doActionResult.first;
  const auto& fromSutData      = doActionResult.second;

  TS_ASSERT_EQUALS (fromSutBitscount, bitsCount);
  TS_ASSERT_EQUALS (fromSutData,      toSutData);
}



//===========================================================================
// End of UT_Remote_Loopback_Protocol.cpp
//===========================================================================
