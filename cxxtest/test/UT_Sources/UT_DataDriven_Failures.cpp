//===========================================================================
//                           UT_DataDriven_Failures.cpp
//===========================================================================
// Copyright © 2015. All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_DataDriven_Failures.cpp
//!
//! Implements test fixture for testing DataDriven
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/16/2015
//===========================================================================



#include "UT_DataDriven_Failures.h"
#include <cxxtest/ValueTraits.h>
#include <vector>
#include <tuple>

using std::vector;
using std::tuple;
using std::make_tuple;
using std::make_pair;
using std::get;

namespace
{
  //! Unary functor that returns true when the value is odd
  //!
  class IsOddPredicate
  {
      public: bool operator()(int x) const
      {
          return x % 2 != 0;
      }
  };

  //! "Standalone function to use for parameterized test"
  //!
  void StandaloneTestFunction (int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  }
}



//! "Standalone function to use for parameterized test"
//!
void UT_DataDriven_Failures::StaticMemberTestFunction (int input, int expected)
{
  auto computedValue = 3 * input;
  TS_ASSERT_EQUALS (computedValue, expected);
}

//! Checks data driven assertion when input and expected results have not the same count of elements
//!
void UT_DataDriven_Failures::test_ParameterCountMismatch ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  std::vector<int> inputs          {0, 1, 2, 5, -1, -1000};
  std::vector<int> expectedResults {0, 3, 6, 12, 3};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when input and expected results are arrays of int
//!
void UT_DataDriven_Failures::test_Array_Int_Int ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  int inputs[]          = {1, 2, 5,  -1, -1000};
  int expectedResults[] = {3, 6, 12, -3, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}


//! Checks data driven assertion when input and expected results are initializers of int
//!
void UT_DataDriven_Failures::test_Initializer_Int_Int ()
{
  // ---------------- DDT Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {1, 3, 7, -3000};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when input and expected results are vectors int
//!
void UT_DataDriven_Failures::test_Vector_Int_Int ()
{
  // ---------------- Prepare test
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  std::vector<int> inputs          {0, 1, 2, 5, -1, -1000, -1};
  std::vector<int> expectedResults {0, 3, 6, 12, 3, -3000,  3};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when inputs are std::string and expected results are int
//!
void UT_DataDriven_Failures::test_StdString_Int ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input, int expectedLength)
  {
    TS_ASSERT_EQUALS (input.length(), expectedLength);
  };

  auto inputs            = {"", "Hi" , "Hello World", "How are you?"};
  int  expectedResults[] = { 0,     2,             7,            -30};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testLength, inputs, expectedResults);
}

//! Checks data driven assertion when inputs are std::string and expected results are compound of bool and int
//!
void UT_DataDriven_Failures::test_StdString_Compound ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input, const tuple<int, bool>& expected)
  {
    auto inputLength = input.length();
    auto isOdd       = (inputLength % 2) == 1;

    TS_ASSERT_EQUALS (inputLength, get<int>(expected));
    TS_ASSERT_EQUALS (isOdd,       get<bool>(expected));
  };

  auto inputs          = {"",                  "Hi",                  "Hello World",        "How are you?"};
  auto expectedResults = {make_tuple(0, false), make_tuple(2, true),  make_tuple(9, true),  make_tuple(-30, true)};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testLength, inputs, expectedResults);
}

//! Checks that data driven assertion can use a standalone function for parameterized test function
//!
void UT_DataDriven_Failures::test_Standalone_Function ()
{
  // ---------------- DDT Setup
  //
  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {1, 3, 7, -3000};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(StandaloneTestFunction, inputs, expectedResults);
}

//! Checks that data driven assertion can use a static member function for parameterized test function
//!
void UT_DataDriven_Failures::test_StaticMember_Function ()
{
  // ---------------- DDT Setup
  //
  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {1, 3, 7, -3000};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(UT_DataDriven_Failures::StaticMemberTestFunction, inputs, expectedResults);
}


//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    TS_ASSERT (input == -expected);
  };

  int inputs[]          = {1,  -2, -5, -1, -1000};
  int expectedResults[] = {-1,  2, -5,  2,  3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_TRUE in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_TRUE ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    TS_ASSERT_TRUE (input == -expected);
  };

  int inputs[]          = {1,  -2, -5, -1, -1000};
  int expectedResults[] = {-1,  2, -5,  2,  3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}


//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_FALSE in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_FALSE ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    TS_ASSERT_FALSE (input == -expected);
  };

  int inputs[]          = { 1, -2, -5, -1, -1000};
  int expectedResults[] = {-1,  2, -5,  2,  3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}



