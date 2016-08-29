//===========================================================================
//                           UT_reader.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinterVisitor.hpp
//!
//! Declares test fixture class for testing GmlPrinterVisitor
//!
//===========================================================================


#ifndef UT_reader_H_INCLUDED_
  #define UT_reader_H__INCLUDED_

#include <cxxtest/TestSuite.h>
#include <memory>

namespace mast
{
  class SystemModel;
} // End of namespace mast

//! Unit tests for class SystemModelBuilder SIT_Reader
//!
class UT_reader : public CxxTest::TestSuite
{
  public:
   UT_reader() = default;
  ~UT_reader() = default;

  // ---------------- Tests
  //
  void test_register ();
  void test_chain ();
  void test_MIB ();
  void test_SIB ();
  void test_JTAG_TAP ();
  void test_1500 ();

  // ---------------- Private  Methods
  //
  void setUp(); //!< Initializes test (called for each test)

  std::shared_ptr<mast::SystemModel> sm;
};
//
//  End of UT_reader class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_GMLPRINTERVISITOR_H_INCLUDED_
//===========================================================================
// End of UT_reader.hpp
//===========================================================================
