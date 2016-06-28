//===========================================================================
//                           UT_NodePathResolver.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_NodePathResolver.hpp
//!
//! Declares test fixture class for testing NodePathResolver
//!
//===========================================================================


#ifndef UT_NODEPATHRESOLVER_H__F002E46A_CDA_4F74_5B9_D79C91E84C85__INCLUDED_
  #define UT_NODEPATHRESOLVER_H__F002E46A_CDA_4F74_5B9_D79C91E84C85__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class NodePathResolver
//!
//!
class UT_NodePathResolver : public CxxTest::TestSuite
{
  public:
   UT_NodePathResolver() = default;
  ~UT_NodePathResolver() = default;

  // ================ Test Methods ================

  void test_testcaseModel ();

  void test_constructor ();
  void test_constructor_nullptr ();

  void test_Resolve_ValidPaths ();
  void test_Resolve_NotValidPaths ();

  void test_SetPrefix_FirstTime_Empty ();
  void test_SetPrefix_FirstTime_Dot ();

  void test_Resolve_Prefix_ValidPaths ();
  void test_Resolve_Prefix_NotValidPaths ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_NodePathResolver class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_NODEPATHRESOLVER_H__F002E46A_CDA_4F74_5B9_D79C91E84C85__INCLUDED_
//===========================================================================
// End of UT_NodePathResolver.hpp
//===========================================================================
