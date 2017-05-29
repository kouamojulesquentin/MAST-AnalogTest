//===========================================================================
//                           UT_YamlLib.hpp
//===========================================================================
//
//! @file UT_YamlLib.hpp
//!
//! Declares test fixture class for testing YamlLib
//!
//===========================================================================



#ifndef UT_YAMLLIB_H__117CF4D0_3A74_4884_D08A_21CC1C0CE2A__INCLUDED_
  #define UT_YAMLLIB_H__117CF4D0_3A74_4884_D08A_21CC1C0CE2A__INCLUDED_

#include <cxxtest/TestSuiteEx.h>

//! Unit tests for Yaml library
//!
class UT_YamlLib : public CxxTest::TestSuite
{
  public:

  // ================ Test Methods ================

  // ---------------- Load
  //
  void test_Load_String_Empty ();

  // ---------------- Subscript Operator
  //
  void test_Subscript_Operator_Scalar_String ();
  void test_Subscript_Operator_Scalar_Int ();
  void test_Subscript_Operator_KeyWithoutValue ();
  void test_Subscript_Operator_NotExistingKey ();

  void test_Load_String_1_Sequence ();
  void test_Load_String_N_Sequence ();
  void test_Load_String_Complex_StringItem ();
  void test_Load_String_SyntaxError ();

  // ---------------- Emitter
  //
  void test_Emitter_Empty ();
  void test_Emitter_String ();
  void test_Emitter_Int ();
  void test_Emitter_Double ();
  void test_Emitter_Sequence ();
  void test_Emitter_Map ();
  void test_Emitter_Anchor ();

  // ---------------- Value Change
  //
  void test_Assignment_String ();
  void test_Assignment_Int ();
  void test_Assignment_Double ();
  void test_Assignment_Float ();
  void test_Assignment_Sequence_Vector ();
  void test_Assignment_Sequence_Map ();
  void test_Assignment_Sequence_I_List ();
  void test_Assignment_Sequence_I_List_of_Vector ();

  // ---------------- push_back;
  //
  void test_PushBack_Vector ();
  void test_PushBack_Map ();
  void test_PushBack_I_List ();

  // ---------------- Path build up
  //
  void test_PathBuilding_Map_1st_Nodes ();
  void test_PathBuilding_Map_2nd_Nodes ();
  void test_PathBuilding_Map_3rd_Nodes ();

  // ---------------- Node Copy
  //
  void test_NodeCopy ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_YamlLib class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_YAMLLIB_H__117CF4D0_3A74_4884_D08A_21CC1C0CE2A__INCLUDED_
//===========================================================================
// End of UT_YamlLib.hpp
//===========================================================================
