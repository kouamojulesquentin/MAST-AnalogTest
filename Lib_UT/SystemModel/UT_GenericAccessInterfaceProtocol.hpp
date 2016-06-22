//===========================================================================
//                           UT_GenericAccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GenericAccessInterfaceProtocol.hpp
//!
//! Declares test fixture class for testing GenericAccessInterfaceProtocol
//!
//===========================================================================


#ifndef UT_GENERICACCESSINTERFACEPROTOCOL_H__942464C3_F4BB_473E_C9B9_772C545C6092__INCLUDED_
  #define UT_GENERICACCESSINTERFACEPROTOCOL_H__942464C3_F4BB_473E_C9B9_772C545C6092__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class GenericAccessInterfaceProtocol
//!
//!
class UT_GenericAccessInterfaceProtocol : public CxxTest::TestSuite
{
  public:
   UT_GenericAccessInterfaceProtocol() = default;
  ~UT_GenericAccessInterfaceProtocol() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor_Initializer_List ();
  void test_Constructor_Initializer_List_TooFew_Primitives ();
  void test_Constructor_Initializer_List_TooFew_Actions ();
  void test_Constructor_Vector ();
  void test_Constructor_Vector_TooFew_Primitives ();
  void test_Constructor_Vector_TooFew_Actions ();

  // ---------------- DoAction
  //
  void test_DoAction_0 ();
  void test_DoAction_1 ();
  void test_DoAction_2 ();
  void test_DoAction_3 ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_GenericAccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_GENERICACCESSINTERFACEPROTOCOL_H__942464C3_F4BB_473E_C9B9_772C545C6092__INCLUDED_
//===========================================================================
// End of UT_GenericAccessInterfaceProtocol.hpp
//===========================================================================