//! Test TS_DATA_DRIVEN_TEST when using TSA_ASSERT_EQUALS in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_EQUALS_AUTO_Value ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    TSA_ASSERT_EQUALS (input, -expected);
  };

  auto inputs          = { 1, -2, -5, -1, -1000};
  auto expectedResults = {-1,  2, -5,  2,  3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Test TS_DATA_DRIVEN_TEST when using TSA_ASSERT_EQUALS with pointers in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_EQUALS_AUTO_Pointer ()
{
  // ---------------- Setup
  //
  auto testFunction = [](auto pValue, auto pExpected)
  {
    TSA_ASSERT_EQUALS (pValue, pExpected);
  };

  int value_1 = 1;
  int value_2 = 2;
  int value_3 = 3;
  int value_4 = 4;

  int* inputs[]          = {&value_1, &value_2, nullptr,  &value_4 };
  int* expectedResults[] = {&value_1, &value_4, &value_3, &value_2 };

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}


//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_DIFFERS in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_DIFFERS ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    TS_ASSERT_DIFFERS (input, expected);
  };

  auto inputs          = {1, 2, -5, 1, -1000};
  auto expectedResults = {1, 2,  5, 2,  3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}



//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_GREATER_THAN in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_GREATER_THAN_Equal ()
{
  // ---------------- Setup
  //
  auto testFunction = [](double input, double expected)
  {
    TS_ASSERT_GREATER_THAN (input, expected);
  };

  auto inputs          = {1.2, 3.0, -5.4, 1.0, -1000.};
  auto expectedResults = {1.4, 2.1, -5.5, 0.2,  3000.};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_GREATER_THAN_EQUALS in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_GREATER_THAN_EQUAL_GreaterThan ()
{
  // ---------------- Setup
  //
  auto testFunction = [](double input, double expected)
  {
    TS_ASSERT_GREATER_THAN_EQUALS (input, expected);
  };

  auto inputs          = {1.4, 3.0, -5.4, 1.0, -1000.};
  auto expectedResults = {1.4, 2.1, -5.5, 0.2,  3000.};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}


//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_LESS_THAN in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_LESS_THAN_Equal ()
{
  // ---------------- Setup
  //
  auto testFunction = [](double input, double expected)
  {
    TS_ASSERT_LESS_THAN (input, expected);
  };

  auto inputs          = {1.2, 3.0, -5.4, 1.0, -1000.};
  auto expectedResults = {1.4, 2.1, -5.5, 0.2,  3000.};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_LESS_THAN_EQUALS in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_LESS_THAN_EQUAL_GreaterThan ()
{
  // ---------------- Setup
  //
  auto testFunction = [](double input, double expected)
  {
    TS_ASSERT_LESS_THAN_EQUALS (input, expected);
  };

  auto inputs          = {1.4, 3.0, -5.4, 1.0, -1000.};
  auto expectedResults = {1.4, 2.1, -5.5, 0.2,  3000.};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_DELTA in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_DELTA ()
{
  // ---------------- Setup
  //
  auto testFunction = [](double input, double expected)
  {
    constexpr auto acceptedDelta = 0.5;
    TS_ASSERT_DELTA (input, expected, acceptedDelta);
  };

  auto inputs          = {1.4, 3.0, -5.4, 1.0, -1.5};
  auto expectedResults = {1.4, 2.6, -5.5, 0.4, -2.1};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}




//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_SAME_DATA in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_SAME_DATA ()
{
  auto testFunction = [](auto lhs, auto rhs)
  {
    TS_ASSERT_SAME_DATA(lhs, rhs, 5);
  };

  auto inputs          = {"Lazy dog", "Hello", "World!", "Ho    "};
  auto expectedResults = {"Lady dog", "hello", "World",  "How   "};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks we can abort remaining data driven test when one fails
//! @note As the default is false (do not abort) this is not necessary to add specific test(s)
void UT_DataDriven_Failures::test_setAbortDataDrivenTestsOnFailure_True ()
{
  // ---------------- DDT Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  auto inputs          = {0, 1, 2, -1000};    // Last test will not be done at all
  auto expectedResults = {0, 3, 7, -3000};

  // ---------------- DDT Exercise
  //
  CxxTest::setAbortDataDrivenTestsOnFailure(true);
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}



//! Checks data driven test when expecting some exception
//!
void UT_DataDriven_Failures::test_Expected_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto throwException = [](int trigger) { if (trigger > 10) throw std::out_of_range("trigger > 10"); };
    auto inputLength    =  input.length();

    TS_ASSERT_THROWS (throwException(expectedLength), std::exception);
    TS_ASSERT_EQUALS (inputLength, expectedLength);
  };

  auto inputs          = {"", "Hello World", "Hi", "How are you?"};
  auto expectedResults = {0,  11,            2,    12};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}


//! Checks data driven test when in assertion requiring nothing is thrown bu there is an exception is thrown
//!
void UT_DataDriven_Failures::test_Throw_Nothing ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto mayThrow    = [](int trigger) { if (trigger > 10) throw "Above trigger!!!"; };
    auto inputLength = input.length();

    TSM_ASSERT_THROWS_NOTHING ("Bad",  mayThrow(expectedLength));
    TSM_ASSERT_EQUALS         ("Luck", inputLength, expectedLength);
  };

  auto inputs          = {"Hello World", "Hi", "This one is so long that it will trigger an exception of some sort !!!!"};
  auto expectedResults = {11,            2,    50};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}


