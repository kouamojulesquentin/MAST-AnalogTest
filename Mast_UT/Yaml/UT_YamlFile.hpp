//===========================================================================
//                           UT_YamlFile.hpp
//===========================================================================
// Copyright (C) 2016 Jean-Francois Coulon. All rights reserved.
//
// Project : Multi-Edit extension DLL
//
//! @file UT_YamlFile.hpp
//!
//! Declares test fixture class for testing YamlFile
//!
//===========================================================================
// Created by JFC on October/25/2016
//===========================================================================


#ifndef UT_YAMLFILE_H__29A78894_55AC_4688_D81_80E9EF3DBAC3__INCLUDED_
  #define UT_YAMLFILE_H__29A78894_55AC_4688_D81_80E9EF3DBAC3__INCLUDED_

#include <cxxtest/TestSuiteEx.h>

//! Unit tests for class YamlFile
//!
//!
class UT_YamlFile : public CxxTest::TestSuite
{
  public:

  // ================ Test Methods ================

  // ---------------- Check
  //
  void test_Check_Invalid ();
  void test_Check_Valid ();

  // ---------------- Constructor
  //
  void test_DefaultConstructor ();


  // ---------------- Load
  //
  void test_Load_InvalidYaml ();
  void test_Load_FirstTime ();
  void test_Load_SecondTime ();

  // ---------------- RemoveKey
  //
  void test_RemoveKey ();

  // ---------------- GetAsString
  //
  void test_GetAsString_EmptyPath ();
  void test_GetAsString_AutoCreate_false ();
  void test_GetAsString_AutoCreate_false_NotExist ();
  void test_GetAsString_AutoCreate_true ();
  void test_GetAsString_NoDefault ();
  void test_GetAsString_NoDefault_NotExist ();

  // ---------------- GetAsInt
  //
  void test_GetAsInt_EmptyPath ();
  void test_GetAsInt_AutoCreate_false ();
  void test_GetAsInt_AutoCreate_true ();
  void test_GetAsInt_NoDefault ();
  void test_GetAsInt_NoDefault_NotExist ();

  // ---------------- GetAsBool
  //
  void test_GetAsBool_EmptyPath ();
  void test_GetAsBool_AutoCreate_false ();
  void test_GetAsBool_AutoCreate_true ();
  void test_GetAsBool_NoDefault ();
  void test_GetAsBool_NoDefault_NotExist ();

  // ---------------- TryGetAsString
  //
  void test_TryGetAsString_Exist ();
  void test_TryGetAsString_NotExist ();

  // ---------------- TryGetAsInt
  //
  void test_TryGetAsInt_NoDefault ();
  void test_TryGetAsInt_NoDefault_NotExist ();

  // ---------------- TryGetAsBool
  //
  void test_TryGetAsBool_NoDefault ();
  void test_TryGetAsBool_NoDefault_NotExist ();

  // ---------------- GetAsString for sequences
  //
  void test_GetAsString_Sequence_EmptyPath ();
  void test_GetAsString_Sequence_AutoCreate_false ();
  void test_GetAsString_Sequence_AutoCreate_false_NotExist ();
  void test_GetAsString_Sequence_AutoCreate_true ();
  void test_GetAsString_Sequence_AutoCreate_true_Ordered ();
  void test_GetAsString_Sequence_NoDefault ();
  void test_GetAsString_Sequence_NoDefault_NotExist ();

  // ---------------- GetAsInt for sequences
  //
  void test_GetAsInt_Sequence_EmptyPath ();
  void test_GetAsInt_Sequence_AutoCreate_false ();
  void test_GetAsInt_Sequence_AutoCreate_false_NotExist ();
  void test_GetAsInt_Sequence_AutoCreate_true ();
  void test_GetAsInt_Sequence_AutoCreate_true_Ordered ();
  void test_GetAsInt_Sequence_NoDefault ();
  void test_GetAsInt_Sequence_NoDefault_NotExist ();

  // ---------------- GetAsBool for sequences
  //
  void test_GetAsBool_Sequence_EmptyPath ();
  void test_GetAsBool_Sequence_AutoCreate_false ();
  void test_GetAsBool_Sequence_AutoCreate_false_NotExist ();
  void test_GetAsBool_Sequence_AutoCreate_true ();
  void test_GetAsBool_Sequence_AutoCreate_true_Ordered ();
  void test_GetAsBool_Sequence_NoDefault ();
  void test_GetAsBool_Sequence_NoDefault_NotExist ();

  // ---------------- GetAsXxx for sequences of sequences
  //


  // ---------------- Set (string)
  //
  void test_Set_String_EmptyPath ();
  void test_Set_String_NotAlreadyExist ();
  void test_Set_String_AlreadyExist_Different ();
  void test_Set_String_AlreadyExist_Same ();

  // ---------------- Set (int)
  //
  void test_Set_Int_EmptyPath ();
  void test_Set_Int_NotAlreadyExist ();
  void test_Set_Int_AlreadyExist_Different ();
  void test_Set_Int_AlreadyExist_Same ();

  // ---------------- Set (bool)
  //
  void test_Set_Bool_EmptyPath ();
  void test_Set_Bool_NotAlreadyExist ();
  void test_Set_Bool_AlreadyExist_Different ();
  void test_Set_Bool_AlreadyExist_Same ();

  // ---------------- Load (continued: using Set)
  //
  void test_Load_Dirty_NoSave ();
  void test_Load_Dirty_SaveButNoAssociatedFile ();

  // ---------------- Save
  //
  void test_Save_EmptyFilePath ();
  void test_Save_NoAssociatedFile ();
  void test_Save_ValidPath();
  void test_Save_InvalidPath();

  // ---------------- CheckFile
  //
  void test_CheckFile_InvalidPath ();
  void test_CheckFile_InvalidYaml ();
  void test_CheckFile_Valid ();

  // ---------------- Load File
  //
  void test_LoadFile_InvalidPath ();
  void test_LoadFile_InvalidYaml ();
  void test_LoadFile_Valid ();

  // ---------------- TryLoadFile
  //
  void test_TryLoadFile_InvalidPath ();
  void test_TryLoadFile_InvalidYaml ();
  void test_TryLoadFile_Valid ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_YamlFile class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_YAMLFILE_H__29A78894_55AC_4688_D81_80E9EF3DBAC3__INCLUDED_

//===========================================================================
// End of UT_YamlFile.hpp
//===========================================================================
