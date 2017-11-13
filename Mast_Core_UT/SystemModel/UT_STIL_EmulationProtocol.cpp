//===========================================================================
//                           UT_SVF_SimulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_STIL_EmulationProtocol.cpp
//!
//! Implements test fixture for testing STIL_EmulationProtocol
//!
//===========================================================================

#include "UT_STIL_EmulationProtocol.hpp"
#include "STIL_EmulationProtocol.hpp"
#include "Spy_STIL_Protocol.hpp"

#include "BinaryVector_Traits.hpp"
#include <tuple>
#include <vector>
#include <string>
#include <experimental/string_view>

using std::make_tuple;
using std::experimental::string_view;
using std::string;
using std::vector;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;
using namespace test;


//! Checks STIL_EmulationProtocol constructor given max endpoint as an integer
//!
void UT_STIL_EmulationProtocol::test_Constructor_Integer ()
{
  // ---------------- Setup
  //
  auto n_chains = 3u;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (STIL_EmulationProtocol sut(n_chains));
}




void UT_STIL_EmulationProtocol::test_MaxSupportedEndPoints ()
{
  // ---------------- Setup
  //
  STIL_EmulationProtocol sut(3u);

  // ---------------- Exercise
  //
  auto maxSupportedEndPoints = sut.MaxSupportedEndPoints();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (maxSupportedEndPoints, 3u);
}

//! Checks STIL_EmulationProtocol constructor given max endpoint as a string
//!
void UT_STIL_EmulationProtocol::test_Constructor_String ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto maxEndPointsString   = string(std::get<0>(data));
    auto expectedMaxEndPoints = std::get<1>(data);

    // ---------------- Exercise
    //
    auto sut = STIL_EmulationProtocol(maxEndPointsString);

    // ---------------- Verify
    //
    auto maxSupportedEndPoints = sut.MaxSupportedEndPoints();
    TS_ASSERT_EQUALS (maxSupportedEndPoints, expectedMaxEndPoints);
  };

  auto data =
  {
    make_tuple("0"sv,          0u),          // 0
    make_tuple("1"sv,          1u),          // 1
    make_tuple("10"sv,         10u),         // 2
    make_tuple(" 11"sv,        11u),         // 3
    make_tuple("12 "sv,        12u),         // 4
    make_tuple(" 13 "sv,       13u),         // 5
    make_tuple("017"sv,        017u),        // 6
    make_tuple("0x17"sv,       0x17u),       // 7
    make_tuple("0xFFFFFFFF"sv, 0xFFFFFFFFu), // 8
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks STIL_EmulationProtocol constructor given max endpoint as a string but with not valid numbers
//!
void UT_STIL_EmulationProtocol::test_Constructor_String_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view data)
  {
    // ---------------- Setup
    //
    string maxSupportedEndPoints(data);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (auto sut = STIL_EmulationProtocol(maxSupportedEndPoints), std::exception);
  };

  auto data =
  {
    ""sv,            // 0: No number at all
    "   "sv,         // 1: No number at all
    "Five"sv,        // 2: Not a number
    "NaN"sv,         // 3: Not a number
    ":10"sv,         // 4: Start with not number character
    "O17"sv,         // 5: 'O' instead of zero
    "x17"sv,         // 6: Missing leading zero
    "0xFFFFFFFF0"sv, // 7: Too big number
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks STIL_EmulationProtocol constructor
//!
void UT_STIL_EmulationProtocol::test_SPY_Constructor()
{
  // ---------------- Setup
  //
  auto n_chains = 3u;

  // ---------------- Exercise
  //
  Spy_STIL_Protocol sut(n_chains);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.MaxSupportedEndPoints(), n_chains);
}


void UT_STIL_EmulationProtocol::test_DoCallback ()
{
  // ---------------- Setup
  //
  auto n_chains = 3;

  // ---------------- Exercise & Verify
  //
  Spy_STIL_Protocol sut(n_chains);
  TS_ASSERT_EQUALS (sut.MaxSupportedEndPoints(), n_chains);

  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto n_chain  = std::get<0> (data);
    auto toSutVector  = BinaryVector::CreateFromString(std::get<1> (data));
    auto expected = std::get<2> (data);

    sut.resetCommands();

    auto fromSutVector = sut.DoCallback(n_chain,nullptr,toSutVector);
    TS_ASSERT_EQUALS (toSutVector,fromSutVector); //It is a loopback

    auto gotSTILCommands=sut.STILCommands();
    TS_ASSERT_EQUALS (gotSTILCommands, expected);
  };

  auto data =
  {
    make_tuple(1,"/b01",vector<std::string> ({"V{ CHAIN = 001010-T}","V{ CHAIN = 0011001T}", "V{ CHAIN = 0011000T}", "V{ CHAIN = 001001-T}",})),
    make_tuple(2,"/b01",vector<std::string> ({"V{ CHAIN = 010010-T}","V{ CHAIN = 0101001T}", "V{ CHAIN = 0101000T}", "V{ CHAIN = 010001-T}",})),
  };

  // ---------------;- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//===========================================================================
// End of UT_STIL_EmulationProtocol.cpp
//===========================================================================
