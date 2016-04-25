//===========================================================================
//                           UT_FileAsserts_Successes.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_FileAsserts_Successes.cpp
//!
//! Implements test fixture for testing FileAsserts
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/30/2015
//===========================================================================



#include "UT_FileAsserts_Successes.h"
#include <cxxtest/ValueTraits.h>

#define SAMPLE_FILES_DIR   "UT_Sources/Files_to_compare/"
#define EMPTY_FILE         SAMPLE_FILES_DIR "Empty.txt"
#define SMALL_FILE         SAMPLE_FILES_DIR "TwoLines.txt"
#define SAME_AS_SMALL_FILE SAMPLE_FILES_DIR "TwoLines - Copie.txt"
#define MEDIUM_FILE        SAMPLE_FILES_DIR "MediumSize.txt"
#define FIVE_LINES_FILE    SAMPLE_FILES_DIR "FiveLines.txt"
#define NOT_EXISTING_FILE  SAMPLE_FILES_DIR "!!! Please_delete_this_file_!!!.txt"

#define FIVE_LINES_FILE_CONTENT "1st Line\n2nd Line from resources !!!\n    This is third line\n\n      This is fifth line (after empty 4th one)."

namespace
{
} // End of unnamed namespace


UT_FileAsserts_Successes::UT_FileAsserts_Successes ()
{
}

UT_FileAsserts_Successes::~UT_FileAsserts_Successes ()
{
}


//! Checks that TS_ASSERT_SAME_FILES() when using std::string
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_SAME_FILES_std_string ()
{
  TS_ASSERT_SAME_FILES(std::string(SAME_AS_SMALL_FILE), std::string(SMALL_FILE));
}


//! Checks that TS_ASSERT_FILE_EXISTS() when files does exist
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_EXISTS ()
{
  TS_ASSERT_FILE_EXISTS(SMALL_FILE);
}

//! Checks that TSM_ASSERT_FILE_EXISTS() when files does exist
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_EXISTS_message ()
{
  TSM_ASSERT_FILE_EXISTS("Message from test_TS_ASSERT_FILE_EXISTS_message", SMALL_FILE);
}


//! Checks that TS_ASSERT_FILE_EXISTS() when using std::string
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_EXISTS_std_string ()
{
  TS_ASSERT_FILE_EXISTS(std::string(SMALL_FILE));
}


//! Checks that TS_ASSERT_FILE_NOT_EXISTS() when files does exist
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_NOT_EXISTS ()
{
  TS_ASSERT_FILE_NOT_EXISTS(NOT_EXISTING_FILE);
}

//! Checks that TSM_ASSERT_FILE_NOT_EXISTS() when files does exist
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_NOT_EXISTS_message ()
{
  TSM_ASSERT_FILE_NOT_EXISTS("Message from test_TS_ASSERT_FILE_NOT_EXISTS_message", NOT_EXISTING_FILE);
}


//! Checks that TS_ASSERT_FILE_NOT_EXISTS() when using std::string
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_NOT_EXISTS_std_string ()
{
  TS_ASSERT_FILE_NOT_EXISTS(std::string(NOT_EXISTING_FILE));
}


//! Checks that TS_ASSERT_FILE_SIZE()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_SIZE ()
{
  TS_ASSERT_FILE_SIZE(MEDIUM_FILE, 6126);
}

//! Checks that TSM_ASSERT_FILE_SIZE()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_SIZE_message ()
{
  TSM_ASSERT_FILE_SIZE("Message from test_TS_ASSERT_FILE_SIZE_message", MEDIUM_FILE, 6126);
}


//! Checks that TS_ASSERT_FILE_SIZE() when using std::string
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_SIZE_std_string ()
{
  TS_ASSERT_FILE_SIZE(std::string(MEDIUM_FILE), 6126);
}

//! Checks that TS_ASSERT_FILE_EMPTY()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_EMPTY ()
{
  TS_ASSERT_FILE_EMPTY(EMPTY_FILE);
}

//! Checks that TSM_ASSERT_FILE_EMPTY()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_EMPTY_message ()
{
  TSM_ASSERT_FILE_EMPTY("Message from test_TS_ASSERT_FILE_EMPTY_message", EMPTY_FILE);
}


//! Checks that TS_ASSERT_FILE_EMPTY() when using std::string
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_EMPTY_std_string ()
{
  TS_ASSERT_FILE_EMPTY(std::string(EMPTY_FILE));
}


//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_CONTENT ()
{
  TS_ASSERT_FILE_CONTENT(FIVE_LINES_FILE, FIVE_LINES_FILE_CONTENT);
}

//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_CONTENT_message ()
{
  const char* expectedContent = FIVE_LINES_FILE_CONTENT;
  TSM_ASSERT_FILE_CONTENT("Message from test_TS_ASSERT_FILE_CONTENT_message", FIVE_LINES_FILE, expectedContent);
}

//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_CONTENT_std_string ()
{
  TS_ASSERT_FILE_CONTENT(std::string(FIVE_LINES_FILE), std::string(FIVE_LINES_FILE_CONTENT));
}

//! Checks that TS_ASSERT_FILE_CONTENT()
//!
void UT_FileAsserts_Successes::test_TS_ASSERT_FILE_CONTENT_both_empty ()
{
  TS_ASSERT_FILE_CONTENT(EMPTY_FILE, "");
}




//===========================================================================
// End of UT_FileAsserts_Successes.cpp
//===========================================================================
