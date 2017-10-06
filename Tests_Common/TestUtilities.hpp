//===========================================================================
//                           TestUtilities.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TestUtilities.hpp
//!
//! Declares utilities for unit testing
//!
//===========================================================================

#ifndef TESTUTILITIES_H__C6FEB581_2410_4C25_86BD_7A0D63EA5A11__INCLUDED_
  #define TESTUTILITIES_H__C6FEB581_2410_4C25_86BD_7A0D63EA5A11__INCLUDED_

#include <string>
#include <initializer_list>

namespace test
{
  //! Returns file path given a test file name and names of sub-dir to it
  //!
  extern std::string GetTestFilePath(const std::initializer_list<std::string>& subPathsToFile, bool checkExists = true);

  //! Returns directory path of currently running test runner
  //!
  extern const std::string& GetRunnerDirectoryPath();

} // End of namespace test

#endif  // not defined TESTUTILITIES_H__C6FEB581_2410_4C25_86BD_7A0D63EA5A11__INCLUDED_
//===========================================================================
// End of TestUtilities.hpp
//===========================================================================



