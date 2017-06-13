//===========================================================================
//                           UT_PDL_AlgorithmsRepository.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PDL_AlgorithmsRepository.cpp
//!
//! Implements test fixture for testing PDL_AlgorithmsRepository
//!
//===========================================================================

#include "UT_PDL_AlgorithmsRepository.hpp"
#include "PDL_AlgorithmsRepository.hpp"
#include <cxxtest/ValueTraits.h>

using namespace mast;



//! Checks PDL_AlgorithmsRepository::Instance()
//!
void UT_PDL_AlgorithmsRepository::test_Instance ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (PDL_AlgorithmsRepository::Instance());
}



//! Checks PDL_AlgorithmsRepository::RegisterAlgorithm() with first algorithm
//!
void UT_PDL_AlgorithmsRepository::test_RegisterAlgorithm_FirstOne ()
{
  auto  started = false;
  auto  algo    = [&started](){ started = true; };
  auto& sut     = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.RegisterAlgorithm("Foo", algo));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredAlgorithmCount(), 1u);
}


//! Checks PDL_AlgorithmsRepository::RegisterAlgorithm() with a second algorithm
//!
void UT_PDL_AlgorithmsRepository::test_RegisterAlgorithm_SecondOne ()
{
  auto  count  = 0;
  auto  algo_1 = [&count](){ count += 1; };
  auto  algo_2 = [&count](){ count += 2; };
  auto& sut    = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions
  sut.RegisterAlgorithm("Foo", algo_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.RegisterAlgorithm("Bar", algo_2));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredAlgorithmCount(), 2u);
}


//! Checks PDL_AlgorithmsRepository::RegisterAlgorithm() when overloading a pre-registered algorithm
//!
void UT_PDL_AlgorithmsRepository::test_RegisterAlgorithm_Overload ()
{
  auto  count = false;
  auto  algo_1  = [&count](){ count += 1; };
  auto  algo_2  = [&count](){ count += 2; };
  auto  algo_3  = [&count](){ count += 4; };
  auto& sut     = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions
  sut.RegisterAlgorithm("Foo", algo_1);
  sut.RegisterAlgorithm("Bar", algo_2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.RegisterAlgorithm("Foo", algo_3));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredAlgorithmCount(), 2u);
}

//! Checks PDL_AlgorithmsRepository::GetAlgorithm() when there is not PDL algorithm registered yet
//!
void UT_PDL_AlgorithmsRepository::test_GetAlgorithm_0 ()
{
  auto& sut = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAlgorithm("Foo"), std::exception);
}

//! Checks PDL_AlgorithmsRepository::GetAlgorithm() when there is one and it exists
//!
void UT_PDL_AlgorithmsRepository::test_GetAlgorithm_1_Exists ()
{
  auto  count = 0;
  auto  algo  = [&count](){ ++count; };
  auto& sut   = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions
  sut.RegisterAlgorithm("Foo", algo);

  // ---------------- Exercise
  //
  auto retrievedAlgo = sut.GetAlgorithm("Foo");

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredAlgorithmCount(), 1u);   // This is an invariant (algorithm is not removed)

  TS_ASSERT_EQUALS (count, 0);  // algorithm has not been run yet
  retrievedAlgo();
  TS_ASSERT_EQUALS (count, 1);
}

//! Checks PDL_AlgorithmsRepository::GetAlgorithm() when there is one but none is registered with given name
//!
void UT_PDL_AlgorithmsRepository::test_GetAlgorithm_1_not_Exists ()
{
  auto  count = 0;
  auto  algo  = [&count](){ ++count; };
  auto& sut   = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions
  sut.RegisterAlgorithm("Foo", algo);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAlgorithm("Bar"), std::exception);
}

//! Checks PDL_AlgorithmsRepository::GetAlgorithm() when there is 3 and requested one exists
//!
void UT_PDL_AlgorithmsRepository::test_GetAlgorithm_3_Exists ()
{
  auto  count  = 0;
  auto  algo_1 = [&count](){ count += 1; };
  auto  algo_2 = [&count](){ count += 2; };
  auto  algo_3 = [&count](){ count += 4; };
  auto& sut    = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions
  sut.RegisterAlgorithm("Foo", algo_1);
  sut.RegisterAlgorithm("Bar", algo_2);
  sut.RegisterAlgorithm("Tap", algo_3);

  // ---------------- Exercise
  //
  auto retrievedAlgo = sut.GetAlgorithm("Tap");

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredAlgorithmCount(), 3u);   // This is an invariant (algorithm is not removed)

  TS_ASSERT_EQUALS (count, 0);  // algorithm has not been run yet
  retrievedAlgo();
  TS_ASSERT_EQUALS (count, 4);
}

//! Checks PDL_AlgorithmsRepository::GetAlgorithm() when there are 3 registered but none with the given name
//!
void UT_PDL_AlgorithmsRepository::test_GetAlgorithm_3_not_Exists ()
{
  // ---------------- Setup
  //
  auto  count  = 0;
  auto  algo_1 = [&count](){ count += 1; };
  auto  algo_2 = [&count](){ count += 2; };
  auto  algo_3 = [&count](){ count += 4; };
  auto& sut    = PDL_AlgorithmsRepository::Instance();

  sut.Clear();  // Get rid of previously registered functions
  sut.RegisterAlgorithm("Foo", algo_1);
  sut.RegisterAlgorithm("Bar", algo_2);
  sut.RegisterAlgorithm("Tap", algo_3);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAlgorithm("Bit"), std::exception);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredAlgorithmCount(), 3u);   // This is an invariant
}



//===========================================================================
// End of UT_PDL_AlgorithmsRepository.cpp
//===========================================================================
