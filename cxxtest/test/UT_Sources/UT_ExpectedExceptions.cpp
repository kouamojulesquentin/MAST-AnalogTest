//===========================================================================
//                           UT_ExpectedExceptions.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_ExpectedExceptions.cpp
//!
//! Implements test fixture for testing assertions expecting exceptions
//===========================================================================


#include "UT_ExpectedExceptions.h"
#include <stdexcept>

namespace
{
  void doNotThrow() {}

  //! Throws and exception instead of returning an int
  //!
  int doThrowInvalidArgument ()
  {
      throw std::invalid_argument("You are dead !!!");
      return 0xDEAD;
  }

  //! Throws and exception instead of returning an int
  //!
  int doThrowRuntimeError ()
  {
      throw std::runtime_error ("This is runtime_error !!!");
      return 0xDEAD;
  }

  //! Throws an unknown type of exception
  //!
  int doThrowUnknown ()
  {
      class MyException{};

      throw MyException ();
      return 0xDEAD;
  }

  //! Throws an int exception
  //!
  int doThrowInt ()
  {
      throw int(-12);
      return 0xDEAD;
  }


} // End of unnamed namespace


UT_ExpectedExceptions::UT_ExpectedExceptions ()
{
}

UT_ExpectedExceptions::~UT_ExpectedExceptions ()
{
}


//! Runs TS_ASSERT_THROWS while throwing proper exception
//!
void UT_ExpectedExceptions::test_Expected_Success ()
{
    TS_ASSERT_THROWS (doThrowInvalidArgument(), std::invalid_argument);
}


//! Runs TS_ASSERT_THROWS while not throwing any exception
//!
void UT_ExpectedExceptions::test_Expected_Failure_NoException ()
{
    TS_ASSERT_THROWS (doNotThrow(), std::exception);
}

//! Runs TS_ASSERT_THROWS while throwing another type of exception
//!
void UT_ExpectedExceptions::test_Expected_Failure_StdException ()
{
    TS_ASSERT_THROWS (doThrowRuntimeError(), std::invalid_argument);
}


//! Runs TS_ASSERT_THROWS while throwing unknown type of exception
//!
void UT_ExpectedExceptions::test_Expected_Failure_UnknownException ()
{
    TS_ASSERT_THROWS (doThrowUnknown(), std::invalid_argument);
}


//! Runs TS_ASSERT_THROWS while throwing an int exception
//!
void UT_ExpectedExceptions::test_Expected_Failure_IntException ()
{
    TS_ASSERT_THROWS (doThrowInt(), std::invalid_argument);
}


//! Runs TS_ASSERT_THROWS while throwing an unsigned int exception
//!
void UT_ExpectedExceptions::test_Expected_Failure_UIntException ()
{
    TS_ASSERT_THROWS (throw 36u, std::invalid_argument);
}


//! Runs TS_ASSERT_THROWS while throwing a bool exception with value true
//!
void UT_ExpectedExceptions::test_Expected_Failure_BoolException_true ()
{
    TS_ASSERT_THROWS (throw true, std::invalid_argument);
}


//! Runs TS_ASSERT_THROWS while throwing a bool exception with value false
//!
void UT_ExpectedExceptions::test_Expected_Failure_BoolException_false ()
{
    TS_ASSERT_THROWS (throw false, std::invalid_argument);
}

//===========================================================================
// End of UT_ExpectedExceptions.cpp
//===========================================================================