//! Checks data driven test when there is an unexpected exception of unknown type
//!
void UT_DataDriven_Failures::test_Unexpected_UnknownType_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto throwException = [](int trigger) { if (trigger > 10) throw "Above trigger!!!"; };
    auto inputLength    =  input.length();

    TS_ASSERT_EQUALS (inputLength, expectedLength);
    throwException(expectedLength);
  };

  auto inputs          = {"Hello World", "Hi"};
  auto expectedResults = {11,            2};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}


//! Checks data driven test when there is an unexpected exception of type bool
//!
void UT_DataDriven_Failures::test_Unexpected_Bool_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto isOdd          = []     (int value)   { return (value % 2) == 1; };
    auto throwException = [isOdd](int trigger) { if (trigger > 10) throw isOdd(trigger); };
    auto inputLength    =  input.length();

    TS_ASSERT_EQUALS (inputLength, expectedLength);
    throwException(expectedLength);
  };

  auto inputs          = {"Hello World", "Hi", "How are you?"};
  auto expectedResults = {11,            2,    12};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}

//! Checks data driven test when there is an unexpected exception of type int
//!
void UT_DataDriven_Failures::test_Unexpected_Int_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto throwException = [](int trigger) { if (trigger > 10) throw trigger; };
    auto inputLength    =  input.length();

    TS_ASSERT_EQUALS (inputLength, expectedLength);
    throwException(expectedLength);
  };

  auto inputs          = {"Hello World", "Hi", "How are you?"};
  auto expectedResults = {11,            2,    12};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}


//! Checks data driven test when there is an unexpected std::exception
//!
void UT_DataDriven_Failures::test_Unexpected_Std_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto throwException = [](int trigger) { if (trigger > 10) throw std::out_of_range("trigger > 10"); };
    auto inputLength    =  input.length();

    TS_ASSERT_EQUALS (inputLength, expectedLength);
    throwException(expectedLength);
  };

  auto inputs          = {"", "Hello World", "Hi", "How are you?"};
  auto expectedResults = {0,  11,            2,    12};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}

//! Checks data driven assertion when inputs are int and no expected value is necessary
//!
void UT_DataDriven_Failures::test_Int ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input)
  {
    auto isOdd = [](int value) { return (value % 2) == 1; };
    TS_ASSERT_TRUE (isOdd(input));
  };

  auto inputs = {0, 1, 2, -1000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs);
}

//! Checks data driven assertion when inputs are std::string and no expected value is necessary
//!
void UT_DataDriven_Failures::test_StdString ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input)
  {
    TS_ASSERT_LESS_THAN (input.length(), 10);
  };

  auto inputs = {"", "Hi" , "Hello World", "How are you?"};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testLength, inputs);
}





//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_DELTA in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_PREDICATE ()
{
  auto testFunction = [](auto value)
  {
    TS_ASSERT_PREDICATE(IsOddPredicate(), value);
  };

  auto inputs = {0, 1, 2, -1001};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs);
}

//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_RELATION in parameterized test
//!
void UT_DataDriven_Failures::test_ASSERT_RELATION ()
{
  auto testFunction = [](auto inputSample)
  {
    auto lhs = std::get<0>(inputSample);
    auto rhs = std::get<1>(inputSample);
    TS_ASSERT_RELATION(std::equal_to<decltype(lhs)>(), lhs, rhs);
  };

  auto inputs = {make_tuple(0, 0), make_tuple(1, -1), make_tuple(4, 5), make_tuple(69, 69)};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs);
}


//! Checks data driven test when expecting some exception and no expected value is necessary
//!
void UT_DataDriven_Failures::test_SingleData_Expected_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input)
  {
    auto throwException = [](int trigger) { if (trigger > 10) throw std::out_of_range("trigger > 10"); };
    auto inputLength    =  input.length();

    TS_ASSERT_THROWS (throwException(inputLength), std::exception);
  };

  auto inputs = {"?", "Hi", "How are you?"};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs);
}

