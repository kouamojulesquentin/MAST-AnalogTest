//===========================================================================
//                           UT_BinaryVector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_BinaryVector.hpp
//!
//! Declares test fixture class for testingBinaryVector
//!
//===========================================================================


#ifndef UT_BINARYVECTOR_H__87D7609F_1DA4_4E53_99B4_F7664E038D88__INCLUDED_
  #define UT_BINARYVECTOR_H__87D7609F_1DA4_4E53_99B4_F7664E038D88__INCLUDED_

#include <cxxtest/TestSuiteEx.h>

//! Unit tests for class BinaryVector
//!
//!
class UT_BinaryVector : public CxxTest::TestSuite
{
  public:
   UT_BinaryVector() = default;
  ~UT_BinaryVector() = default;


  // ================ Test Methods ================

  // ---------------- Constructors
  //
  void test_Constructor_Default ();
  void test_Constructor_Copy_When_SrcIsEmpty ();
  void test_Constructor_Move_When_SrcIsEmpty ();

  // ---------------- Append
  //
  void test_Append_8_bits_When_Empty ();
  void test_Append_16_bits_When_Empty ();
  void test_Append_32_bits_When_Empty ();
  void test_Append_64_bits_When_Empty ();

  void test_Append_8_bits_When_NotEmpty ();
  void test_Append_16_bits_When_NotEmpty ();
  void test_Append_32_bits_When_NotEmpty ();
  void test_Append_64_bits_When_NotEmpty ();

  void test_Append_1_to_8_bits_When_Empty ();
  void test_Append_1_to_8_bits_When_NotEmpty ();

  //! @todo [JFC]-[April/25/2016]: Remove "test_No_test_yet_for_Guard" method when all tests are implemented
  //!
  void test_No_test_yet_for_Guard ();
};
//
//  End of UT_BinaryVector class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_BINARYVECTOR_H__87D7609F_1DA4_4E53_99B4_F7664E038D88__INCLUDED_
//===========================================================================
// End of UT_BinaryVector.hpp
//===========================================================================
