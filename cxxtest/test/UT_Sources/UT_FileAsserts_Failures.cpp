//===========================================================================
//                           UT_FileAsserts_Failures.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_FileAsserts_Failures.cpp
//!
//! Implements test fixture for testing FileAsserts
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/30/2015
//===========================================================================



#include "UT_FileAsserts_Failures.h"
#include <cxxtest/ValueTraits.h>

#define SAMPLE_FILES_DIR   "UT_Sources/Files_to_compare/"
#define EMPTY_FILE         SAMPLE_FILES_DIR "Empty.txt"
#define ONE_LINE_FILE      SAMPLE_FILES_DIR "OneLine.txt"
#define SMALL_FILE         SAMPLE_FILES_DIR "TwoLines.txt"
#define SAME_AS_SMALL_FILE SAMPLE_FILES_DIR "TwoLines - Copie.txt"
#define FIVE_LINES_FILE    SAMPLE_FILES_DIR "FiveLines.txt"
#define NOT_EXISTING_FILE  SAMPLE_FILES_DIR "!!! Please_delete_this_file_!!!.txt"

#define FIVE_LINES_FILE_CONTENT "1st Line\n2nd Line from resources !!!\n    This is third line\n\n      This is fifth line (after empty 4th one)."

namespace
{
  const char* getFilePathThrowing()
  {
    throw "Hello from getFilePathThrowing() !!!";
    return SMALL_FILE;
  }

  void justThrowSomething() { throw "This was thrown from justThrowSomething()."; }
} // End of unnamed namespace


UT_FileAsserts_Failures::UT_FileAsserts_Failures ()
{
}

UT_FileAsserts_Failures::~UT_FileAsserts_Failures ()
{
}


//! Checks that TS_ASSERT_SAME_FILES() when an exception is thrown inside the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_SAME_FILES_Exception_inside ()
{
  TS_ASSERT_SAME_FILES(getFilePathThrowing(), SMALL_FILE);
}

//! Checks that TS_ASSERT_SAME_FILES() when an exception is thrown after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_SAME_FILES_Exception_after ()
{
  TS_ASSERT_SAME_FILES(SMALL_FILE, SAME_AS_SMALL_FILE);
  justThrowSomething();
}

//! Checks that TS_ASSERT_SAME_FILES() when an exception is thrown inside and after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_SAME_FILES_Exception_inside_and_after ()
{
  TS_ASSERT_SAME_FILES(SMALL_FILE, getFilePathThrowing());
  justThrowSomething();
}

//! Checks that TS_ASSERT_FILE_EXISTS() when files does not exist
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EXISTS ()
{
  TS_ASSERT_FILE_EXISTS(NOT_EXISTING_FILE);
}

//! Checks that TSM_ASSERT_FILE_EXISTS() when files does not exist
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EXISTS_message ()
{
  TSM_ASSERT_FILE_EXISTS("Message from test_TS_ASSERT_FILE_EXISTS_message", NOT_EXISTING_FILE);
}

//! Checks that TS_ASSERT_FILE_EXISTS() when an exception is thrown inside the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EXISTS_Exception_inside ()
{
  TS_ASSERT_FILE_EXISTS(getFilePathThrowing());
}

//! Checks that TS_ASSERT_FILE_EXISTS() when an exception is thrown after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EXISTS_Exception_after ()
{
  TS_ASSERT_FILE_EXISTS(NOT_EXISTING_FILE);
  justThrowSomething();
}

//! Checks that TS_ASSERT_FILE_EXISTS() when an exception is thrown inside and after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EXISTS_Exception_inside_and_after ()
{
  TS_ASSERT_FILE_EXISTS(getFilePathThrowing());
  justThrowSomething();
}

//! Checks that TS_ASSERT_FILE_NOT_EXISTS() when files does exist
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_NOT_EXISTS ()
{
  TS_ASSERT_FILE_NOT_EXISTS(SMALL_FILE);
}

//! Checks that TS_ASSERT_FILE_NOT_EXISTS() when files does exist
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_NOT_EXISTS_message ()
{
  TSM_ASSERT_FILE_NOT_EXISTS("Message from test_TS_ASSERT_FILE_NOT_EXISTS_message", SMALL_FILE);
}

//! Checks that TS_ASSERT_FILE_NOT_EXISTS() when an exception is thrown inside the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_NOT_EXISTS_Exception_inside ()
{
  TS_ASSERT_FILE_NOT_EXISTS(getFilePathThrowing());
}

//! Checks that TS_ASSERT_FILE_NOT_EXISTS() when an exception is thrown after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_NOT_EXISTS_Exception_after ()
{
  TS_ASSERT_FILE_NOT_EXISTS(SMALL_FILE);
  justThrowSomething();
}

//! Checks that TS_ASSERT_FILE_NOT_EXISTS() when an exception is thrown inside and after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_NOT_EXISTS_Exception_inside_and_after ()
{
  TS_ASSERT_FILE_NOT_EXISTS(getFilePathThrowing());
  justThrowSomething();
}


//! Checks that TS_ASSERT_FILE_SIZE()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_SIZE ()
{
  TS_ASSERT_FILE_SIZE(SMALL_FILE, 100);
}

//! Checks that TSM_ASSERT_FILE_SIZE()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_SIZE_message ()
{
  TSM_ASSERT_FILE_SIZE("Message from test_TS_ASSERT_FILE_SIZE_message", SMALL_FILE, 200);
}