//! Checks data driven test when there is an unexpected exception from assertion specifying nothing should be thrown and no expected value is necessary
//!
void UT_DataDriven_Failures::test_SingleData_Throw_Nothing ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input)
  {
    auto mayThrow    = [](int trigger) { if (trigger > 10) throw "Above trigger!!!"; };
    auto inputLength =  input.length();

    TS_ASSERT_THROWS_NOTHING (mayThrow(inputLength));
    TS_ASSERT_GREATER_THAN   (inputLength, 5U);
  };

  auto inputs = {"Hello World", "Hi"};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs);
}

//! Checks data driven test when there is an unexpected exception of unknown type and no expected value is necessary
//!
void UT_DataDriven_Failures::test_SingleData_Unexpected_UnknownType_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input)
  {
    auto throwException = [](int trigger) { if (trigger > 10) throw "Above trigger!!!"; };
    auto inputLength    =  input.length();

    throwException(inputLength);
    TS_ASSERT_GREATER_THAN (inputLength, 5U);
  };

  auto inputs = {"Hello World", "Hi"};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs);
}



//! Checks data driven test when there is an unexpected std::exception and no expected value is necessary
//!
void UT_DataDriven_Failures::test_SingleData_Unexpected_Std_Exception ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input)
  {
    auto throwException = [](int trigger) { if (trigger > 10) throw std::out_of_range("trigger > 10"); };
    auto inputLength    =  input.length();

    TS_ASSERT_GREATER_THAN (inputLength, 5U);
    throwException(inputLength);
  };

  auto inputs = {"", "Hello World", "Hi", "How are you?"};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs);
}

//! Checks data driven assertion with mixed "input" (string) and "expected" (int)
//!
void UT_DataDriven_Failures::test_Mixed_StdString_Int ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input, int expectedLength)
  {
    TS_ASSERT_EQUALS (input.length(), expectedLength);
  };

  std::vector<std::tuple<std::string, int>> data;

  data.emplace_back(make_tuple(std::string(""),             0));   // 0
  data.emplace_back(make_tuple(std::string("Hi"),           2));   // 1
  data.emplace_back(make_tuple(std::string("Hello World"),  7));   // 2
  data.emplace_back(make_tuple(std::string("How are you?"), -30)); // 3

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST_MIX(testLength, data);
}


//! Checks data driven assertion with mixed "input" (string) and "expected" (int)
//!
void UT_DataDriven_Failures::test_Mixed_StdString_Compound ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input, const tuple<int, bool>& expected)
  {
    auto inputLength = input.length();
    auto isOdd       = (inputLength % 2) == 1;

    TS_ASSERT_EQUALS (inputLength, get<int>(expected));
    TS_ASSERT_EQUALS (isOdd,       get<bool>(expected));
  };

  auto data =
  {
    make_pair(std::string(""),             make_tuple(0, false)),
    make_pair(std::string("Hi"),           make_tuple(2, true)),
    make_pair(std::string("Hello World"),  make_tuple(9, true)),
    make_pair(std::string("How are you?"), make_tuple(7, true)),
  };

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST_MIX(testLength, data);
}



//! Checks data driven assertion when there is usage of a test macro that assert nothing before ddt
//!
void UT_DataDriven_Failures::test_WithNoAssert_Before ()
{
  TS_WARN ("Warning message for test_WithNoAssert_Before");

  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 4, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);

}

//! Checks data driven assertion when there is usage of a test macro that assert nothing afterddt
//!
void UT_DataDriven_Failures::test_WithNoAssert_After ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 4, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);

  TS_WARN ("Warning message for test_WithNoAssert_After");
}


//! Checks data driven assertion when first test does not assert on anything (e.g. due to conditional assertion )
//!
void UT_DataDriven_Failures::test_MissingAssertion_First ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    if (input != 0)
    {
      auto computedValue = 3 * input;
      TS_ASSERT_EQUALS (computedValue, expected);
    }
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 4, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when a middle test does not assert on anything (e.g. due to conditional assertion )
//!
void UT_DataDriven_Failures::test_MissingAssertion_Middle ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    if (input != 2)
    {
      auto computedValue = 3 * input;
      TS_ASSERT_EQUALS (computedValue, expected);
    }
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 4, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when last test does not assert on anything (e.g. due to conditional assertion )
//!
void UT_DataDriven_Failures::test_MissingAssertion_Last ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    if (input >= 0)
    {
      auto computedValue = 3 * input;
      TS_ASSERT_EQUALS (computedValue, expected);
    }
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 4, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}
//===========================================================================
// End of UT_DataDriven_Failures.cpp
//===========================================================================
