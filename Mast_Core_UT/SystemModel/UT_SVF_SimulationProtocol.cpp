//===========================================================================
//                           UT_SVF_SimulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SVF_SimulationProtocol.cpp
//!
//! Implements test fixture for testing SVF_SimulationProtocol
//!
//===========================================================================

#include "UT_SVF_SimulationProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "Fake_SVF_Simulator.hpp"
#include "RVF.hpp"

#include "BinaryVector_Traits.hpp"
#include <tuple>
#include <experimental/string_view>

using std::make_tuple;
using std::experimental::string_view;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace mast;
using namespace test;

namespace
{
  constexpr char TO_SUT_PATH[]   = "TO_SUT_PATH";
  constexpr char FROM_SUT_PATH[] = "FROM_SUT_PATH";
} // End of unnamed namespace


//! Initializes test (called for each test)
void UT_SVF_SimulationProtocol::setUp ()
{
}

//! Checks SVF_SimulationProtocol constructor with files path
//!
void UT_SVF_SimulationProtocol::test_Constructor ()
{
  // ---------------- Setup
  //
  auto toSutPath = "TO_SUT_PATH";
  auto fromSutPath = "FROM_SUT_PATH";

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SVF_SimulationProtocol sut(toSutPath, fromSutPath));
}


//! Checks SVF_SimulationProtocol constructor when cannot open "To SUT path"
//!
void UT_SVF_SimulationProtocol::test_Constructor_Bad_ToSutPath ()
{
  // ---------------- Setup
  //
  auto toSutPath   = "BAD/PATH/TO_SUT_PATH";
  auto fromSutPath = "FROM_SUT_PATH";
  auto toSutVector = BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (SVF_SimulationProtocol(toSutPath, fromSutPath), std::exception);
}


//! Checks SVF_SimulationProtocol constructor DoCallback when cannot open "From SUT path"
//!
void UT_SVF_SimulationProtocol::test_Constructor_Bad_FromSutPath ()
{
  // ---------------- Setup
  //
  auto toSutPath   = "TO_SUT_PATH";
  auto fromSutPath = "BAD/PATH/FROM_SUT_PATH";
  auto toSutVector = BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0");

//+  sut.FromSutDataWait(1ms);     // This is required to get rapidly an exception
//+  sut.FromSutDataTimeout(2ms);  // This is required to get rapidly an exception

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (SVF_SimulationProtocol(toSutPath, fromSutPath), std::exception);
}



