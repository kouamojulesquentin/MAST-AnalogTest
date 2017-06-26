//===========================================================================
//                           UT_MTQueue.h
//===========================================================================
// Copyright (C) 2015 Jean-Francois Coulon. All rights reserved.
//               2017 Michele Portolan, TIMA             
// 
// Project : C++11 blocking queues
// based on : Multi-Edit extension DLL
//
//! @file UT_MTQueue.hpp
//!
//! Declares test fixture class for testing MTQueue
//!
// Some Tags are inserted for Doxygen (JavaDoc like) doc generator.
//===========================================================================
// Created by JFC on January/28/2015
//===========================================================================

#ifndef UT_MTQUEUE_H__B810F61D_845B_4427_3BA9_8FF0DC77287D__INCLUDED_
  #define UT_MTQUEUE_H__B810F61D_845B_4427_3BA9_8FF0DC77287D__INCLUDED_

#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>


/**
*  Unit tests for class MTQueue
*/
class UT_MTQueue : public CxxTest::TestSuite
{
  public:
   UT_MTQueue();
  ~UT_MTQueue();


  // ================ Test Methods ================

  // ---------------- Constructors
  //
  void test_DefaultConstructor ();

  // ---------------- Single thread Push
  //
  void test_Push_When_Empty_No_Pop ();
  void test_Push_With_SpyItem_Copy_1 ();
  void test_Push_With_SpyItem_Copy_N ();
  void test_Push_With_SpyItem_Move_1 ();
  void test_Push_With_SpyItem_Move_N ();
  void test_Push_With_SpyItem_Emplace ();
  void test_Push_When_NotEmpty_No_Pop ();

  // ---------------- Single thread Pop
  //
  void test_Pop_Timeout_When_Empty ();
  void test_Pop_Timeout_When_1_Item ();
  void test_Pop_Timeout_When_2_Items ();
  void test_Pop_Timeout_When_2_SpyItems ();
  void test_Pop_Timeout_When_Becomes_Empty();

  // ---------------- Dual thread Push
  //
  void test_Push_When_DualThreads_Empty_No_Pop ();
  void test_Push_When_DualThreads_NotEmpty_No_Pop ();

  // ---------------- Dual thread Pop
  //
  void test_Pop_When_DualThreads_And_Empty ();
  void test_Pop_When_DualThreads_1_Item_Before ();
  void test_Pop_When_DualThreads_1_Item_After ();

  // ---------------- Multi-thread, high contentions, Push & Pop
  //
  void test_When_1_Producers_1_Consumers();
  void test_When_1_Producers_N_Consumers();
  void test_When_N_Producers_1_Consumers();
  void test_When_N_Producers_N_Consumers();
  void test_When_N_Producers_N_Consumers_SpyItems_Copy ();
  void test_When_N_Producers_N_Consumers_SpyItems_Move ();
  void test_When_N_Producers_N_Consumers_SpyItems_Emplace ();

  // ================ Other Methods ================

  //! Initializes test (called for each test)
  //!
  void setUp();

  //! Cleanups test (called for each test)
  //!
  void tearDown();
};
//
//  End of UT_MTQueue class declaration
//---------------------------------------------------------------------------




#endif  // not defined UT_MTQUEUE_H__B810F61D_845B_4427_3BA9_8FF0DC77287D__INCLUDED_
//===========================================================================
// End of UT_MTQueue.h
//===========================================================================
