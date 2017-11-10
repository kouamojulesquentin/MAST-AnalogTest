//===========================================================================
//                           UT_PathSelectorFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PathSelectorFactory.hpp
//!
//! Declares test fixture class for testing PathSelectorFactory
//!
//===========================================================================


#ifndef UT_PATHSELECTORFACTORY_H__A3F572D8_ED70_4208_FA0_50CE2D5972D1__INCLUDED_
  #define UT_PATHSELECTORFACTORY_H__A3F572D8_ED70_4208_FA0_50CE2D5972D1__INCLUDED_
#include <cxxtest/TestSuite.h>

//! Unit tests for class PathSelectorFactory
//!
class UT_PathSelectorFactory : public CxxTest::TestSuite
{
  public:
           UT_PathSelectorFactory() = default;
  virtual ~UT_PathSelectorFactory() = default;

  // ================ Test Methods ================

  // ---------------- Instance
  //
  void test_Instance ();

  // ---------------- Create
  //
  void test_Create_Std_Success ();
  void test_Create_Std_Success_CanSelectNone ();
  void test_Create_Std_Id_Error ();
  void test_Create_Std_Param_Error ();

  void test_Create_Custom_Success ();
//+  void test_Create_Custom_Id_Error ();
//+  void test_Create_Custom_Param_Error ();

  // ---------------- Factory Management
  //
  void test_Clear ();
  void test_InitializeWithDefaults ();

  void test_RegisterCreator_NewOne_when_Defaults ();
  void test_RegisterCreator_NewOne_when_None ();
  void test_RegisterCreator_Replace_Default ();
};
//
//  End of UT_PathSelectorFactory class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PATHSELECTORFACTORY_H__A3F572D8_ED70_4208_FA0_50CE2D5972D1__INCLUDED_
//===========================================================================
// End of UT_PathSelectorFactory.hpp
//===========================================================================
