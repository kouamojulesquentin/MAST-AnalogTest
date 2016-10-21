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
using namespace mast;
using namespace test;


//! Initializes test (called for each test)
void UT_STIL_EmulationProtocol::setUp ()
{
}

//! Checks STIL_EmulationProtocol constructor
//!
void UT_STIL_EmulationProtocol::test_Constructor ()
{
  // ---------------- Setup
  //
  auto n_chains = 3;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (STIL_EmulationProtocol sut(n_chains));
}

void UT_STIL_EmulationProtocol::test_MaxSupportedDerivations ()
{
  // ---------------- Setup
  //
  auto n_chains = 3;

  // ---------------- Exercise & Verify
  //
  STIL_EmulationProtocol sut(n_chains);
  TS_ASSERT_EQUALS (sut.MaxSupportedDerivations(), n_chains);
}

//! Checks STIL_EmulationProtocol constructor
//!
void UT_STIL_EmulationProtocol::test_SPY_Constructor ()
{
  // ---------------- Setup
  //
  auto n_chains = 3;

  // ---------------- Exercise & Verify
  //
  Spy_STIL_Protocol sut(n_chains);
  TS_ASSERT_EQUALS (sut.MaxSupportedDerivations(), n_chains);
}


void UT_STIL_EmulationProtocol::test_DoAction ()
{
  // ---------------- Setup
  //
  auto n_chains = 3;

  // ---------------- Exercise & Verify
  //
  Spy_STIL_Protocol sut(n_chains);
  TS_ASSERT_EQUALS (sut.MaxSupportedDerivations(), n_chains);
  
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto n_chain  = std::get<0> (data);
    auto toSutVector  = BinaryVector::CreateFromString(std::get<1> (data));
    auto expected = std::get<2> (data);
    
    sut.resetCommands();

  auto fromSutVector = sut.DoAction(n_chain,nullptr,toSutVector);
  TS_ASSERT_EQUALS (toSutVector,fromSutVector); //It is a loopback
 
  auto gotSTILCommands=sut.STILCommands();
  TS_ASSERT_EQUALS (gotSTILCommands, expected);
  };
  
  auto data = {
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
