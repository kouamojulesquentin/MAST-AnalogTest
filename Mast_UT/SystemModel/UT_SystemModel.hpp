//===========================================================================
//                           UT_SystemModel.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModel.hpp
//!
//! Declares test fixture class for testing SystemModel
//!
//===========================================================================


#ifndef UT_SYSTEMMODEL_H__632BF3E9_85F5_4F87_909F_73B812A860B__INCLUDED_
  #define UT_SYSTEMMODEL_H__632BF3E9_85F5_4F87_909F_73B812A860B__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModel
//!
class UT_SystemModel : public CxxTest::TestSuite
{
  public:
   UT_SystemModel() = default;
  ~UT_SystemModel() = default;


  // ================ Test Methods ================

  void test_Constructor ();
  void test_NodeWithId_When_Empty ();

  void test_CreateAccessInterface ();
  void test_CreateRegister_Without_ParentNode ();
  void test_CreateChain_Without_ParentNode ();
  void test_CreateChain_With_ParentNode ();
  void test_CreateLinker_Without_ParentNode ();
  void test_CreateLinker_With_ParentNode ();

  void test_NotAutoRoot ();
  void test_SetRoot ();

  void test_DisconnectNode_Bottom ();
  void test_DisconnectNode_Middle ();
  void test_DisconnectNode_Top ();

  void test_ReplaceRoot_NotRemoved ();
  void test_ReplaceRoot_Removed ();
  void test_ReplaceRoot_WithNullptr ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SystemModel class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SYSTEMMODEL_H__632BF3E9_85F5_4F87_909F_73B812A860B__INCLUDED_
//===========================================================================
// End of UT_SystemModel.hpp
//===========================================================================
