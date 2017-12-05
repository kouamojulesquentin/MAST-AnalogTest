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
#include "Spy_Emulation_Translator.hpp"

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
  TS_ASSERT_FALSE(sut.ParentTranslator_is_set())
  //checks that it is recognized as a Raw protocol
  
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
  TS_ASSERT_EQUALS(sut.CallbackId(0),TRST);
  TS_ASSERT_EQUALS(sut.CallbackId(1),SIR);
  TS_ASSERT_EQUALS(sut.CallbackId(2),SDR);
  TS_ASSERT_EQUALS(sut.CallbackId(3),UNDEFINED);
}

//! Checks SVF_RawPlayer reset request
//!
void UT_SVF_RawPlayer::test_doReset ()
{
  // ---------------- Setup
  //
 auto Interface = make_shared<AccessInterfaceTranslator> ("Test",make_shared<Spy_Emulation_Translator>());
  TS_ASSERT_THROWS_NOTHING (SVF_RawPlayer sut());
   auto sut=make_shared<SVF_RawPlayer>();
   auto ai = make_shared<AccessInterface> ("dummy",sut);
   Interface->RegisterInterface(ai);

  // ---------------- Exercise & Verify
  //
  //Put a result value to avoid callback getting stuck on result queue
  auto  test = BinaryVector::CreateFromBinaryString("01");
  
  //Direct call to reset
  Interface->PushfromSut(test,0);
 sut->DoReset(true);
 auto result = Interface->PopRequest(0);
  TS_ASSERT_EQUALS(result.CallbackId(),TRST);

}


//! Checks SVF_RawPlayer callback requests
//!
void UT_SVF_RawPlayer::test_Callbacks ()
{
  // ---------------- Setup
  //
 auto Interface = make_shared<AccessInterfaceTranslator> ("Test",make_shared<Spy_Emulation_Translator>());
  TS_ASSERT_THROWS_NOTHING (SVF_RawPlayer sut());
     auto sut=make_shared<SVF_RawPlayer>();
   auto ai = make_shared<AccessInterface> ("dummy",sut);
   Interface->RegisterInterface(ai);

  // ---------------- Exercise & Verify
  //
  //Put a result value to avoid callback getting stuck on result queue
  auto  test = BinaryVector::CreateFromBinaryString("01");

  //Reset as Callback 0 : does not need a fromSUT value
  Interface->PushfromSut(test,0);
 sut->DoCallback(0,nullptr,test);
 auto result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),TRST);
 TS_ASSERT_EQUALS(result.FormattedData(),""); 

  //Callback 1 : SIR
  Interface->PushfromSut(test,0);
 sut->DoCallback(1,nullptr,test);
 result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),SIR);
 TS_ASSERT_EQUALS(result.FormattedData(),"2 TDI(01);");

  //Callback 2 : SDR
  Interface->PushfromSut(test,0);
 sut->DoCallback(2,nullptr,test);
 result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),SDR);
 TS_ASSERT_EQUALS(result.FormattedData(),"2 TDI(01);");

  //Callback 3 : UNDEFINED
  Interface->PushfromSut(test,0);
 sut->DoCallback(3,nullptr,test);
 result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),UNDEFINED);

}

//! Checks SVF_RawPlayer callback requests as multithread implementation
//!
void UT_SVF_RawPlayer::test_Callbacks_multithread ()
{
  // ---------------- Setup
  //
 auto Interface = make_shared<AccessInterfaceTranslator> ("Test",make_shared<Spy_Emulation_Translator>());
  TS_ASSERT_THROWS_NOTHING (SVF_RawPlayer sut());
   auto sut=make_shared<SVF_RawPlayer>();
     auto ai = make_shared<AccessInterface> ("dummy",sut);
   Interface->RegisterInterface(ai);


  // ---------------- Exercise & Verify
  //
  //Put a result value to avoid callback getting stuck on result queue
  auto  test = BinaryVector::CreateFromBinaryString("01");

 auto callback_thread = [this] (std::shared_ptr<SVF_RawPlayer> sut,int EndpointId,BinaryVector  test) 
   { sut->DoCallback(EndpointId,nullptr,test);};

  //Reset as Callback 0 :
 Interface->PushfromSut(test,0);
    std::thread AI_thread(callback_thread,sut,0,test);
 auto result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),TRST);
 AI_thread.join();

  //Callback 1 : SIR
  Interface->PushfromSut(test,0);
 std::thread AI_thread_1(callback_thread,sut,1,test);
 result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),SIR);
 AI_thread_1.join();

  //Callback 2 : SDR
  Interface->PushfromSut(test,0);
 std::thread AI_thread_2(callback_thread,sut,2,test);
 result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),SDR);
 AI_thread_2.join();

  //Callback 3 : UNDEFINED
  Interface->PushfromSut(test,0);
 std::thread AI_thread_3(callback_thread,sut,3,test);
 result = Interface->PopRequest(0);
 TS_ASSERT_EQUALS(result.CallbackId(),UNDEFINED);
 AI_thread_3.join();

}

//===========================================================================
// End of UT_SVF_RawPlayer.cpp
//===========================================================================
