//===========================================================================
//                           UT_Fragments.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Fragments.hpp
//!
//! Declares test fixture class for testing ICL_Reader
//!
//===========================================================================


#ifndef UT_FRAGMENTS_H__959CB261_9E43_4B6B_B08F_5F5BC84D3E15__INCLUDED_
  #define UT_FRAGMENTS_H__959CB261_9E43_4B6B_B08F_5F5BC84D3E15__INCLUDED_
#include <cxxtest/TestSuite.h>
#include <memory>


//! Unit tests for class ICL_Reader in context of ICL file got with the Bison parser skeleton   skeleton
//!
class UT_Fragments : public CxxTest::TestSuite
{
  public:
   UT_Fragments() = default;
  ~UT_Fragments() = default;

  // ================ Test Methods ================


  // ---------------- Fragments
  //
  void test_Parse_Parser_Fragments ();
  void test_Parse_Parser_Fragments_with_SyntaxErrors ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_Fragments class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_FRAGMENTS_H__959CB261_9E43_4B6B_B08F_5F5BC84D3E15__INCLUDED_

//===========================================================================
// End of UT_Fragments.hpp
//===========================================================================