//! Checks SVF_SimulationProtocol::DoCallback when the exchanges are synchronous (same thread)
//!
void UT_SVF_SimulationProtocol::test_DoCallback_Sync ()
{
  // ---------------- DDT Setup
  //
  auto toSutPath     = "TO_SUT_PATH";
  auto fromSutPath   = "FROM_SUT_PATH";
  auto sut           = SVF_SimulationProtocol(toSutPath, fromSutPath);

  Fake_SVF_Simulator fakeSimulator(toSutPath, fromSutPath);

  auto checker = [&sut, &fakeSimulator](const auto& data)
  {
    // ---------------- Setup
    //
    auto channelId = std::get<0>(data);
    auto toSutData    = std::get<1>(data);

    auto toSutVector = BinaryVector::CreateFromString(toSutData);
    auto fromSutData = toSutVector.DataAsBinaryString("", "");
    auto Primitive     = "Primitive"s;
    RVFRequest Test(Primitive, toSutVector, nullptr);

    fakeSimulator.Produce(fromSutData);

    // ---------------- Exercise
    //
    auto fromSutVector = sut.DoCallback(Test,channelId);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (fromSutVector, toSutVector);
  };

  auto data =
  {
    make_tuple(1, "/x01"),                 // 00
    make_tuple(2, "/x0505/b10"),           // 01
    make_tuple(2, "/x3636/b0:100"),        // 02
    make_tuple(2, "/x3636/b1/xC0C0_C0C0"), // 03
    make_tuple(2, "/x3636/b0:011"),        // 04
    make_tuple(2, "/x3636/b1/xB0B0_B0B0"), // 05
    make_tuple(2, "/x3636/b0:010"),        // 06
    make_tuple(2, "/x3636/b1/x0A0A_0A0A"), // 07
    make_tuple(2, "/x3636/b0:001"),        // 08
    make_tuple(2, "/x3636/b0/x0909_0909"), // 09
    make_tuple(2, "/x3636/b1/x4040_4040"), // 10
    make_tuple(2, "/x3636/b1:000"),        // 11
    make_tuple(2, "/x3636/b0:000"),        // 15
    make_tuple(1, "/xFF"),                 // 16
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);

  // ---------------- DDT Verify
  //
  // To SUT command file:
  auto expectedToSutFileContent =
   "SIR 8 TDI(01);\n"                   // 00
   "SDR 18 TDI(001416);\n"              // 01
   "SDR 20 TDI(036364);\n"              // 02
   "SDR 49 TDI(006C6DC0C0C0C0);\n"      // 03
   "SDR 20 TDI(036363);\n"              // 04
   "SDR 49 TDI(006C6DB0B0B0B0);\n"      // 05
   "SDR 20 TDI(036362);\n"              // 06
   "SDR 49 TDI(006C6D0A0A0A0A);\n"      // 07
   "SDR 20 TDI(036361);\n"              // 08
   "SDR 49 TDI(006C6C09090909);\n"      // 09
   "SDR 49 TDI(006C6D40404040);\n"      // 10
   "SDR 20 TDI(036368);\n"              // 11
   "SDR 20 TDI(036360);\n"              // 15
   "SIR 8 TDI(FF);\n";                  // 16

  TS_ASSERT_FILE_CONTENT (toSutPath, expectedToSutFileContent);

  // From SUT data file:
  auto expectedFromSutFileContent =
    "00000001\n"                                          // 00
    "000001010000010110\n"                                // 01
    "00110110001101100100\n"                              // 02
    "0011011000110110111000000110000001100000011000000\n" // 03
    "00110110001101100011\n"                              // 04
    "0011011000110110110110000101100001011000010110000\n" // 05
    "00110110001101100010\n"                              // 06
    "0011011000110110100001010000010100000101000001010\n" // 07
    "00110110001101100001\n"                              // 08
    "0011011000110110000001001000010010000100100001001\n" // 09
    "0011011000110110101000000010000000100000001000000\n" // 10
    "00110110001101101000\n"                              // 11
    "00110110001101100000\n"                              // 12
    "11111111\n";                                         // 13

  TS_ASSERT_FILE_CONTENT (fromSutPath, expectedFromSutFileContent);
}


