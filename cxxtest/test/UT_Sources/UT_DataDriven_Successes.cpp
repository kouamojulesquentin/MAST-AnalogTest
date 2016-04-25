//===========================================================================
//                           UT_DataDriven_Successes.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_DataDriven_Successes.cpp
//!
//! Implements test fixture for testing DataDriven
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/16/2015
//===========================================================================



#include "UT_DataDriven_Successes.h"
#include <cxxtest/ValueTraits.h>
#include <vector>
#include <tuple>
//+#include <memory>

using std::tuple;
using std::make_tuple;
using std::make_pair;



//! Increments call to setup
//!
void UT_DataDriven_Successes::setUp ()
{
  ++m_setupCount;
}

//! Increments call to tearDown
//!
void UT_DataDriven_Successes::tearDown ()
{
  ++m_teardownCount;
}

//! Checks data driven assertion when input and expected results are arrays of int
//!
void UT_DataDriven_Successes::test_Array_Int_Int ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  int inputs[]          = {1, 2, -1, -1000};
  int expectedResults[] = {3, 6, -3, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}


//! Checks data driven assertion when input and expected results are initializers of int
//!
void UT_DataDriven_Successes::test_Initializer_Int_Int ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 3, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when input and expected results are vectors int
//!
void UT_DataDriven_Successes::test_Vector_Int_Int ()
{
  // ---------------- Prepare test
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  std::vector<int> inputs          {0, 5,  -1, -1000, -1};
  std::vector<int> expectedResults {0, 15, -3, -3000, -3};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven test when expecting some exception
//!
void UT_DataDriven_Successes::test_Expected_Exception ()
{
  // ---------------- TDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto doThrow    = [](int trigger) { if (trigger > -1) throw std::out_of_range("trigger > -1"); };
    auto inputLength = input.length();

    TS_ASSERT_THROWS (doThrow(expectedLength), std::exception);
    TS_ASSERT_EQUALS (inputLength, expectedLength);
  };

  auto inputs          = {"", "Hello World", "Hi", "How are you?"};
  auto expectedResults = {0,  11,            2,    12};

  // ---------------- TDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}


//! Checks data driven test when in assertion requiring nothing is thrown bu there is an exception is thrown
//!
void UT_DataDriven_Successes::test_Throw_Nothing ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input, int expectedLength)
  {
    auto doNotThrow  = [](int trigger) { if (trigger > 100) throw "Above trigger!!!"; };
    auto inputLength = input.length();

    TSM_ASSERT_THROWS_NOTHING ("Bad",  doNotThrow(expectedLength));
    TSM_ASSERT_EQUALS         ("Luck", inputLength, expectedLength);
  };

  auto inputs          = {"Hello World", "Hi", "This one is so long that it will trigger an exception of some sort !!!!"};
  auto expectedResults = {11,            2,    71};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs, expectedResults);
}

//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_RELATION in parameterized test
//!
//! @note setUp and tearDown are called for each tests on the same UT_DataDriven_Successes instance
//!
void UT_DataDriven_Successes::test_Setup_Teardown ()
{
  // ---------------- DDT Setup
  //
  m_setupCount    = 0;  // Clear those value to avoid being dependant of previous tests
  m_teardownCount = 0;

  auto testFunction = [this](auto expectedSetupCount, auto expectedTeardownCount)
  {
    TS_ASSERT_EQUALS (m_setupCount,    expectedSetupCount);
    TS_ASSERT_EQUALS (m_teardownCount, expectedTeardownCount);
  };

  auto expectedSetups    = {1, 2, 3, 4, 5};
  auto expectedTeardowns = {1, 2, 3, 4, 5};   // The 1st tearDown call is done by TS_DATA_DRIVEN_TEST to balance the setup of this test method

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, expectedSetups, expectedTeardowns);
}


//! Checks data driven assertion when inputs are int and no expected value is necessary
//!
void UT_DataDriven_Successes::test_Int ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input)
  {
    auto isOdd = [](int value) { return (value % 2) != 0; };
    TS_ASSERT_TRUE (isOdd(input));
  };

  auto inputs = {9, 1, 3, -1001};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs);
}

//! Checks data driven assertion when inputs are std::string and no expected value is necessary
//!
void UT_DataDriven_Successes::test_StdString ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input)
  {
    TS_ASSERT_LESS_THAN (input.length(), 20);
  };

  auto inputs = {"", "Hi" , "Hello World", "How are you?"};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testLength, inputs);
}



//! Checks data driven test when expecting some exception and no expected value is necessary
//!
void UT_DataDriven_Successes::test_SingleData_Expected_Exception ()
{
  // ---------------- TDT Setup
  //
  auto testExpectedException = [](const std::string& input)
  {
    auto mayThrow    = [](int trigger) { if (trigger < 100) throw std::out_of_range("trigger < 100"); };
    auto inputLength =  input.length();

    TS_ASSERT_THROWS (mayThrow(inputLength), std::exception);
  };

  auto inputs = {"?", "Hi", "How are you?"};

  // ---------------- TDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs);
}

