//===========================================================================
//                           UT_AccessInterfaceTranslator.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AccessInterfaceTranslator.cpp
//!
//! Implements test fixture for testing AccessInterfaceTranslator
//!
//===========================================================================

#include "UT_AccessInterfaceTranslator.hpp"
#include "AccessInterfaceTranslator.hpp"
#include "BinaryVector_Traits.hpp"

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::ostringstream;
using std::initializer_list;
using std::vector;

using namespace std::string_literals;
using namespace mast;


//! Initializes test (called for each test)
void UT_AccessInterfaceTranslator::setUp ()
{
}


//! Checks AccessInterfaceTranslator empty constructor
//!
void UT_AccessInterfaceTranslator::test_empty_Constructor ()
{
  // ---------------- Setup
  //

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (AccessInterfaceTranslator sut());
  
}

//! Checks AccessInterfaceTranslator empty constructor
//!
void UT_AccessInterfaceTranslator::test_not_empty_Constructor ()
{
  // ---------------- Setup
  //

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (AccessInterfaceTranslator sut("Test"));

  AccessInterfaceTranslator sut("Test");
  TS_ASSERT_EQUALS(sut.TypeName(),"AccessInterfaceTranslator")
  TS_ASSERT_EQUALS(sut.Name(),"Test")
}



//===========================================================================
// End of UT_AccessInterfaceTranslator.cpp
//===========================================================================
