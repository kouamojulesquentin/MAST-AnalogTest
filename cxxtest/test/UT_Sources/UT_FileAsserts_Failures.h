//===========================================================================
//                           UT_FileAsserts_Failures.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_FileAsserts_Failures.h
//!
//! Declares test fixture class for testing FileAsserts
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/30/2015
//===========================================================================

#ifndef UT_FILEASSERTS_FAILURES_H__AFE10583_41D6_4FED_589E_334D6FEDEAFB__INCLUDED_
  #define UT_FILEASSERTS_FAILURES_H__AFE10583_41D6_4FED_589E_334D6FEDEAFB__INCLUDED_

#include <cxxtest/TestSuite.h>



//! Unit tests for files related asserts when they should fail
//!
//! TS_ASSERT_SAME_FILES has already been pretty well tested elsewere (see xxxFilesyyy.h files)
//!
class UT_FileAsserts_Failures : public CxxTest::TestSuite
{
  public:
   UT_FileAsserts_Failures();
  ~UT_FileAsserts_Failures();


  // ================ Test Methods ================

  // ---------------- TS_ASSERT_SAME_FILES
  //
  void test_TS_ASSERT_SAME_FILES_Left_LessLines ();
  void test_TS_ASSERT_SAME_FILES_Left_MoreLines ();
  void test_TS_ASSERT_SAME_FILES_Differ_at_FirstCol ();
  void test_TS_ASSERT_SAME_FILES_Differ_at_MiddleCol ();
  void test_TS_ASSERT_SAME_FILES_Differ_at_LastCol ();
  void test_TS_ASSERT_SAME_FILES_Exception_inside ();
  void test_TS_ASSERT_SAME_FILES_Exception_after ();
  void test_TS_ASSERT_SAME_FILES_Exception_inside_and_after ();

  // ---------------- TS_ASSERT_FILE_EXISTS
  //
  void test_TS_ASSERT_FILE_EXISTS ();
  void test_TS_ASSERT_FILE_EXISTS_message ();
  void test_TS_ASSERT_FILE_EXISTS_Exception_inside ();
  void test_TS_ASSERT_FILE_EXISTS_Exception_after ();
  void test_TS_ASSERT_FILE_EXISTS_Exception_inside_and_after ();

  // ---------------- TS_ASSERT_FILE_NOT_EXISTS
  //
  void test_TS_ASSERT_FILE_NOT_EXISTS ();
  void test_TS_ASSERT_FILE_NOT_EXISTS_message ();
  void test_TS_ASSERT_FILE_NOT_EXISTS_Exception_inside ();
  void test_TS_ASSERT_FILE_NOT_EXISTS_Exception_after ();
  void test_TS_ASSERT_FILE_NOT_EXISTS_Exception_inside_and_after ();

  // ---------------- TS_ASSERT_FILE_SIZE
  //
  void test_TS_ASSERT_FILE_SIZE ();
  void test_TS_ASSERT_FILE_SIZE_message ();
  void test_TS_ASSERT_FILE_SIZE_std_string ();
  void test_TS_ASSERT_FILE_SIZE_not_exist ();
  void test_TS_ASSERT_FILE_SIZE_Exception_inside ();
  void test_TS_ASSERT_FILE_SIZE_Exception_after ();
  void test_TS_ASSERT_FILE_SIZE_Exception_inside_and_after ();

  // ---------------- TS_ASSERT_FILE_EMPTY
  //
  void test_TS_ASSERT_FILE_EMPTY ();
  void test_TS_ASSERT_FILE_EMPTY_message ();
  void test_TS_ASSERT_FILE_EMPTY_std_string ();
  void test_TS_ASSERT_FILE_EMPTY_not_exist ();
  void test_TS_ASSERT_FILE_EMPTY_Exception_inside ();
  void test_TS_ASSERT_FILE_EMPTY_Exception_after ();
  void test_TS_ASSERT_FILE_EMPTY_Exception_inside_and_after ();

  // ---------------- TS_ASSERT_FILE_CONTENT
  //
  void test_TS_ASSERT_FILE_CONTENT_Differ_At_Line_1 ();
  void test_TS_ASSERT_FILE_CONTENT_file_empty ();
  void test_TS_ASSERT_FILE_CONTENT_file_smaller ();
  void test_TS_ASSERT_FILE_CONTENT_file_larger ();
  void test_TS_ASSERT_FILE_CONTENT_empty_expected ();
  void test_TS_ASSERT_FILE_CONTENT_message ();
  void test_TS_ASSERT_FILE_CONTENT_std_string ();
  void test_TS_ASSERT_FILE_CONTENT_not_exist ();
  void test_TS_ASSERT_FILE_CONTENT_Exception_inside ();
  void test_TS_ASSERT_FILE_CONTENT_Exception_after ();
  void test_TS_ASSERT_FILE_CONTENT_Exception_inside_and_after ();

};
//
//  End of UT_FileAsserts_Failures class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_FILEASSERTS_FAILURES_H__AFE10583_41D6_4FED_589E_334D6FEDEAFB__INCLUDED_
//===========================================================================
// End of UT_FileAsserts_Failures.h
//===========================================================================
