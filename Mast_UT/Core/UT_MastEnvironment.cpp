//===========================================================================
//                           UT_MastEnvironment.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastEnvironment.cpp
//!
//! Implements test fixture for testing MastEnvironment
//!
//===========================================================================

#include "UT_MastEnvironment.hpp"
#include "MastEnvironment.hpp"

#include <cxxtest/ValueTraits.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

using namespace mast;



//! Checks MastEnvironment construction for unit tests context
//!
void UT_MastEnvironment::test_Constructor_UnitTest ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (MastEnvironment sut(true));
}



//! Checks MastEnvironment::ParseOptions() with no arguments (just program name)
//!
void UT_MastEnvironment::test_ParseOptions_NoOptions ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  vector<string>  options {"Mast.exe"};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseOptions(options));
}


//! Checks MastEnvironment::LoadPlugins() before requesting to parse options
//!
void UT_MastEnvironment::test_LoadPlugins_BeforeParsingOptions ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.LoadPlugins(), std::exception);
}

//===========================================================================
// End of UT_MastEnvironment.cpp
//===========================================================================
