//===========================================================================
//                           UT_BSDL_Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_BSDL_Reader.hpp
//!
//! Declares test fixture class for testing BSDL_Reader
//!
//===========================================================================

#ifndef UT_BSDL_READER_H__A398818A_E490_4B87_41BC_6D20A56C4EF__INCLUDED_
  #define UT_BSDL_READER_H__A398818A_E490_4B87_41BC_6D20A56C4EF__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class BSDL_Reader
//!
//!
class UT_BSDL_Reader : public CxxTest::TestSuite
{
  public:
   UT_BSDL_Reader() = default;
  ~UT_BSDL_Reader() = default;

  // ================ Test Methods ================

  void test_Constructor_Default ();

  void test_Parse ();
  void test_Parse_LargeFile ();

};
//
//  End of UT_BSDL_Reader class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_BSDL_READER_H__A398818A_E490_4B87_41BC_6D20A56C4EF__INCLUDED_
//===========================================================================
// End of UT_BSDL_Reader.hpp
//===========================================================================