//! Checks that TS_ASSERT_FILE_SIZE() when using std::string
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_SIZE_std_string ()
{
  TS_ASSERT_FILE_SIZE(std::string(SMALL_FILE), 220);
}

//! Checks that TS_ASSERT_FILE_SIZE() when files does exist
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_SIZE_not_exist ()
{
  TS_ASSERT_FILE_SIZE(NOT_EXISTING_FILE, 39);
}

//! Checks that TS_ASSERT_FILE_SIZE() when an exception is thrown inside the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_SIZE_Exception_inside ()
{
  TS_ASSERT_FILE_SIZE(getFilePathThrowing(), 300);
}

//! Checks that TS_ASSERT_FILE_SIZE() when an exception is thrown after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_SIZE_Exception_after ()
{
  TS_ASSERT_FILE_SIZE(SMALL_FILE, 400);
  justThrowSomething();
}

//! Checks that TS_ASSERT_FILE_SIZE() when an exception is thrown inside and after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_SIZE_Exception_inside_and_after ()
{
  TS_ASSERT_FILE_SIZE(getFilePathThrowing(), 400);
  justThrowSomething();
}


//! Checks that TS_ASSERT_FILE_EMPTY()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EMPTY ()
{
  TS_ASSERT_FILE_EMPTY(SMALL_FILE);
}

//! Checks that TSM_ASSERT_FILE_EMPTY()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EMPTY_message ()
{
  TSM_ASSERT_FILE_EMPTY("Message from test_TS_ASSERT_FILE_EMPTY_message", SMALL_FILE);
}

//! Checks that TS_ASSERT_FILE_EMPTY() when using std::string
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EMPTY_std_string ()
{
  TS_ASSERT_FILE_EMPTY(std::string(SMALL_FILE));
}

//! Checks that TS_ASSERT_FILE_EMPTY() when files does exist
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EMPTY_not_exist ()
{
  TS_ASSERT_FILE_EMPTY(NOT_EXISTING_FILE);
}

//! Checks that TS_ASSERT_FILE_EMPTY() when an exception is thrown inside the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EMPTY_Exception_inside ()
{
  TS_ASSERT_FILE_EMPTY(getFilePathThrowing());
}

//! Checks that TS_ASSERT_FILE_EMPTY() when an exception is thrown after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EMPTY_Exception_after ()
{
  TS_ASSERT_FILE_EMPTY(SMALL_FILE);
  justThrowSomething();
}

//! Checks that TS_ASSERT_FILE_EMPTY() when an exception is thrown inside and after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_EMPTY_Exception_inside_and_after ()
{
  TS_ASSERT_FILE_EMPTY(getFilePathThrowing());
  justThrowSomething();
}


//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_Differ_At_Line_1 ()
{
  TS_ASSERT_FILE_CONTENT(FIVE_LINES_FILE, "1st line");
}

//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_file_empty ()
{
  TS_ASSERT_FILE_CONTENT(EMPTY_FILE, "1st line");
}

//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_file_smaller ()
{
  TS_ASSERT_FILE_CONTENT(ONE_LINE_FILE, "This is the only one line of this file\n?\n");
}

//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_file_larger ()
{
  TS_ASSERT_FILE_CONTENT(SMALL_FILE, "1st Line");
}

//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_empty_expected ()
{
  TS_ASSERT_FILE_CONTENT(SMALL_FILE, "");
}


//! Checks that TSM_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_message ()
{
  TSM_ASSERT_FILE_CONTENT("Message from test_TS_ASSERT_FILE_CONTENT_message", SMALL_FILE, "\n");
}

//! Checks that TS_ASSERT_FILE_CONTENT() when using std::string
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_std_string ()
{
  std::string expectedContent("1st Line\n2nd Line from resources !!!\nThis is third line\nAnd for 4th line?\nThis is fifth line (after empty 4th one).");
  TS_ASSERT_FILE_CONTENT(std::string(FIVE_LINES_FILE), expectedContent);
}

//! Checks that TS_ASSERT_FILE_CONTENT() when files does exist
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_not_exist ()
{
  TS_ASSERT_FILE_CONTENT(NOT_EXISTING_FILE, FIVE_LINES_FILE_CONTENT);
}

//! Checks that TS_ASSERT_FILE_CONTENT() when an exception is thrown inside the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_Exception_inside ()
{
  TS_ASSERT_FILE_CONTENT(getFilePathThrowing(), FIVE_LINES_FILE_CONTENT);
}

//! Checks that TS_ASSERT_FILE_CONTENT() when an exception is thrown after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_Exception_after ()
{
  TS_ASSERT_FILE_CONTENT(FIVE_LINES_FILE, FIVE_LINES_FILE_CONTENT);
  justThrowSomething();
}

//! Checks that TS_ASSERT_FILE_CONTENT() when an exception is thrown inside and after the macro
//!
void UT_FileAsserts_Failures::test_TS_ASSERT_FILE_CONTENT_Exception_inside_and_after ()
{
  TS_ASSERT_FILE_CONTENT(getFilePathThrowing(), FIVE_LINES_FILE_CONTENT);
  justThrowSomething();
}

//===========================================================================
// End of UT_FileAsserts_Failures.cpp
//===========================================================================
