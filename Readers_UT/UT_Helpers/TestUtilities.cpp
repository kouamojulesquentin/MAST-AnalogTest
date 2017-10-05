//===========================================================================
//                           TestUtilities.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TestUtilities.cpp
//!
//! Implements utilities for unit testing
//!
//===========================================================================

#include "TestUtilities.hpp"
#include "MastConfig.hpp"

#include <cxxtest/TestSuite.h>

using std::string;
using std::initializer_list;

extern string Exe_Dir_Path;

//! Returns file path given a test file name and names of sub-dir to it
//!
//! @param subPathsToFile Names of directory down to file to used in test
//! @param checkExists    When true, file reality is checked (useful for input files)
//!
string test::GetTestFilePath(const initializer_list<std::string>& subPathsToFile, bool checkExists)
{
  CxxTest::CountAssertDisabler assertDisabler;  // To not count the assertion below as a caller test assertion

  auto makePath = [](string first, initializer_list<string> otherParts)
  {
    string filePath(first);
    for (const auto& part : otherParts)
    {
      filePath.append(DIRECTORY_SEPARATOR).append(part);
    }
    return filePath;
  };

  auto filePath = makePath(Exe_Dir_Path, subPathsToFile);
  if (checkExists)
  {
    TS_ASSERT_FILE_EXISTS (filePath);
  }
  return filePath;
}

//! Returns directory path of currently running test runner
//!
const string& test::GetRunnerDirectoryPath()
{
  return Exe_Dir_Path;
}


//===========================================================================
// End of TestUtilities.cpp
//===========================================================================
