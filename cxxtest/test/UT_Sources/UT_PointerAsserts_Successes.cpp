//===========================================================================
//                           UT_PointerAsserts_Successes.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_PointerAsserts_Successes.cpp
//!
//! Implements test fixture for testing PointerAsserts_Failures
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/28/2015
//===========================================================================



#include "UT_PointerAsserts_Successes.h"
#include <cxxtest/ValueTraits.h>
#include <memory>

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;


UT_PointerAsserts_Successes::UT_PointerAsserts_Successes ()
{
}

UT_PointerAsserts_Successes::~UT_PointerAsserts_Successes ()
{
}


//! Checks that TS_ASSERT_NULLPTR() when passing a nullptr
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_NULLPTR ()
{
  int* pValue = NULL;
  TS_ASSERT_NULLPTR(pValue);
}


//! Checks that TS_ASSERT_NULLPTR() when passing an uninitialized shared_ptr
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_NULLPTR_shared_ptr ()
{
  shared_ptr<int> nullSharedPtr;
  TS_ASSERT_NULLPTR(nullSharedPtr);
}

//! Checks that TS_ASSERT_NULLPTR() when passing an uninitialized unique_ptr
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_NULLPTR_unique_ptr ()
{
  unique_ptr<int> nullUniquePtr;
  TS_ASSERT_NULLPTR(nullUniquePtr);
}


//! Checks that TS_ASSERT_NOT_NULLPTR() when passing a valid pointer
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_NOT_NULLPTR ()
{
  int value = 15;
  TS_ASSERT_NOT_NULLPTR(&value);
}


//! Checks that TS_ASSERT_NOT_NULLPTR() when passing a valid shared_ptr
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_NOT_NULLPTR_shared_ptr ()
{
  TS_ASSERT_NOT_NULLPTR(make_shared<int>());
}

//! Checks that TS_ASSERT_NOT_NULLPTR() when passing a valid unique_ptr
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_NOT_NULLPTR_unique_ptr ()
{
  TS_ASSERT_NOT_NULLPTR(make_unique<bool>());
}

//! Checks TS_ASSERT_EQUALS_PTR() when both pointers are nullptr
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_EQUALS_PTR_When_Both_NullPtr ()
{
  int* pValue_1 = NULL;
  int* pValue_2 = NULL;
  TS_ASSERT_EQUALS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_EQUALS_PTR() when both pointers are not nullptr
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_EQUALS_PTR_When_Both_Not_NullPtr ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = &value;
  TS_ASSERT_EQUALS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_DIFFERS_PTR() when first pointer is a nullptr and second pointer is not
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_DIFFERS_PTR_When_First_NullPtr ()
{
  int  value;
  int* pValue_1 = NULL;
  int* pValue_2 = &value;
  TS_ASSERT_DIFFERS_PTR (pValue_1, pValue_2);
}

//! Checks TS_ASSERT_DIFFERS_PTR() when first pointer is a not nullptr and second pointer is
//!
void UT_PointerAsserts_Successes::test_TS_ASSERT_DIFFERS_PTR_When_First_Not_NullPtr ()
{
  int  value;
  int* pValue_1 = &value;
  int* pValue_2 = NULL;
  TS_ASSERT_DIFFERS_PTR (pValue_1, pValue_2);
}



//===========================================================================
// End of UT_PointerAsserts_Successes.cpp
//===========================================================================
