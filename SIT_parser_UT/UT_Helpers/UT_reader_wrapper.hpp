//===========================================================================
//                           UT_reader_wrapper.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_reader_wrapper.hpp
//!
//! Declares a common wrapping to call SIT_reader for UTs
//!
//===========================================================================


#ifndef UT_reader_wrapper_H_INCLUDED_
  #define UT_reader_wrapper_H__INCLUDED_

#include <string>
using std::string;

namespace test 
{
//! reader Unit tests
//!
//!
 class UT_reader_wrapper final
 {
  public:
   UT_reader_wrapper() = default;
  ~UT_reader_wrapper() = default;

  // ---------------- Constructor
  //
  string run_parser_for_UT(string input_SIT);

  // ---------------- Private  Methods
  //
  void setUp(); //!< Initializes test (called for each test)
 };
//
//  End of UT_reader_wrapper class declaration
//---------------------------------------------------------------------------

}
#endif  // not defined UT_reader_wrapper_H_INCLUDED_
//===========================================================================
// End of UT_reader_wrapper.hpp
//===========================================================================
