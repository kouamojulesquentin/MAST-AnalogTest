//===========================================================================
//                           UT_SystemModelCheckerVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelCheckerVisitor.hpp
//!
//! Declares test fixture class for testing SystemModelCheckerVisitor
//!
//===========================================================================


#ifndef UT_SYSTEMMODELCHECKERVISITOR_H__7B7782B0_5B71_4ED4_F38E_2389907371B__INCLUDED_
  #define UT_SYSTEMMODELCHECKERVISITOR_H__7B7782B0_5B71_4ED4_F38E_2389907371B__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelCheckerVisitor
//!
//!
class UT_SystemModelCheckerVisitor : public CxxTest::TestSuite
{
  public:
   UT_SystemModelCheckerVisitor() = default;
  ~UT_SystemModelCheckerVisitor() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor_From_EmptyModel ();
  void test_Constructor_From_ModelWithTap ();

  // ---------------- CheckIdentifiers
  //
  void test_CheckIdentifiers_With_EmptyModel ();
  void test_CheckIdentifiers_With_ModelWithTap ();
  void test_CheckIdentifiers_With_UnusedIdentifier ();

  // ---------------- CheckTree
  //
  void test_CheckTree_With_EmptyModel ();
  void test_CheckTree_With_ModelWithTap ();
  void test_CheckTree_ParentWithoutChild ();
  void test_CheckTree_UnmanagedNode ();
  void test_CheckTree_UnreachableNode ();
  void test_CheckTree_NodeAppendedTwice ();
  void test_CheckTree_NodeAppendedTwice_SameParent_1 ();
  void test_CheckTree_NodeAppendedTwice_SameParent_2 ();
  void test_CheckTree_NodeAppended_ToSelf_Last();
  void test_CheckTree_NodeAppended_ToSelf_Middle();

  void test_CheckTree_When_Linker_Less_Children ();
  void test_CheckTree_When_Linker_More_Children ();
  void test_CheckTree_When_MaxPath_Zero ();

  void test_Check_When_NoAccessInterface ();
  void test_Check_When_RootIsAccessInterface ();
  void test_Check_When_RootIsChainWithAccessInterface ();
  void test_Check_When_RootIsChainWithAccessInterfaces ();
  void test_Check_When_RootIsChainWithoutAccessInterface ();
  void test_Check_When_RootIsChainWithMixKindChildren ();
//+  void test_Check_When_RootIsAccessInterface_and_AnotherBellow ();

  void test_Check_When_AccessInterface_has_MoreDerivations ();
  void test_Check_When_AccessInterface_has_LessDerivations ();

  void test_Check_SameNames (); // Other name issues are tested by UT_NamesChecker

  void test_Check_NoProtocol ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SystemModelCheckerVisitor class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SYSTEMMODELCHECKERVISITOR_H__7B7782B0_5B71_4ED4_F38E_2389907371B__INCLUDED_
//===========================================================================
// End of UT_SystemModelCheckerVisitor.hpp
//===========================================================================
