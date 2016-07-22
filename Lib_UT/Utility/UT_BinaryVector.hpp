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

  // ---------------- CreateFromXXXString
  //
  void test_CreateFromBinaryString ();
  void test_CreateFromBinaryString_InvalidChars ();
  void test_CreateFromHexString ();
  void test_CreateFromHexString_InvalidChars ();
  void test_CreateFromString ();
  void test_CreateFromString_InvalidChars ();

  // ---------------- DataAsBinaryString
  //
  void test_DataAsBinaryString_Without_NewLine ();
  void test_DataAsBinaryString_With_NewLine ();
  void test_DataAsBinaryString_Without_Separators ();
  void test_DataAsHexString_Without_NewLine ();
  void test_DataAsHexString_With_NewLine ();
  void test_DataAsHexString_Without_Separators ();
  void test_DataAsMixString_Without_Separators ();
  void test_DataAsMixString_Without_NewLine ();
  void test_DataAsMixString_With_NewLine ();

  // ---------------- Operator==
  //
  void test_operator_eq_With_Self ();
  void test_operator_eq_When_Equal ();
  void test_operator_eq_When_NotEqual ();

  // ---------------- Operator!=
  //
  void test_operator_neq_With_Self ();
  void test_operator_neq_When_Equal ();
  void test_operator_neq_When_NotEqual ();

  // ---------------- Constructors when making it not empty
  //
  void test_Constructor_With_Count_and_Pattern ();
  void test_Constructor_Copy ();
  void test_Constructor_Copy_FixedSize ();
  void test_Constructor_Copy_FixedSizeOnCopy ();
  void test_Constructor_Move ();
  void test_Constructor_Move_FixedSize ();
  void test_Constructor_Move_FixedSizeOnCopy ();

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

  void test_Append_8_bits_When_FixedSize ();
  void test_Append_16_bits_When_FixedSize ();
  void test_Append_32_bits_When_FixedSize ();
  void test_Append_64_bits_When_FixedSize ();

  void test_Append_1_to_8_bits_When_Empty_Right_Aligned ();
  void test_Append_1_to_8_bits_When_Empty_Left_Aligned ();
  void test_Append_1_to_8_bits_When_NotEmpty_Right_Aligned ();
  void test_Append_1_to_8_bits_When_NotEmpty_Left_Aligned ();

  void test_Append_1_to_16_bits_When_Empty_Right_Aligned ();
  void test_Append_1_to_16_bits_When_Empty_Left_Aligned ();
  void test_Append_1_to_16_bits_When_NotEmpty_Right_Aligned ();
  void test_Append_1_to_16_bits_When_NotEmpty_Left_Aligned ();

  void test_Append_1_to_32_bits_When_Empty_Right_Aligned ();
  void test_Append_1_to_32_bits_When_Empty_Left_Aligned ();
  void test_Append_1_to_32_bits_When_NotEmpty_Right_Aligned ();
  void test_Append_1_to_32_bits_When_NotEmpty_Left_Aligned ();

  void test_Append_1_to_64_bits_When_Empty_Right_Aligned ();
  void test_Append_1_to_64_bits_When_Empty_Left_Aligned ();
  void test_Append_1_to_64_bits_When_NotEmpty_Right_Aligned ();
  void test_Append_1_to_64_bits_When_NotEmpty_Left_Aligned ();

  void test_Append_Other_When_Empty ();
  void test_Append_Other_When_NotEmpty ();
  void test_Append_Other_When_FixedSize ();

  // ---------------- Set
  //
  void test_Set_uint_8_When_Empty ();
  void test_Set_uint_16_When_Empty ();
  void test_Set_uint_32_When_Empty ();
  void test_Set_uint_64_When_Empty ();

  void test_Set_uint_8_When_NotEmpty_Not_Fixed ();
  void test_Set_uint_16_When_NotEmpty_Not_Fixed ();
  void test_Set_uint_32_When_NotEmpty_Not_Fixed ();
  void test_Set_uint_64_When_NotEmpty_Not_Fixed ();

  void test_Set_uint_8_When_NotEmpty_Fixed ();
  void test_Set_uint_16_When_NotEmpty_Fixed ();
  void test_Set_uint_32_When_NotEmpty_Fixed ();
  void test_Set_uint_64_When_NotEmpty_Fixed ();

  void test_Set_int_8_When_Empty ();
  void test_Set_int_16_When_Empty ();
  void test_Set_int_32_When_Empty ();
  void test_Set_int_64_When_Empty ();

  void test_Set_int_8_When_NotEmpty_Not_Fixed ();
  void test_Set_int_16_When_NotEmpty_Not_Fixed ();
  void test_Set_int_32_When_NotEmpty_Not_Fixed ();
  void test_Set_int_64_When_NotEmpty_Not_Fixed ();

  void test_Set_int_8_When_NotEmpty_Fixed ();
  void test_Set_int_16_When_NotEmpty_Fixed ();
  void test_Set_int_32_When_NotEmpty_Fixed ();
  void test_Set_int_64_When_NotEmpty_Fixed ();

  // ---------------- Get
  //
  void test_Get_uint_8_When_Empty ();
  void test_Get_uint_16_When_Empty ();
  void test_Get_uint_32_When_Empty ();
  void test_Get_uint_64_When_Empty ();

  void test_Get_uint_8_When_NotEmpty ();
  void test_Get_uint_16_When_NotEmpty ();
  void test_Get_uint_32_When_NotEmpty ();
  void test_Get_uint_64_When_NotEmpty ();

  void test_Get_int_8_When_Empty ();
  void test_Get_int_16_When_Empty ();
  void test_Get_int_32_When_Empty ();
  void test_Get_int_64_When_Empty ();

  void test_Get_int_8_When_NotEmpty ();
  void test_Get_int_16_When_NotEmpty ();
  void test_Get_int_32_When_NotEmpty ();
  void test_Get_int_64_When_NotEmpty ();

  // ---------------- operator<<
  //
  void test_Operator_Shift_When_Empty ();
  void test_Operator_Shift_When_NotEmpty ();
  void test_Operator_Shift_When_Cascaded ();
  void test_Operator_Shift_When_FixedSize ();

  // ---------------- operator+
  //
  void test_Operator_Plus_When_Empty ();
  void test_Operator_Plus_When_NotEmpty ();
  void test_Operator_Plus_When_FixedSize ();

  // ---------------- FixSize
  //
  void test_FixSize_When_WasNotFixed ();
  void test_FixSize_When_WasFixed ();

  // ---------------- operator=
  //
  void test_CopyAssignmentOperator ();
  void test_CopyAssignmentOperator_When_FixedSize_SameSize ();
  void test_CopyAssignmentOperator_When_FixedSize_DifferentSize ();
  void test_CopyAssignmentOperator_From_FixedSize ();
  void test_CopyAssignmentOperator_From_FixedSizeOnCopy ();
  void test_MoveAssignmentOperator ();
  void test_MoveAssignmentOperator_When_FixedSize_SameSize ();
  void test_MoveAssignmentOperator_When_FixedSize_DifferentSize ();
  void test_MoveAssignmentOperator_From_FixedSize ();
  void test_MoveAssignmentOperator_From_FixedSizeOnCopy ();

  // ---------------- ToggleBits
  //
  void test_ToggleBits ();
  void test_ToggleBits_Using_HexString ();
  void test_Operator_Tilde ();

  // ---------------- Single bit manipulation
  //
  void test_SetBit ();
  void test_ClearBit ();
  void test_ToggleBit ();

  // ---------------- Binary operators
  //
  void test_Operator_Bitwise_And_Assignment ();
  void test_Operator_Bitwise_Or_Assignment ();
  void test_Operator_Bitwise_Xor_Assignment ();
  void test_Operator_Bitwise_And_Assignment_SizeMismatch ();
  void test_Operator_Bitwise_Or_Assignment_SizeMismatch ();
  void test_Operator_Bitwise_Xor_Assignment_SizeMismatch ();

  void test_Operator_Bitwise_And ();
  void test_Operator_Bitwise_Or ();
  void test_Operator_Bitwise_Xor ();
  void test_Operator_Bitwise_And_SizeMismatch ();
  void test_Operator_Bitwise_Or_SizeMismatch ();
  void test_Operator_Bitwise_Xor_SizeMismatch ();

  // ---------------- Slice
  //
  void test_Slice ();
  void test_Slice_When_Exceeding_Capacity ();

  // ---------------- Other methods
  //
  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_BinaryVector class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_BINARYVECTOR_H__87D7609F_1DA4_4E53_99B4_F7664E038D88__INCLUDED_
//===========================================================================
// End of UT_BinaryVector.hpp
//===========================================================================
