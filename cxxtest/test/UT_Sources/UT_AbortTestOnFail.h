//===========================================================================
//                           UT_AbortTestOnFail.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_AbortTestOnFail.h
//!
//! Declares test fixture for testing abort on failure
//!
//===========================================================================


#ifndef UT_ABORTTESTONFAIL_H__84A09BB0_B9B9_48E6_95B6_CD7425A08F39__INCLUDED_
  #define UT_ABORTTESTONFAIL_H__84A09BB0_B9B9_48E6_95B6_CD7425A08F39__INCLUDED_


#include <cxxtest/TestSuite.h>

//! Runs cxxtest asserts macros with AbortTestOnFail true or false
//!
//! This is intended to check the report with some "gold" results
//!
class UT_AbortTestOnFail : public CxxTest::TestSuite
{
    public:
     UT_AbortTestOnFail();
    ~UT_AbortTestOnFail();

    // #========================================================================================================
    // # Test Methods
    // #========================================================================================================

    // ---------------- When AbortTestOnFail is true
    //
    void test_ASSERT_AbortTestOnFail_True ();
    void test_ASSERT_FALSE_AbortTestOnFail_True ();
    void test_ASSERT_EQUALS_AbortTestOnFail_True ();
    void test_ASSERT_DIFFERS_AbortTestOnFail_True ();
    void test_ASSERT_GREATER_THAN_AbortTestOnFail_True();
    void test_ASSERT_GREATER_THAN_EQUAL_AbortTestOnFail_True();
    void test_ASSERT_LESS_THAN_AbortTestOnFail_True();
    void test_ASSERT_LESS_THAN_EQUAL_AbortTestOnFail_True();
    void test_ASSERT_DELTA_AbortTestOnFail_True ();
    void test_ASSERT_PREDICATE_AbortTestOnFail_True ();
    void test_ASSERT_RELATION_AbortTestOnFail_True ();
    void test_ASSERT_SAME_DATA_AbortTestOnFail_True ();
    void test_ASSERT_THROWS_AbortTestOnFail_True ();
    void test_ASSERT_THROWS_ANYTHING_AbortTestOnFail_True ();
    void test_ASSERT_THROWS_NOTHING_AbortTestOnFail_True ();

    // ---------------- When AbortTestOnFail is false
    //
    void test_ASSERT_AbortTestOnFail_False ();
    void test_ASSERT_FALSE_AbortTestOnFail_False ();
    void test_ASSERT_EQUALS_AbortTestOnFail_False ();
    void test_ASSERT_DIFFERS_AbortTestOnFail_False ();
    void test_ASSERT_GREATER_THAN_AbortTestOnFail_False();
    void test_ASSERT_GREATER_THAN_EQUAL_AbortTestOnFail_False();
    void test_ASSERT_LESS_THAN_AbortTestOnFail_False();
    void test_ASSERT_LESS_THAN_EQUAL_AbortTestOnFail_False();
    void test_ASSERT_DELTA_AbortTestOnFail_False ();
    void test_ASSERT_PREDICATE_AbortTestOnFail_False ();
    void test_ASSERT_RELATION_AbortTestOnFail_False ();
    void test_ASSERT_SAME_DATA_AbortTestOnFail_False ();
    void test_ASSERT_THROWS_AbortTestOnFail_False ();
    void test_ASSERT_THROWS_ANYTHING_AbortTestOnFail_False ();
    void test_ASSERT_THROWS_NOTHING_AbortTestOnFail_False ();

    // #========================================================================================================
    // # Private  Methods
    // #========================================================================================================
    private:
    void doNothing ();
    void throwException ();
};
#endif  // not defined UT_ABORTTESTONFAIL_H__84A09BB0_B9B9_48E6_95B6_CD7425A08F39__INCLUDED_

//===========================================================================
// End of UT_AbortTestOnFail.h
//===========================================================================
