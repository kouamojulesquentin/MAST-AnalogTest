//===========================================================================
//                           UT_Streamer.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Streamer.cpp
//!
//! Implements test fixture for testing Streamer
//!
//===========================================================================


#include "UT_Streamer.hpp"
#include "Streamer.hpp"
#include "BinaryVector_Traits.hpp"

#include <tuple>

using std::tuple;
using std::make_tuple;

using namespace mast;

//! Checks Streamer:: constructor
//!
//! @note SystemModelNode parts have been test elsewhere
void UT_Streamer::test_Constructor ()
{
  // ---------------- Setup
  //
  auto name           = "A name";

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING(Streamer sut(name));

}

void UT_Streamer::test_Getters ()
{
  // ---------------- Setup
  //
  auto name           = "A name";
  auto protocolKind   = "TransparentStreamer";

  // ---------------- Exercise
  //
  Streamer sut(name);
  TS_ASSERT_EQUALS(protocolKind,sut.Protocol()->KindName());
  
  auto mask = sut.Protocol()->CurrentMask();
  TS_ASSERT_EQUALS(mask.BitsCount(),0);
  

}

//===========================================================================
void UT_Streamer::test_Setters ()
{
  // ---------------- Setup
  //
  auto name           = "A name";
  auto protocolKind   = "TransparentStreamer";

  // ---------------- Exercise
  //
  Streamer sut(name);
  TS_ASSERT_EQUALS(protocolKind,sut.Protocol()->KindName());
  
  auto mask = sut.Protocol()->NewMask(12);
  TS_ASSERT_EQUALS(mask.BitsCount(),12);
  TS_ASSERT_EQUALS(mask.DataAsBinaryString(),"0000_0000:0000");
  
  auto plain = BinaryVector::CreateFromHexString("123");
  BinaryVector result;
  
  TS_ASSERT_THROWS_NOTHING(result = sut.Protocol()->ApplyMask(plain,mask));
  TS_ASSERT_EQUALS(result,plain);

  mask = sut.Protocol()->NewMask(3);
  TS_ASSERT_THROWS(result = sut.Protocol()->ApplyMask(plain,mask),std::exception);  

}
// End of UT_Streamer.cpp
//===========================================================================
