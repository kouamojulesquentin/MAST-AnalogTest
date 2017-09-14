//===========================================================================
//                           UT_AccessInterfaceTranslator.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AccessInterfaceTranslator.hpp
//!
//! Declares test fixture class for testing AccessInterfaceTranslator
//!
//===========================================================================


#ifndef UT_AccessInterfaceTranslator_H__942464C3_F4BB_473E_C9B9_772C545C6092__INCLUDED_
  #define UT_AccessInterfaceTranslator_H__942464C3_F4BB_473E_C9B9_772C545C6092__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AccessInterfaceTranslator
//!
//!
class UT_AccessInterfaceTranslator : public CxxTest::TestSuite
{
  public:
   UT_AccessInterfaceTranslator() = default;
  ~UT_AccessInterfaceTranslator() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_empty_Constructor ();
  void test_not_empty_Constructor ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_AccessInterfaceTranslator class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_AccessInterfaceTranslator_H__942464C3_F4BB_473E_C9B9_772C545C6092__INCLUDED_
//===========================================================================
// End of UT_AccessInterfaceTranslator.hpp
//===========================================================================
