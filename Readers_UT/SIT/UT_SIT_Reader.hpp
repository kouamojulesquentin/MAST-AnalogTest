//===========================================================================
//                           UT_SIT_Reader.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SIT_Reader.hpp
//!
//! Declares test fixture class for testing SIT_Reader
//!
//===========================================================================

#ifndef UT_SIT_READER_H__2DBB5164_B335_40CB_5B9F_2D488C53F0F2__INCLUDED_
  #define UT_SIT_READER_H__2DBB5164_B335_40CB_5B9F_2D488C53F0F2__INCLUDED_

#include <cxxtest/TestSuite.h>
#include <memory>

//! Unit tests for class SIT_Reader
//!
class UT_SIT_Reader : public CxxTest::TestSuite
{
  public:
   UT_SIT_Reader() = default;
  ~UT_SIT_Reader() = default;

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
//  End of UT_SIT_Reader class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SIT_READER_H__2DBB5164_B335_40CB_5B9F_2D488C53F0F2__INCLUDED_
//===========================================================================
// End of UT_SIT_Reader.hpp
//===========================================================================
