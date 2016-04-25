//===========================================================================
//                           UT_PointerAsserts_Failures.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_PointerAsserts_Failures.cpp
//!
//! Implements test fixture for testing PointerAsserts_Failures
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/28/2015
//===========================================================================



#include "UT_PointerAsserts_Failures.h"
#include <cxxtest/ValueTraits.h>
#include <memory>

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;


namespace
{
  int* getPointerThrowing()
  {
    static int value = 0xdead;
    throw "Hello from getPointerThrowing() !!!";
    return &value;
  }

  void justThrowSomething() { throw "This was thrown from justThrowSomething()."; }
} // End of unnamed namespace

UT_PointerAsserts_Failures::UT_PointerAsserts_Failures ()
{
}

UT_PointerAsserts_Failures::~UT_PointerAsserts_Failures ()
{
}


//! Checks TS_ASSERT_NULLPTR() when passing not nullptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NULLPTR ()
{
  int value = 5;
  TS_ASSERT_NULLPTR(&value);
}


//! Checks TS_ASSERT_NULLPTR() when passing valid shared_ptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NULLPTR_shared_ptr ()
{
  TS_ASSERT_NULLPTR(make_shared<int>());
}

//! Checks TS_ASSERT_NULLPTR() when passing a valid unique_ptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NULLPTR_unique_ptr ()
{
  TS_ASSERT_NULLPTR(make_unique<int>());
}

//! Checks TS_ASSERT_NULLPTR() when an exception is thrown inside the macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NULLPTR_Exception_inside ()
{
  TS_ASSERT_NULLPTR(getPointerThrowing());
}

//! Checks TS_ASSERT_NULLPTR() when an exception is thrown before the macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NULLPTR_Exception_before ()
{
  justThrowSomething();
  TS_ASSERT_NULLPTR(nullptr);
}

//! Checks TS_ASSERT_NULLPTR() when an exception is thrown after the macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NULLPTR_Exception_after ()
{
  TS_ASSERT_NULLPTR(nullptr);
  justThrowSomething();
}


//! Checks TS_ASSERT_NOT_NULLPTR() when passing a nullptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NOT_NULLPTR ()
{
  int* pValue = NULL;
  TS_ASSERT_NOT_NULLPTR(pValue);
}


//! Checks TS_ASSERT_NOT_NULLPTR() when passing an uninitialized shared_ptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NOT_NULLPTR_shared_ptr ()
{
  shared_ptr<int> nullSharedPtr;
  TS_ASSERT_NOT_NULLPTR(nullSharedPtr);
}

//! Checks TS_ASSERT_NOT_NULLPTR() when passing a unique_ptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NOT_NULLPTR_unique_ptr ()
{
  unique_ptr<int> nullUniquePtr;
  TS_ASSERT_NOT_NULLPTR(nullUniquePtr);
}


//! Checks TS_ASSERT_NOT_NULLPTR() when an exception is thrown inside the macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NOT_NULLPTR_Exception_inside ()
{
  TS_ASSERT_NOT_NULLPTR(getPointerThrowing());
}

//! Checks TS_ASSERT_NOT_NULLPTR() when an exception is thrown before the macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NOT_NULLPTR_Exception_before ()
{
  justThrowSomething();
  TS_ASSERT_NOT_NULLPTR(make_shared<double>());
}

//! Checks TS_ASSERT_NOT_NULLPTR() when an exception is thrown after the macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_NOT_NULLPTR_Exception_after ()
{
  TS_ASSERT_NOT_NULLPTR(make_unique<float>());
  justThrowSomething();
}


//! Checks TS_ASSERT_EQUALS_PTR() when first pointer is a nullptr and second pointer is not
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_EQUALS_PTR_When_First_NullPtr ()
{
  int  value;
  int* pValue_1 = NULL;
  int* pValue_2 = &value;
  TS_ASSERT_EQUALS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_EQUALS_PTR() when first pointer is a not nullptr and second pointer is
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_EQUALS_PTR_When_First_Not_NullPtr ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = NULL;
  TS_ASSERT_EQUALS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_EQUALS_PTR() when there is an exception thrown within the assertion macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_EQUALS_PTR_Exception_inside ()
{
  int  value;
  int* pValue_1 = &value;
  TS_ASSERT_EQUALS_PTR (pValue_1, getPointerThrowing());
}

//! Checks TS_ASSERT_EQUALS_PTR() when there is an exception thrown before the assertion macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_EQUALS_PTR_Exception_before ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = NULL;

  justThrowSomething();
  TS_ASSERT_EQUALS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_EQUALS_PTR() when there is an exception thrown after the assertion macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_EQUALS_PTR_Exception_after ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = NULL;

  TS_ASSERT_EQUALS_PTR (pValue_1, pValue_2);
  justThrowSomething();
}

//! Checks TS_ASSERT_DIFFERS_PTR() when both pointers are nullptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_DIFFERS_PTR_When_Both_NullPtr ()
{
  int* pValue_1 = NULL;
  int* pValue_2 = NULL;
  TS_ASSERT_DIFFERS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_DIFFERS_PTR() when both pointers are not nullptr
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_DIFFERS_PTR_When_Both_Not_NullPtr ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = &value;
  TS_ASSERT_DIFFERS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_DIFFERS_PTR() when there is an exception thrown within the assertion macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_DIFFERS_PTR_Exception_inside ()
{
  int  value;
  int* pValue_1 = &value;
  TS_ASSERT_DIFFERS_PTR (pValue_1, getPointerThrowing());
}

//! Checks TS_ASSERT_DIFFERS_PTR() when there is an exception thrown before the assertion macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_DIFFERS_PTR_Exception_before ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = NULL;

  justThrowSomething();
  TS_ASSERT_DIFFERS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_DIFFERS_PTR() when there is an exception thrown after the assertion macro
//!
void UT_PointerAsserts_Failures::test_TS_ASSERT_DIFFERS_PTR_Exception_after ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = NULL;

  TS_ASSERT_DIFFERS_PTR (pValue_1, pValue_2);
  justThrowSomething();
}


//===========================================================================
// End of UT_PointerAsserts_Failures.cpp
//===========================================================================
