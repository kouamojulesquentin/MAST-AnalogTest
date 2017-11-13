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

  // ---------------- Register, Chain, MIB, SIB...
  //
  void test_register_Success ();
  void test_register_Error ();
  void test_chain ();
  void test_MIB ();
  void test_SIB ();

  // ---------------- JTAG TAP
  //
  void test_JTAG_TAP_Success ();
  void test_JTAG_TAP_Failure ();

  // ---------------- AccessInterface
  //
  void test_ACCES_INTERFACE_Success ();
  void test_ACCES_INTERFACE_Failure ();

  // ---------------- 1500
  //
  void test_1500 ();

  // ---------------- Linker
  //
  void test_LINKER_Success ();
  void test_LINKER_Error ();
  void test_LINKER_CustomTable_Success ();

  // ---------------- PDL Statement
  //
  void test_PDL_Success ();
  void test_PDL_Failure ();

  // ---------------- INSTANCE_OF
  //
  void test_INSTANCE_OF_Single_Success ();
  void test_INSTANCE_OF_Multiple_Success ();
  void test_INSTANCE_OF_Failure ();

  // ---------------- Other methods
  //
  void setUp();    //!< Initializes test (called for each test)
  void tearDown(); //!< Cleanups test (called for each test)
};
//
//  End of UT_reader class declaration
//---------------------------------------------------------------------------

#endif
//===========================================================================
// End of UT_reader.hpp
//===========================================================================