//! Checks data driven test when there is an unexpected exception from assertion specifying nothing should be thrown and no expected value is necessary
//!
void UT_DataDriven_Successes::test_SingleData_Throw_Nothing ()
{
  // ---------------- DDT Setup
  //
  auto testExpectedException = [](const std::string& input)
  {
    auto mayThrow    = [](int trigger) { if (trigger > 100) throw "Above trigger!!!"; };
    auto inputLength =  input.length();

    TS_ASSERT_THROWS_NOTHING (mayThrow(inputLength));
    TS_ASSERT_GREATER_THAN   (inputLength, 1U);
  };

  auto inputs = {"Hello World", "Hi"};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testExpectedException, inputs);
}



//! Test TS_DATA_DRIVEN_TEST when using TS_ASSERT_RELATION in parameterized test
//!
//! @note setUp and tearDown are called for each tests on the same UT_DataDriven_Successes instance
//!
void UT_DataDriven_Successes::test_SingleData_Setup_Teardown ()
{
  // ---------------- DDT Setup
  //
  m_setupCount    = 0;  // Clear those value to avoid being dependant of previous tests
  m_teardownCount = 0;

  auto testFunction = [this](auto inputSample)
  {
    auto expectedSetupCount    = std::get<0>(inputSample);
    auto expectedTeardownCount = std::get<1>(inputSample);

    TS_ASSERT_EQUALS (m_setupCount,    expectedSetupCount);
    TS_ASSERT_EQUALS (m_teardownCount, expectedTeardownCount);
  };

  auto expectedSetupAndTeardown = {make_tuple(1, 1), make_tuple(2, 2), make_tuple(3, 3), make_tuple(4, 4)};  // The 1st tearDown call is done by TS_DATA_DRIVEN_TEST to balance the setup of this test method

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, expectedSetupAndTeardown);
}


//! Checks data driven assertion with mixed "input" (string) and "expected" (int)
//!
void UT_DataDriven_Successes::test_Mixed_StdString_Int ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input, int expectedLength)
  {
    TS_ASSERT_EQUALS (input.length(), expectedLength);
  };

  std::vector<std::tuple<std::string, int>> data;

  data.emplace_back(make_tuple(std::string(""),             0));  // 0
  data.emplace_back(make_tuple(std::string("Hi"),           2));  // 1
  data.emplace_back(make_tuple(std::string("Hello World"),  11)); // 2
  data.emplace_back(make_tuple(std::string("How are you?"), 12)); // 3

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST_MIX(testLength, data);
}


//! Checks data driven assertion with mixed "input" (string) and "expected" (int)
//!
void UT_DataDriven_Successes::test_Mixed_StdString_Compound ()
{
  // ---------------- Setup
  //
  auto testLength = [](const std::string& input, const tuple<int, bool>& expected)
  {
    auto inputLength = input.length();
    auto isOdd       = (inputLength % 2) == 1;

    TS_ASSERT_EQUALS (inputLength, std::get<int>(expected));
    TS_ASSERT_EQUALS (isOdd,       std::get<bool>(expected));
  };

  auto data =
  {
    make_pair(std::string(""),             make_tuple(0,  false)),
    make_pair(std::string("Hi"),           make_tuple(2,  false)),
    make_pair(std::string("Hello World"),  make_tuple(11, true)),
    make_pair(std::string("How are you?"), make_tuple(12, false)),
  };

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST_MIX(testLength, data);
}




//! Checks data driven assertion when there is usage of a test macro that assert nothing before ddt
//!
void UT_DataDriven_Successes::test_WithNoAssert_Before ()
{
  TS_WARN ("Warning message for UT_DataDriven_Successes");

  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 3, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);

}

//! Checks data driven assertion when there is usage of a test macro that assert nothing afterddt
//!
void UT_DataDriven_Successes::test_WithNoAssert_After ()
{
  // ---------------- Setup
  //
  auto testFunction = [](int input, int expected)
  {
    auto computedValue = 3 * input;
    TS_ASSERT_EQUALS (computedValue, expected);
  };

  auto inputs          = {0, 1, 2, -1000};
  auto expectedResults = {0, 3, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);

  TS_WARN ("Warning message for UT_DataDriven_Successes");
}


//! Checks data driven assertion when first test does not assert on anything (e.g. due to conditional assertion )
//!
void UT_DataDriven_Successes::test_MissingAssertion_First ()
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
  auto expectedResults = {0, 3, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when a middle test does not assert on anything (e.g. due to conditional assertion )
//!
void UT_DataDriven_Successes::test_MissingAssertion_Middle ()
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
  auto expectedResults = {0, 3, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}

//! Checks data driven assertion when last test does not assert on anything (e.g. due to conditional assertion )
//!
void UT_DataDriven_Successes::test_MissingAssertion_Last ()
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
  auto expectedResults = {0, 3, 6, -3000};

  // ---------------- Exercise
  //
  TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
}


//! @todo [JFC]-[September/16/2015]: Remove "No_test_yet_for_Guard" method when all tests are implemented
//!
void UT_DataDriven_Successes::test_No_test_yet_for_Guard ()
{
  TS_WARN ("No test yet for: all kind of assertions with all successes");
}




//===========================================================================
// End of UT_DataDriven_Successes.cpp
//===========================================================================
