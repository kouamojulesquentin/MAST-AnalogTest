//===========================================================================
//                           UT_reader.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_reader.hpp
//!
//! Declares test fixture class for testing SIT_Reader
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

//! Unit tests for class SIT_Reader
//!
class UT_reader : public CxxTest::TestSuite
{
  public:
   UT_reader() = default;
  ~UT_reader() = default;

  // ---------------- Tests
  //
  void test_register_Success ();
  void test_register_Error ();
  void test_chain ();
  void test_MIB ();
  void test_SIB ();

  void test_JTAG_TAP_Success ();
  void test_JTAG_TAP_Failure ();

  void test_ACCES_INTERFACE_Success ();
  void test_ACCES_INTERFACE_Failure ();

  void test_1500 ();
  void test_LINKER_Success ();
  void test_LINKER_Error ();

  // ---------------- Other methods
  //
  void setUp();    //!< Initializes test (called for each test)
  void tearDown(); //!< Cleanups test (called for each test)

  private:
  std::shared_ptr<mast::SystemModel> sm;
  bool m_loggerInitialState = false;      //!< To enable logger for each test
};
//
//  End of UT_reader class declaration
//---------------------------------------------------------------------------

#endif
//===========================================================================
// End of UT_reader.hpp
//===========================================================================
