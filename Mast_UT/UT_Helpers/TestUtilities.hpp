//===========================================================================
//                           TestUtilities.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TestUtilities.hpp
//!
//! Declares Mast Core unit tests utilities
//!
//===========================================================================


#ifndef TESTUTILITIES_H__79C2AF65_C90B_4E55_1D88_1E6D53095C0__INCLUDED_
  #define TESTUTILITIES_H__79C2AF65_C90B_4E55_1D88_1E6D53095C0__INCLUDED_

#include <string>

namespace test
{
  //! Returns file path given a test file name
  //!
  extern std::string GetTestFilePath(const std::string& fileName);
} // End of namespace test

#endif  // not defined TESTUTILITIES_H__79C2AF65_C90B_4E55_1D88_1E6D53095C0__INCLUDED_
//===========================================================================
// End of TestUtilities.hpp
//===========================================================================



