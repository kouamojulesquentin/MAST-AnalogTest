//===========================================================================
//                           UT_Emulation_TranslatorProtocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Emulation_TranslatorProtocol.cpp
//!
//! Implements test fixture for testing Emulation_TranslatorProtocol
//!
//===========================================================================

#include "UT_Emulation_TranslatorProtocol.hpp"
#include "Emulation_TranslatorProtocol.hpp"
#include "Spy_Emulation_Translator.hpp"
#include "CallbackIds.hpp"

#include "BinaryVector_Traits.hpp"
#include <tuple>
#include <vector>
#include <string>
#include <experimental/string_view>
#include <initializer_list>

using std::make_tuple;
using std::experimental::string_view;
using std::string;
using std::vector;
using std::initializer_list;
using std::make_shared;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;
using namespace test;

//! Checks Emulation_Translator constructor 
//!
void UT_Emulation_TranslatorProtocol::test_Constructor ()
{
  // ---------------- Setup
  //

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (Emulation_TranslatorProtocol sut())
  Emulation_TranslatorProtocol sut;

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.KindName(), "Emulation_Translator");
}

//! Checks Emulation_Translator TransformationCallback
//! NB: Requests are manually formed, only formatted_Data is used. The value of toSurVector is useless 
//! SVF commands are not semantically correct, we just check they are correctly formed

void UT_Emulation_TranslatorProtocol::test_TransformationCallback ()
{
  // ---------------- DDT Setup
  //

  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto        CallbackId    = std::get<0> (data);
    auto        toSutVector     = BinaryVector::CreateFromString(std::get<1> (data));
    auto        formatted_Data     = std::get<2> (data);
    const auto& expectedCommand = std::get<3> (data);
    CallbackRequest  test;

    Spy_Emulation_Translator sut;

    if (strlen(formatted_Data)>0)
      test = *( new CallbackRequest(CallbackId,toSutVector,formatted_Data));
    else
     test = *(new CallbackRequest(CallbackId,toSutVector));
    //Check Callback
    // ---------------- Exercise
    //
    auto fromSutVector = sut.TransformationCallback(test);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (toSutVector, fromSutVector); // It is a loopback

    const auto& gotCommands = sut.Commands();
    TS_ASSERT_EQUALS (gotCommands, expectedCommand);
  };

  auto data =
  {
    make_tuple(TRST, "", "",
               vector<std::string> (
               {
                 TRST" ", //Test adds a space
               })),
    make_tuple(SIR, "/b01", "2 TDI(01);",
               vector<std::string> (
               {
                 SIR" 2 TDI(01);",
               })),
    make_tuple(SDR, "/b1001", "2 TDI(01);", //NB: Binary vector unused
               vector<std::string> (
               {
                 SDR " 2 TDI(01);",
               })),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks Emulation_Translator TransformationCallback from SVF Raw Player to have correctly formed SVF commands
//!
void UT_Emulation_TranslatorProtocol::test_SVF_RawPlayer_TransformationCallback ()
{
  // ---------------- DDT Setup
  //

  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto        n_Callback    = std::get<0> (data);
    auto        toSutVector     = BinaryVector::CreateFromString(std::get<1> (data));
    const auto& expectedCommand = std::get<2> (data);
    CallbackRequest  test;
     auto Interface = make_shared<AccessInterfaceTranslator> ("Test");
     TS_ASSERT_THROWS_NOTHING (SVF_RawPlayer player());
    SVF_RawPlayer player;
    player.SetParentInterface(Interface);

    string CallbackId;
   switch (n_Callback) {
     case 0u:
      CallbackId=TRST;
    case 1u:
     CallbackId= SIR;
      break;
    case 2u:
      CallbackId= SDR;
      break;
    default:
     CallbackId= UNDEFINED;
      break;
   }


    Spy_Emulation_Translator sut;


    // ---------------- Exercise
    //
    //Exploit RawPlayer to get request
    if (n_Callback != 0)  
       Interface->PushfromSut(toSutVector); //Avoid stall if not reset
    
    player.DoCallback(n_Callback,nullptr,toSutVector);
    test = Interface->PopRequest();

    auto fromSutVector = sut.TransformationCallback(test);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (toSutVector, fromSutVector); // It is a loopback

    const auto& gotCommands = sut.Commands();
    TS_ASSERT_EQUALS (gotCommands, expectedCommand);
  };

  auto data =
  {
    make_tuple(0u, "",
               vector<std::string> (
               {
                 TRST" ", //Test adds a space
               })),
    make_tuple(1u, "/b01",
               vector<std::string> (
               {
                 SIR" 2 TDI(01);",
               })),
    make_tuple(2u, "/b101",
               vector<std::string> (
               {
                 SDR " 3 TDI(05);",
               })),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//===========================================================================
// End of UT_Emulation_TranslatorProtocol.cpp
//===========================================================================
