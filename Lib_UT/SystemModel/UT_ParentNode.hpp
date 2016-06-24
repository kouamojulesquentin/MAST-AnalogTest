//===========================================================================
//                           UT_ParentNode.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ParentNode.hpp
//!
//! Declares test fixture class for testing ParentNode
//!
//===========================================================================


#ifndef UT_PARENTNODE_H__D848F979_ADBD_41E1_3AAC_6ABB5CD8A19E__INCLUDED_
  #define UT_PARENTNODE_H__D848F979_ADBD_41E1_3AAC_6ABB5CD8A19E__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class ParentNode
//!
//!
class UT_ParentNode : public CxxTest::TestSuite
{
  public:
   UT_ParentNode() = default;
  ~UT_ParentNode() = default;

  // ================ Test Methods ================

  void test_constructor ();

  // ---------------- Appending
  //
  void test_AppendChild_1 ();
  void test_AppendChild_2 ();

  void test_SetChildAppender ();

  // ---------------- Disconnection
  //
  void test_DisconnectDerivation_1st_OutOf_1 ();
  void test_DisconnectDerivation_1st_OutOf_2 ();
  void test_DisconnectDerivation_2nd_OutOf_2 ();
  void test_DisconnectDerivation_2nd_OutOf_3 ();
  void test_DisconnectDerivation_3rd_OutOf_2 ();
  void test_DisconnectDerivation_0_OutOf_2 ();
  void test_DisconnectDerivation_0_OutOf_0 ();
  void test_DisconnectDerivation_1_OutOf_0 ();

  void test_DisconnectChild_1st_OutOf_1 ();
  void test_DisconnectChild_1st_OutOf_2 ();
  void test_DisconnectChild_2nd_OutOf_2 ();
  void test_DisconnectChild_2nd_OutOf_3 ();
  void test_DisconnectChild_NotAChild ();
  void test_DisconnectChild_NoChild ();
  void test_DisconnectChild_Nullptr ();

  void test_DisconnectAllChildren ();
  void test_DisconnectAllChildren_When_None ();

  // ---------------- Parenthood
  //
  void test_HasDirectChild_1st_OutOf_1 ();
  void test_HasDirectChild_1st_OutOf_2 ();
  void test_HasDirectChild_2nd_OutOf_2 ();
  void test_HasDirectChild_2nd_OutOf_3 ();
  void test_HasDirectChild_NotAChild ();
  void test_HasDirectChild_NoChild ();
  void test_HasDirectChild_Nullptr ();

  void test_FindParentOfNode_NotAShared_ptr ();
  void test_FindParentOfNode_DirectChild ();
  void test_FindParentOfNode_Level_2_Child ();
  void test_FindParentOfNode_Level_3_Child ();
  void test_FindParentOfNode_Level_4_Child ();
  void test_FindParentOfNode_NotAChild ();
  void test_FindParentOfNode_NoChild ();
  void test_FindParentOfNode_Nullptr ();

  // ================ Other Methods ================
};
//
//  End of UT_ParentNode class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PARENTNODE_H__D848F979_ADBD_41E1_3AAC_6ABB5CD8A19E__INCLUDED_
//===========================================================================
// End of UT_ParentNode.hpp
//===========================================================================
