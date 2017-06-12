//===========================================================================
//                           UT_ExpectedExceptions.h
//===========================================================================
//
// Project : Cxxtest
//
//! @file UT_ExpectedExceptions.h
//!
//! Declares test fixture for testing expected exceptions
//===========================================================================


#ifndef UT_EXPECTEDEXCEPTIONS_H__C1498796_FA67_430C_9288_630BF44960CF__INCLUDED_
  #define UT_EXPECTEDEXCEPTIONS_H__C1498796_FA67_430C_9288_630BF44960CF__INCLUDED_

#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>

//! Runs cxxtest asserts macros expecting exception
//!
//! This is intended to check the report with some "gold" results
//!
class UT_ExpectedExceptions : public CxxTest::TestSuite
{
    public:
     UT_ExpectedExceptions();
    ~UT_ExpectedExceptions();

    // #========================================================================================================
    // # Test Methods
    // #========================================================================================================

    // ---------------- Expected exception
    //
    void test_Expected_Success ();
    void test_Expected_Failure_NoException ();
    void test_Expected_Failure_StdException ();
    void test_Expected_Failure_UnknownException ();
    void test_Expected_Failure_IntException ();
    void test_Expected_Failure_UIntException ();
    void test_Expected_Failure_BoolException_true ();
    void test_Expected_Failure_BoolException_false ();
};
#endif  // not defined UT_EXPECTEDEXCEPTIONS_H__C1498796_FA67_430C_9288_630BF44960CF__INCLUDED_

//===========================================================================
// End of UT_ExpectedExceptions.h
//===========================================================================
