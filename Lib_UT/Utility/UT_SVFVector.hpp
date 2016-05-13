//===========================================================================
//                           UT_SVFVector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SVFVector.hpp
//!
//! Declares test fixture class for testing SVFVector
//!
//===========================================================================


#ifndef UT_SVFVECTOR_H__42FD8ECC_6C82_4D6A_8382_CB1A6840B539__INCLUDED_
  #define UT_SVFVECTOR_H__42FD8ECC_6C82_4D6A_8382_CB1A6840B539__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SVFVector
//!
//!
class UT_SVFVector : public CxxTest::TestSuite
{
  public:
   UT_SVFVector() = default;
  ~UT_SVFVector() = default;


  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor_Default ();
  void test_Constructor_FromString ();
  void test_Constructor_FromString_With_Separators ();
  void test_Constructor_FromString_Not_Valid_Parameters ();
  void test_Constructor_FromBinary ();

  // ---------------- Other methods
  //
  void test_ToBinaryVector ();
};
//
//  End of UT_SVFVector class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SVFVECTOR_H__42FD8ECC_6C82_4D6A_8382_CB1A6840B539__INCLUDED_
//===========================================================================
// End of UT_SVFVector.hpp
//===========================================================================