//! Checks SVF_SimulationProtocol::DoCallback when the exchanges are asynchronous (different thread)
//!
void UT_SVF_SimulationProtocol::test_DoCallback_ASync ()
{
  // ---------------- DDT Setup
  //
  auto toSutPath   = "TO_SUT_PATH";
  auto fromSutPath = "FROM_SUT_PATH";
  auto sut         = SVF_SimulationProtocol(toSutPath, fromSutPath);

  Fake_SVF_Simulator fakeSimulator(toSutPath, fromSutPath);
  fakeSimulator.Start();  // Asynchronously make a loopback from SVF command to binary bitstream

  auto checker = [&sut, &fakeSimulator](const auto& data)
  {
    // ---------------- Setup
    //
    auto channelId = std::get<0>(data);
    auto toSutData    = std::get<1>(data);

    auto toSutVector = BinaryVector::CreateFromString(toSutData);
    auto fromSutData = toSutVector.DataAsBinaryString("", "");
    auto Primitive     = "Primitive"s;
    RVFRequest Test(Primitive, toSutVector, nullptr);

    // ---------------- Exercise
    //
    auto fromSutVector = sut.DoCallback(Test,channelId);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (fromSutVector, toSutVector);
  };

  auto data =
  {
    make_tuple(1, "/x01"),                 // 00
    make_tuple(2, "/x0505/b10"),           // 01
    make_tuple(2, "/x3636/b0:100"),        // 02
    make_tuple(2, "/x3636/b1/xC0C0_C0C0"), // 03
    make_tuple(2, "/x3636/b0:011"),        // 04
    make_tuple(2, "/x3636/b1/xB0B0_B0B0"), // 05
    make_tuple(2, "/x3636/b0:010"),        // 06
    make_tuple(2, "/x3636/b1/x0A0A_0A0A"), // 07
    make_tuple(2, "/x3636/b0:001"),        // 08
    make_tuple(2, "/x3636/b0/x0909_0909"), // 09
    make_tuple(2, "/x3636/b1/x4040_4040"), // 10
    make_tuple(2, "/x3636/b1:000"),        // 11
    make_tuple(2, "/x3636/b0:000"),        // 15
    make_tuple(1, "/xFF"),                 // 16
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);

  // ---------------- DDT Verify
  //
  fakeSimulator.Stop();

  // To SUT command file:
  auto expectedToSutFileContent =
   "SIR 8 TDI(01);\n"                   // 00
   "SDR 18 TDI(001416);\n"              // 01
   "SDR 20 TDI(036364);\n"              // 02
   "SDR 49 TDI(006C6DC0C0C0C0);\n"      // 03
   "SDR 20 TDI(036363);\n"              // 04
   "SDR 49 TDI(006C6DB0B0B0B0);\n"      // 05
   "SDR 20 TDI(036362);\n"              // 06
   "SDR 49 TDI(006C6D0A0A0A0A);\n"      // 07
   "SDR 20 TDI(036361);\n"              // 08
   "SDR 49 TDI(006C6C09090909);\n"      // 09
   "SDR 49 TDI(006C6D40404040);\n"      // 10
   "SDR 20 TDI(036368);\n"              // 11
   "SDR 20 TDI(036360);\n"              // 15
   "SIR 8 TDI(FF);\n";                  // 16

  TS_ASSERT_FILE_CONTENT (toSutPath, expectedToSutFileContent);

  // From SUT data file:
  auto expectedFromSutFileContent =
    "00000001\n"                                          // 00
    "000001010000010110\n"                                // 01
    "00110110001101100100\n"                              // 02
    "0011011000110110111000000110000001100000011000000\n" // 03
    "00110110001101100011\n"                              // 04
    "0011011000110110110110000101100001011000010110000\n" // 05
    "00110110001101100010\n"                              // 06
    "0011011000110110100001010000010100000101000001010\n" // 07
    "00110110001101100001\n"                              // 08
    "0011011000110110000001001000010010000100100001001\n" // 09
    "0011011000110110101000000010000000100000001000000\n" // 10
    "00110110001101101000\n"                              // 11
    "00110110001101100000\n"                              // 12
    "11111111\n";                                         // 13

  TS_ASSERT_FILE_CONTENT (fromSutPath, expectedFromSutFileContent);
}


//! Checks SVF_SimulationProtocol::Reset
//!
void UT_SVF_SimulationProtocol::test_DoReset ()
{
  // ---------------- DDT Setup
  //
  auto sut = SVF_SimulationProtocol(TO_SUT_PATH, FROM_SUT_PATH);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoReset(false));

  auto expectedToSutFileContent = "STATE RESET;\n";

  TS_ASSERT_FILE_CONTENT (TO_SUT_PATH, expectedToSutFileContent);
}


//! Checks SVF_SimulationProtocol::Reset when protocol support TRST
//!
void UT_SVF_SimulationProtocol::test_DoReset_SupportTRST ()
{
  // ---------------- DDT Setup
  //
  auto sut = SVF_SimulationProtocol(TO_SUT_PATH, FROM_SUT_PATH);
  sut.SupportTRST(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoReset(false));

  auto expectedToSutFileContent = "TRST ON;\nTRST OFF;\n";

  TS_ASSERT_FILE_CONTENT (TO_SUT_PATH, expectedToSutFileContent);
}


//! Checks SVF_SimulationProtocol::Reset when protocol support TRST but a 'sync' mode reset is requested
//!
void UT_SVF_SimulationProtocol::test_DoReset_Sync ()
{
  // ---------------- DDT Setup
  //
  auto sut = SVF_SimulationProtocol(TO_SUT_PATH, FROM_SUT_PATH);
  sut.SupportTRST(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoReset(true));

  auto expectedToSutFileContent = "STATE RESET;\n";

  TS_ASSERT_FILE_CONTENT (TO_SUT_PATH, expectedToSutFileContent);
}

//===========================================================================
// End of UT_SVF_SimulationProtocol.cpp
//===========================================================================
