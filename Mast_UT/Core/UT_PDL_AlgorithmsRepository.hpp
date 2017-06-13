//===========================================================================
//                           UT_PDL_AlgorithmsRepository.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PDL_AlgorithmsRepository.hpp
//!
//! Declares test fixture class for testing PDL_AlgorithmsRepository
//!
//===========================================================================


#ifndef UT_PDL_ALGORITHMSREPOSITORY_H__9E241B0E_A2C7_4971_F7A0_19433A6EA923__INCLUDED_
  #define UT_PDL_ALGORITHMSREPOSITORY_H__9E241B0E_A2C7_4971_F7A0_19433A6EA923__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class PDL_AlgorithmsRepository
//!
class UT_PDL_AlgorithmsRepository : public CxxTest::TestSuite
{
  public:
   UT_PDL_AlgorithmsRepository() = default;
  ~UT_PDL_AlgorithmsRepository() = default;

  // ================ Test Methods ================

  // ---------------- Instance
  //
  void test_Instance ();

  // ---------------- RegisterAlgorithm
  //
  void test_RegisterAlgorithm_FirstOne ();
  void test_RegisterAlgorithm_SecondOne ();
  void test_RegisterAlgorithm_Overload ();

  // ---------------- GetAlgorithm
  //
  void test_GetAlgorithm_0 ();
  void test_GetAlgorithm_1_Exists ();
  void test_GetAlgorithm_1_not_Exists ();
  void test_GetAlgorithm_3_Exists ();
  void test_GetAlgorithm_3_not_Exists ();
};
//
//  End of UT_PDL_AlgorithmsRepository class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PDL_ALGORITHMSREPOSITORY_H__9E241B0E_A2C7_4971_F7A0_19433A6EA923__INCLUDED_
//===========================================================================
// End of UT_PDL_AlgorithmsRepository.hpp
//===========================================================================
