//===========================================================================
//                           UT_SystemModelNode.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelNode.hpp
//!
//! Declares test fixture class for testing SystemModelNode
//!
//===========================================================================


#ifndef UT_SYSTEMMODELNODE_H__CCE0D46D_597C_4EC8_328A_F3CADCD5D04A__INCLUDED_
  #define UT_SYSTEMMODELNODE_H__CCE0D46D_597C_4EC8_328A_F3CADCD5D04A__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelNode
//!
class UT_SystemModelNode : public CxxTest::TestSuite
{
  public:
   UT_SystemModelNode() = default;
  ~UT_SystemModelNode() = default;

  // ================ Test Methods ================

  void test_Constructor ();

  void test_AppendSibling ();
  void test_SetPending ();
  void test_ResetPending ();
  void test_SetPriority ();
  void test_SetName ();
  void test_SetApplicationData ();


  //! @todo [JFC]-[May/26/2016]: Remove "test_No_test_yet_for_Guard" method when all tests are implemented
  //!
  void test_No_test_yet_for_Guard ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SystemModelNode class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SYSTEMMODELNODE_H__CCE0D46D_597C_4EC8_328A_F3CADCD5D04A__INCLUDED_
//===========================================================================
// End of UT_SystemModelNode.hpp
//===========================================================================
