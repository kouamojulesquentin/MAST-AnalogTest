//===========================================================================
//                           UT_DataCycle.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DataCycle.hpp
//!
//! Declares test fixture class for testing DataCycle
//!
//===========================================================================


#ifndef UT_DATACYCLE_H__CDABAA69_A31A_480E_11A1_A70C851230FD__INCLUDED_
  #define UT_DATACYCLE_H__CDABAA69_A31A_480E_11A1_A70C851230FD__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for behaviors associated with "data cycle" by combination of several classes
//!
//! @note This mainly test the synergie between ConfigureVisitor, ToSutVisitor and FromSutUpdater
//!
class UT_DataCycle : public CxxTest::TestSuite
{
  public:
   UT_DataCycle() = default;
  ~UT_DataCycle() = default;

  // ================ Test Methods ================

  void test_SimpleDataCycle_Without_ConfAlgo ();
  void test_SimpleDataCycle_With_DefaultConfAlgo ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_DataCycle class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_DATACYCLE_H__CDABAA69_A31A_480E_11A1_A70C851230FD__INCLUDED_
//===========================================================================
// End of UT_DataCycle.hpp
//===========================================================================
