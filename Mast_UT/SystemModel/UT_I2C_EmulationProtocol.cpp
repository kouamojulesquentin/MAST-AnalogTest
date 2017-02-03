//===========================================================================
//                           UT_I2C_EmulationProtocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_I2C_EmulationProtocol.cpp
//!
//! Implements test fixture for testing I2C_EmulationProtocol
//!
//===========================================================================

#include "UT_I2C_EmulationProtocol.hpp"
#include "I2C_EmulationProtocol.hpp"
#include "Spy_I2C_Protocol.hpp"

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

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;
using namespace test;


//! Checks I2C_EmulationProtocol constructor given registers addresses as integral values
//!
void UT_I2C_EmulationProtocol::test_Constructor_Integer ()
{
  // ---------------- Setup
  //
  auto i2C_Adresses = initializer_list<uint32_t>{ 0x30u, 0x31u };
  auto prefix       = "I2C"s;

  // ---------------- Exercise
  //
  I2C_EmulationProtocol sut(i2C_Adresses, prefix);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.CommandsPrefix(), prefix);
}


//! Checks I2C_EmulationProtocol constructor given registers addresses as integral values but with some errors
//!
void UT_I2C_EmulationProtocol::test_Constructor_Integer_Error ()
{
  // ---------------- Setup
  //
  auto i2C_Adresses = initializer_list<uint32_t>{ 0x30u};
  auto prefix       = "I2C"s;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (I2C_EmulationProtocol sut(i2C_Adresses, prefix), std::exception);
}




void UT_I2C_EmulationProtocol::test_MaxSupportedDerivations ()
{
  // ---------------- Setup
  //
  I2C_EmulationProtocol sut({ 0x30u, 0x31u }, "[I2C]");

  // ---------------- Exercise
  //
  auto maxSupportedDerivations = sut.MaxSupportedDerivations();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (maxSupportedDerivations, 2u);
}

//! Checks I2C_EmulationProtocol constructor given max derivation as a string
//!
void UT_I2C_EmulationProtocol::test_Constructor_String ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto parameters             = string(std::get<0>(data));
    auto expectedMaxDerivations = std::get<1>(data);

    // ---------------- Exercise
    //
    auto sut = I2C_EmulationProtocol(parameters);

    // ---------------- Verify
    //
    auto maxSupportedDerivations = sut.MaxSupportedDerivations();
    TS_ASSERT_EQUALS (maxSupportedDerivations, expectedMaxDerivations);
  };

  auto data =
  {
    make_tuple("0x30, 0x31"sv,                    2u), // 0
    make_tuple("0x30, 0x31, [I2C]"sv,             2u), // 1
    make_tuple("0x30, 0x31, 0x32, [I2C]"sv,       3u), // 2
    make_tuple("0xFFFFFFFE, 0xFFFFFFFF, [i2c]"sv, 2u), // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks I2C_EmulationProtocol constructor given max derivation as a string but with not valid numbers
//!
void UT_I2C_EmulationProtocol::test_Constructor_String_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto parameters)
  {
    string strParameters(parameters);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (auto sut = I2C_EmulationProtocol(strParameters), std::exception);
  };

  auto data =
  {
    ""sv,                         // 0: No number at all
    "   "sv,                      // 1: No number at all
    "0x30, i2c"sv,                // 2: Should provide at least 2 addresses
    "0x30, [I2C], 0x31"sv,        // 3: Prefix is misplaced
    "0x30 0x31 0x32, [I2C]"sv,    // 4: Missing separator
    "0x30, 0xFFFFFFFF1, [i2c]"sv, // 5: Too big value
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks I2C_EmulationProtocol DoAction
//!
void UT_I2C_EmulationProtocol::test_DoAction ()
{
  // ---------------- DDT Setup
  //

  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto        derivationId    = std::get<0> (data);
    auto        toSutVector     = BinaryVector::CreateFromString(std::get<1> (data));
    const auto& expectedCommand = std::get<2> (data);

    auto addresses = { 0x00u, 0x41u, 0x42u };
    Spy_I2C_Protocol sut(addresses, "S2R "s);

    // ---------------- Exercise
    //
    auto fromSutVector = sut.DoAction(derivationId, nullptr, toSutVector);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (toSutVector, fromSutVector); // It is a loopback

    const auto& gotI2cCommands = sut.I2CCommands();
    TS_ASSERT_EQUALS (gotI2cCommands, expectedCommand);
  };

  auto data =
  {
    make_tuple(1, "/b01",
               vector<std::string> (
               {
                 "S2R I2C_READ(0x41)",
                 "S2R I2C_WRITE(0x41, 0b01)",
               })),

    make_tuple(2, "/b101",
               vector<std::string> (
               {
                 "S2R I2C_READ(0x42)",
                 "S2R I2C_WRITE(0x42, 0b101)",
               })),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks I2C_EmulationProtocol DoAction when having been constructed from string
//!
void UT_I2C_EmulationProtocol::test_DoAction_String()
{
  // ---------------- DDT Setup
  //

  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto        derivationId    = std::get<0> (data);
    auto        toSutVector     = BinaryVector::CreateFromString(std::get<1> (data));
    const auto& expectedCommand = std::get<2> (data);

    Spy_I2C_Protocol sut("0x00u, 0x41u, 0x42u,S2R "s);

    // ---------------- Exercise
    //
    auto fromSutVector = sut.DoAction(derivationId, nullptr, toSutVector);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (toSutVector, fromSutVector); // It is a loopback

    const auto& gotI2cCommands = sut.I2CCommands();
    TS_ASSERT_EQUALS (gotI2cCommands, expectedCommand);
  };

  auto data =
  {
    make_tuple(1, "/b01",
               vector<std::string> (
               {
                 "S2R I2C_READ(0x41)",
                 "S2R I2C_WRITE(0x41, 0b01)",
               })),

    make_tuple(2, "/b101",
               vector<std::string> (
               {
                 "S2R I2C_READ(0x42)",
                 "S2R I2C_WRITE(0x42, 0b101)",
               })),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//===========================================================================
// End of UT_I2C_EmulationProtocol.cpp
//===========================================================================
