//===========================================================================
//                           UT_IEEE_P1687.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_IEEE_P1687.hpp
//!
//! Declares test fixture class for testing ICL_Reader
//!
//===========================================================================


#ifndef UT_IEEE_P1687_H__1597BF99_A59E_4DB5_6BA9_A2D8900F03__INCLUDED_
  #define UT_IEEE_P1687_H__1597BF99_A59E_4DB5_6BA9_A2D8900F03__INCLUDED_

#include <cxxtest/TestSuite.h>
#include <memory>


//! Unit tests for class ICL_Reader in context of IEEE P1687 standard
//!
class UT_IEEE_P1687 : public CxxTest::TestSuite
{
  public:
   UT_IEEE_P1687() = default;
  ~UT_IEEE_P1687() = default;

  // ================ Test Methods ================

  void test_Parse_Annex_E_Examples ();
  void test_Parse_Annex_F_Design_Guidance ();

  // ---------------- Other methods
  //
  void setUp();    //!< Initializes test (called for each test)
  void tearDown(); //!< Cleanups test (called for each test)
};
//
//  End of UT_IEEE_P1687 class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_IEEE_P1687_H__1597BF99_A59E_4DB5_6BA9_A2D8900F03__INCLUDED_

//===========================================================================
// End of UT_IEEE_P1687.hpp
//===========================================================================
