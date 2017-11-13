//===========================================================================
//                           UT_SystemModelFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelFactory.hpp
//!
//! Declares test fixture class for testing SystemModelFactory
//!
//===========================================================================


#ifndef UT_SYSTEMMODELFACTORY_H__5C05D0A4_3ED4_4D87_C68D_5561AB74F10B__INCLUDED_
  #define UT_SYSTEMMODELFACTORY_H__5C05D0A4_3ED4_4D87_C68D_5561AB74F10B__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelFactory
//!
//!
class UT_SystemModelFactory : public CxxTest::TestSuite
{
  public:
   UT_SystemModelFactory() = default;
  ~UT_SystemModelFactory() = default;

  // ================ Test Methods ================

  void test_Instance ();
  void test_RegisterCreator_NewOne_when_None ();
  void test_Clear ();
  void test_RegisterCreator_NewOne_when_One ();

  void test_Create_BadVersion ();
  void test_Create_Success ();
  void test_Create_Error ();

  void test_RegisterCreator_Replace_Previous ();


  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SystemModelFactory class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SYSTEMMODELFACTORY_H__5C05D0A4_3ED4_4D87_C68D_5561AB74F10B__INCLUDED_
//===========================================================================
// End of UT_SystemModelFactory.hpp
//===========================================================================
