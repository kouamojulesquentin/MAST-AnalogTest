//===========================================================================
//                           UT_FileAsserts_Successes.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_FileAsserts_Successes.h
//!
//! Declares test fixture class for testing FileAsserts
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/30/2015
//===========================================================================

#ifndef UT_FILEASSERTS_SUCCESSES_H__DD224493_92A3_4D65_D485_C853F97200__INCLUDED_
  #define UT_FILEASSERTS_SUCCESSES_H__DD224493_92A3_4D65_D485_C853F97200__INCLUDED_

#include <cxxtest/TestSuite.h>



//! Unit tests for files related asserts when they should fail
//!
//! TS_ASSERT_SAME_FILES has already been pretty well tested elsewere (see xxxFilesyyy.h files)
//!
class UT_FileAsserts_Successes : public CxxTest::TestSuite
{
  public:
   UT_FileAsserts_Successes();
  ~UT_FileAsserts_Successes();


  // ================ Test Methods ================

  // ---------------- TS_ASSERT_SAME_FILES
  //
  void test_TS_ASSERT_SAME_FILES_std_string ();

  // ---------------- TS_ASSERT_FILE_EXISTS
  //
  void test_TS_ASSERT_FILE_EXISTS ();
  void test_TS_ASSERT_FILE_EXISTS_message ();
  void test_TS_ASSERT_FILE_EXISTS_std_string ();

  // ---------------- TS_ASSERT_FILE_NOT_EXISTS
  //
  void test_TS_ASSERT_FILE_NOT_EXISTS ();
  void test_TS_ASSERT_FILE_NOT_EXISTS_message ();
  void test_TS_ASSERT_FILE_NOT_EXISTS_std_string ();

  // ---------------- TS_ASSERT_FILE_SIZE
  //
  void test_TS_ASSERT_FILE_SIZE ();
  void test_TS_ASSERT_FILE_SIZE_message ();
  void test_TS_ASSERT_FILE_SIZE_std_string ();

  // ---------------- TS_ASSERT_FILE_EMPTY
  //
  void test_TS_ASSERT_FILE_EMPTY ();
  void test_TS_ASSERT_FILE_EMPTY_message ();
  void test_TS_ASSERT_FILE_EMPTY_std_string ();

  // ---------------- TS_ASSERT_FILE_CONTENT
  //
  void test_TS_ASSERT_FILE_CONTENT ();
  void test_TS_ASSERT_FILE_CONTENT_message ();
  void test_TS_ASSERT_FILE_CONTENT_std_string ();
  void test_TS_ASSERT_FILE_CONTENT_both_empty ();

};
//
//  End of UT_FileAsserts_Successes class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_FILEASSERTS_SUCCESSES_H__DD224493_92A3_4D65_D485_C853F97200__INCLUDED_
//===========================================================================
// End of UT_FileAsserts_Successes.h
//===========================================================================
