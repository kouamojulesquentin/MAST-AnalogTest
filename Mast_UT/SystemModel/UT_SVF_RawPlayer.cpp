//===========================================================================
//                           UT_SVF_RawPlayer.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SVF_RawPlayer.cpp
//!
//! Implements test fixture for testing SVF_RawPlayer
//!
//===========================================================================

#include "UT_SVF_RawPlayer.hpp"
#include "SVF_RawPlayer.hpp"
#include "Fake_SVF_Simulator.hpp"

#include "BinaryVector_Traits.hpp"
#include <tuple>
#include <experimental/string_view>

using std::make_tuple;
using std::experimental::string_view;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace mast;
using namespace test;
using std::shared_ptr;
using std::make_shared;



//! Initializes test (called for each test)
void UT_SVF_RawPlayer::setUp ()
{
}

//! Checks SVF_RawPlayer constructor with empty parameter
//!
void UT_SVF_RawPlayer::test_empty_Constructor ()
{
  // ---------------- Setup
  //

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SVF_RawPlayer sut());
  SVF_RawPlayer sut;
  TS_ASSERT_NULLPTR(sut.ParentInterface())
  //checks that it is recognized as a Raw protocol
  
}

//! Checks SVF_RawPlayer constructor with parameter
//!
void UT_SVF_RawPlayer::test_not_empty_Constructor ()
{
  // ---------------- Setup
  //
 auto Interface = make_shared<AccessInterfaceTranslator> ("Test");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SVF_RawPlayer sut(Interface));
  SVF_RawPlayer sut(Interface);
  TS_ASSERT_EQUALS(sut.ParentInterface(),Interface) 
  auto protocol_is_raw =  dynamic_cast<AccessInterfaceRawProtocol*>(&sut);
  TS_ASSERT_TRUE(protocol_is_raw);

}

//! Checks SVF_RawPlayer can set the Parent interface
//!
void UT_SVF_RawPlayer::test_set_Interface ()
{
  // ---------------- Setup
  //
 auto Interface = make_shared<AccessInterfaceTranslator> ("Test");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SVF_RawPlayer sut());
  SVF_RawPlayer sut;
  TS_ASSERT_NULLPTR(sut.ParentInterface())
  sut.SetParentInterface(Interface);
  TS_ASSERT_EQUALS(sut.ParentInterface(),Interface)
  
}


//! Checks SVF_RawPlayer callback ids
//!
void UT_SVF_RawPlayer::test_CallbackIds ()
{
  // ---------------- Setup
  //

  // ---------------- Exercise & Verify
  //
  SVF_RawPlayer sut;
  TS_ASSERT_EQUALS(sut.MaxSupportedEndPoints(),3u);
  TS_ASSERT_EQUALS(sut.CallbackId(0),TRSRT);
  TS_ASSERT_EQUALS(sut.CallbackId(1),SIR);
  TS_ASSERT_EQUALS(sut.CallbackId(2),SDR);
  TS_ASSERT_EQUALS(sut.CallbackId(3),UNDEFINED);
}


//===========================================================================
// End of UT_SVF_RawPlayer.cpp
//===========================================================================
