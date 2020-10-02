//===========================================================================
//                           UT_Streamer.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Streamer.hpp
//!
//! Declares test fixture class for testing Streamer
//!
//===========================================================================


#ifndef UT_STREAMER_H__78C4B8C9_4565_49BE_2D84_20A2F5A162DD__INCLUDED_
  #define UT_STREAMER_H__78C4B8C9_4565_49BE_2D84_20A2F5A162DD__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class Streamer
//!
class UT_Streamer : public CxxTest::TestSuite
{
  public:
   UT_Streamer() = default;
  ~UT_Streamer() = default;


  // ================ Test Methods ================


  // ---------------- Constructor
  //
  void test_Constructor ();
  void test_Getters ();
  void test_Setters ();


};
//
//  End of UT_Streamer class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_STREAMER_H__78C4B8C9_4565_49BE_2D84_20A2F5A162DD__INCLUDED_
//===========================================================================
// End of UT_Streamer.hpp
//===========================================================================
