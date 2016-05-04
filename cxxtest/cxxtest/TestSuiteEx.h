//===========================================================================
//                           TestSuiteEx.h
//===========================================================================
// Copyright (C) 2015. All rights reserved.
//
// Project : Cxxtest
//
//! @file TestSuiteEx.h
//!
//! Declares advanced assertions that needs at least C++ 11
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/17/2015
//===========================================================================


#ifndef TESTSUITEEX_H__CA77FCBF_9F3E_47D6_C7B8_53C26335F341__INCLUDED_
  #define TESTSUITEEX_H__CA77FCBF_9F3E_47D6_C7B8_53C26335F341__INCLUDED_

#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <functional>
#include <tuple>
#include <vector>
#include <cxxtest/TestSuite.h>
#include <cxxtest/TestStateGuard.h>

namespace CxxTest
{
  //! Returns traits for some types
  //!
  //!   * size: The number of elements in STL containers
  //!
  template<typename TContainer>
  struct Type_Trait
  {
    static size_t size(const TContainer& container) { return container.size(); }
  };

  //! Returns traits for some arrays
  //!
  //!   * size: The number of elements an array
  //!
  template<typename T, size_t N>
  struct Type_Trait<T[N]>
  {
    static size_t size(const T[N]) { return N; }
  };

  //! Defines a variant of ValueTrait that avoid displaying pointer.
  //!
  //! @note This instance of template is not intended to be instantiated
  //!
  template<typename T>
  class ValueTrait_NoPointer final
  {
    //+ Todo (JFC) (January/12/2016) : Consider customizing ValueTrait constructors with additional 'tag' parameter to switch to alternate behavior
    public:
    ~ValueTrait_NoPointer() {}
    ValueTrait_NoPointer() = delete;
  };

  //! Defines a variant of ValueTrait_NoPointer for c-style strings
  //!
  template <>
  class ValueTrait_NoPointer<char*> final
  {
    ValueTraits<char*> trait;

    public:
    ~ValueTrait_NoPointer() {}
    ValueTrait_NoPointer() = delete;
    ValueTrait_NoPointer(const char* pText) : trait(pText) { }
    ValueTrait_NoPointer(char*       pText) : trait(pText) { }

    const char *asString(void) const { return trait.asString(); }
  };

  //! Defines a variant of ValueTrait_NoPointer for general pointers (excepting c-style strings)
  //!
  template<typename T>
  class ValueTrait_NoPointer<T*> final
  {
    enum { BUFFER_SIZE = 20 };
    char m_valueAsString[BUFFER_SIZE];

    public:
    ~ValueTrait_NoPointer() {}
    ValueTrait_NoPointer() = delete;
    ValueTrait_NoPointer(const T* pointer)
    {
      if (!pointer)
      {
        copyStringN(m_valueAsString, "a null pointer", BUFFER_SIZE);
      }
      else
      {
        copyStringN(m_valueAsString, "a not null pointer", BUFFER_SIZE);
      }
    }
    const char *asString(void) const { return m_valueAsString; }
  };

  // Creates an instance of ValueTrait for not pointer object
  //
  template <class T>
  inline decltype(auto) traits_NoPointer(const T& t)
  {
    return traits(t);
  }

  // Overloads "traits_NoPointer" function template for plain strings
  //
  inline decltype(auto) traits_NoPointer(const char* pText)
  {
      return ValueTrait_NoPointer<char*>(pText);
  }

  // Overloads "traits_NoPointer" function template for plain strings
  //
  inline decltype(auto) traits_NoPointer(char* pText)
  {
      return ValueTrait_NoPointer<char*>(pText);
  }

  // Creates an instance of ValueTrait for pointer object (excepting c-style string)
  //
  template <class T>
  inline decltype(auto) traits_NoPointer(const T* pValue)
  {
    return ValueTrait_NoPointer<T*>(pValue);
  }

  template <class T>
  inline decltype(auto) traits_NoPointer(T* pValue)
  {
    return ValueTrait_NoPointer<T*>(pValue);
  }



  //! Helper class to remove reference_wrapper from some data
  //!
  //! @note The default behavior is to just return a reference to data passed to constructor
  template<typename T>
  class ValueTrait_NoReferenceWrapper final
  {
    public:
    ~ValueTrait_NoReferenceWrapper() = default;
    ValueTrait_NoReferenceWrapper()  = delete;
    ValueTrait_NoReferenceWrapper(T& data)
      : m_data (data)
    {
    }

    T& get() const { return m_data; }

    private:
    T& m_data;
  };

  //! Specialized ValueTrait_NoReferenceWrapper helper class for reference_wrapper
  //!
  template<typename T>
  class ValueTrait_NoReferenceWrapper<std::reference_wrapper<T>> final
  {
    public:
    ~ValueTrait_NoReferenceWrapper() {}
    ValueTrait_NoReferenceWrapper() = delete;
    ValueTrait_NoReferenceWrapper(std::reference_wrapper<T>& data)
      : m_data (data.get())
    {
    }

    T& get() const { return m_data; }

    private:
    T& m_data;
  };

  // Constructs a ValueTrait_NoReferenceWrapper for any data
  //
  template <class T>
  inline ValueTrait_NoReferenceWrapper<T> traits_NoReferenceWrapper(T& data)
  {
      return ValueTrait_NoReferenceWrapper<T>(data);
  }

  // Overloads "ValueTrait_NoReferenceWrapper" function template for plain strings
  //
  template <class T>
  inline ValueTrait_NoReferenceWrapper<std::reference_wrapper<T>> traits_NoReferenceWrapper(std::reference_wrapper<T>& data)
  {
      return ValueTrait_NoReferenceWrapper<std::reference_wrapper<T>>(data);
  }



  #define _TS_TRY_DDT     \
  auto testAbort = false; \
  try
  #define _TS_CATCH_DDT    \
  catch(const CxxTest::AbortTest& exc) \
  { \
    testAbort = true; \
  } \
  catch(bool exceptionValue) \
  { \
    tracker.unexpectExceptionFromTest("bool", TS_AS_STRING(exceptionValue), file, line); \
    testAbort = true; \
  } \
  catch(int exceptionValue) \
  { \
    tracker.unexpectExceptionFromTest("int", TS_AS_STRING(exceptionValue), file, line); \
    testAbort = true; \
  } \
  catch(std::exception& exc) \
  { \
    tracker.unexpectExceptionFromTest("std::exception", exc.what(), file, line); \
    testAbort = true; \
  } \
  catch (...) \
  { \
    tracker.unexpectExceptionFromTest("", "", file, line); \
    testAbort = true; \
  } \
  if (testAbort && abortDataDrivenTestsOnFailure()) \
  { \
    throw AbortTest(); \
  }


  //! Executes a parameterized test function with each given input and expected parameters
  //! as if it was defined as different unique tests
  //!
  //! @note   The log will not be exactly as if were different tests because cxxtest
  //!         has only a static test discovery.
  //! @note   Supports globlal fixture nor test setup/teardown
  //!
  //! @note   If any ValueTrait<input_type> (or ValueTrait<output_type>) is defined somewhere,
  //!         THEN, it MUST BE DEFINED IN ALL TEST FILES that use input_type (or output_type),
  //!         OTHERWISE there may be wrong assessment of ValueTrait<...> size leading to data
  //!         corruption and probably some segmentation fault!!!
  //!
  //! @see TS_DATA_DRIVEN_TEST TS_DATA_DRIVEN_TEST_MIX
  //!
  template<typename TParametizedTest, typename TInputs, typename TExpected>
  void doDataDrivenTests (const char* file, int line, TParametizedTest testFunction, TInputs& inputs, TExpected& expectedResults)
  {
    // ---------------- First check that there are as many expected data as input data
    //
    auto inputsSize          = Type_Trait<TInputs>::size(inputs);
    auto expectedResultsSize = Type_Trait<TExpected>::size(expectedResults);

    auto previousAbortOnFailure = CxxTest::setAbortTestOnFail(true);
    if (inputsSize != expectedResultsSize)   // Avoid showing a message in verbose mode (because it is not a user assertion)
    {
      _TSM_ASSERT_EQUALS (file, line, "Parameterized tests ARE NOT EXECUTED when inputs and expected results sets have not the same size!!!", inputsSize, expectedResultsSize);
    }
    CxxTest::setAbortTestOnFail(previousAbortOnFailure);

    auto& tracker = TestTracker::tracker();
    tracker.countDataDrivenTests(inputsSize);
    auto& testDescription = tracker.test();

    auto nextInput    = std::begin(inputs);
    auto nextExpected = std::begin(expectedResults);

    testDescription.tearDown();   // This tearDown is to balance the setup already done for the data driven test "master" before doing a setup for the 1st parameterized test
    for (size_t testId = 0; testId < inputsSize; ++testId)
    {
      _TS_TRY_DDT
      {
        TestStateGuard stateGuard;

        auto& expected = *nextExpected++;     // Pointers must be incremented there because of potential exception due to test failure
        auto& input    = *nextInput++;

        auto inputNoRefWT = traits_NoReferenceWrapper(input);
        auto inputNoRefW  = inputNoRefWT.get();                 // Remove potential reference_wrapper (added by split when using TS_DATA_DRIVEN_TEST_MIX)

        // ---------------- Get printable value for input sample
        //
        auto previousState = setStringResultsOnNewLine(false);
        auto inputTraits = traits_NoPointer(inputNoRefW);
        auto inputAsText = inputTraits.asString();
        setStringResultsOnNewLine(previousState);

        testDescription.setUp();
        tracker.enterDataDrivenTest(testId, inputAsText);
        testFunction(input, expected);
        tracker.leaveDataDrivenTest();
        testDescription.tearDown();
      }
      _TS_CATCH_DDT
    }
    tracker.leaveDataDrivenTests();
    testDescription.setUp();    // This setUp is to balance the tearDown that will be made by the runner for the data driven test "master"
  }
  //
  //  End of fct_doParameterizedTests
  //---------------------------------------------------------------------------




  //! Executes a parameterized test function with each element of given data parameter as if it was defined as different unique tests
  //!
  //! @note   The log will not be exactly as if it were different tests because cxxtest
  //!         has only a static test discovery.
  //! @note   Can be used for testing function where effect is constant relative to input data or for testing generators outputs
  //! @note   Supports globlal fixture nor test setup/teardown
  //!
  //! @note   If any ValueTrait<input_type> (or ValueTrait<output_type>) is defined somewhere,
  //!         THEN, it MUST BE DEFINED IN ALL TEST FILES that use input_type (or output_type),
  //!         OTHERWISE there may be wrong assessment of ValueTrait<...> size leading to data
  //!         corruption and probably some segmentation fault!!!
  //!
  //!
  //! @see TS_DATA_DRIVEN_TEST
  //!
  template<typename TParametizedTest, typename TData>
  void doDataDrivenTests (const char* file, int line, TParametizedTest testFunction, TData& data)
  {
    auto inputsSize = Type_Trait<TData>::size(data);

    auto& tracker = TestTracker::tracker();
    tracker.countDataDrivenTests(inputsSize);
    auto& testDescription = tracker.test();

    testDescription.tearDown();   // This tearDown is to balance the setup already done for the data driven test "master" before doing a setup for the 1st parameterized test
    auto testId = 0;
    for (auto& sample : data)
    {
      _TS_TRY_DDT
      {
        TestStateGuard stateGuard;

        auto previousState = setStringResultsOnNewLine(false);
        auto inputNoRefW = traits_NoReferenceWrapper(sample).get();  // Remove potential reference_wrapper (for better result display)
        auto inputTraits = traits_NoPointer(inputNoRefW);
        auto inputAsText = inputTraits.asString();
        setStringResultsOnNewLine(previousState);

        testDescription.setUp();
        tracker.enterDataDrivenTest(testId, inputAsText);
        testFunction(sample);
        testDescription.tearDown();
      }
      _TS_CATCH_DDT

      tracker.leaveDataDrivenTest();
      ++testId;
    }
    testDescription.setUp();    // This setUp is to balance the tearDown that will be made by the runner for the data driven test "master"
  }
  //
  //  End of fct_doParameterizedTests
  //---------------------------------------------------------------------------


  //! Splits a collection of pairs into a pair of collections of references to the pair parts.
  //!
  //! @note In fact, it deals with C++ 11 tuples
  //! @note This is intended to be used in context of TS_DATA_DRIVEN_TEST
  //!
  //! @return A 2-tuple of vectors with:
  //!         - first vector contains first elements of 2-tuples of input collection
  //!         - second vector contains second elements of 2-tuples of input collection
  //!
  template<typename Container>
  auto split(const Container& collection)
  {
    using Item_Type   = typename Container::value_type;
    using First_Type  = typename std::tuple_element_t<0, Item_Type>;
    using Second_Type = typename std::tuple_element_t<1, Item_Type>;

    using First_Ref  = std::reference_wrapper<const std::remove_reference_t<First_Type>>;
    using Second_Ref = std::reference_wrapper<const std::remove_reference_t<Second_Type>>;

    std::vector<First_Ref>  first;
    std::vector<Second_Ref> second;

    first.reserve(collection.size());
    second.reserve(collection.size());

    for (auto& item : collection)
    {
      auto f_item = std::cref(std::get<0>(item));
      auto s_item = std::cref(std::get<1>(item));
      first.push_back  (f_item);
      second.push_back (s_item);
    }
    return std::make_tuple(first, second);
  }
  // End of split


  //! Assertion for data driven tests
  //!
  //! @note First parameter must be an unary or binary callable
  //!       When using an unary callable, then must follow one iterable "container" of type compatible with callable parameter
  //!       When using an binary callable, then must follow two iterable "containers" of types compatible with callable parameters (same order),
  //!       and MUST have the same number of "samples".
  //!
  //! @note Setup and teardown are executed before and after each tests run for each sample
  //! @note Setup and teardown are called twice for the test method using TS_DATA_DRIVEN_TEST assertion.
  //!       In fact TS_DATA_DRIVEN_TEST start with a teardown and ends with a setup (to compensate for the usual setup/teardown for tests methods)
  //!
  //! @see CxxTest::doDataDrivenTests
  //!
  //! Examples:
  //!
  //! - With only a set of "inputs" data:
  //!   @code  {.cpp}
  //!     auto testFunction = [](int input)
  //!     {
  //!       TS_ASSERT (::isOdd(input))
  //!     };
  //!
  //!     auto inputs = {0, 1, 2, -1000};
  //!
  //!     TS_DATA_DRIVEN_TEST(testFunction, inputs);
  //!
  //!   @endcode
  //!
  //! - With only a set of "expected" data:
  //!   @code  {.cpp}
  //!     auto testFunction = [](int expected)
  //!     {
  //!       auto generatedValue = ::generator();
  //!       TS_ASSERT_EQUALS (generatedValue, expected);
  //!     };
  //!
  //!     auto expectedSet = {1, 2, 3, 5, 8};
  //!
  //!     TS_DATA_DRIVEN_TEST(testFunction, expectedSet);
  //!
  //!   @endcode
  //!
  //! - With a set of "inputs" and a set of "expected" data:
  //!   @code  {.cpp}
  //!     auto testFunction = [](int input, int expected)
  //!     {
  //!       auto computedValue = 3 * input;
  //!       TS_ASSERT_EQUALS (computedValue, expected);
  //!     };
  //!
  //!     auto inputs          = {0, 1, 2, -1000};
  //!     auto expectedResults = {1, 3, 7, -3000};
  //!
  //!     TS_DATA_DRIVEN_TEST(testFunction, inputs, expectedResults);
  //!
  //!   @endcode
  //!
  //! - With a collection of mixed "inputs" and "expected" data (this is easier to read input and expected on the same source line):
  //!   @code  {.cpp}
  //!     auto testFunction = [](int input, int expected)
  //!     {
  //!       auto computedValue = 3 * input;
  //!       TS_ASSERT_EQUALS (computedValue, expected);
  //!     };
  //!
  //!     auto data =
  //!     {
  //!       make_tuple(0, 1),
  //!       make_tuple(1, 3),
  //!       make_tuple(2, 7),
  //!       make_tuple(-1000, -3000),
  //!     };
  //!     auto  splitData = split(data);
  //!     TS_DATA_DRIVEN_TEST(testFunction, std::get<0>(splitData), std::get<1>(splitData));
  //!
  //!   @endcode
  //!
  #define TS_DATA_DRIVEN_TEST(paramerizedTestFunction,...)  CxxTest::countAssert(); CxxTest::doDataDrivenTests(__FILE__, __LINE__, (paramerizedTestFunction), __VA_ARGS__)

  #define NAME_CONCAT_DETAIL(a, b) a##b

  #define NAME_CONCAT(a, b) NAME_CONCAT_DETAIL(a,b)

  #define UNIQUE_NAME(name) NAME_CONCAT(name, __LINE__)

  //! Assertion for data driven tests (mixing input and expected data in same "data set")
  //!
  //! @param [in]      paramerizedTestFunction  First parameter must be a binary callable
  //! @param [in]      dataSet                  Iterable "containers" of types compatible with callable parameters (same order),
  //!
  //! @note Setup and teardown are executed before and after each tests run for each sample
  //! @note Setup and teardown are called twice for the test method using TS_DATA_DRIVEN_TEST_MIX assertion.
  //!       In fact TS_DATA_DRIVEN_TEST_MIX start with a teardown and ends with a setup (to compensate for the usual setup/teardown for tests methods)
  //!
  //! @see CxxTest::doDataDrivenTests
  //!
  //! Examples:
  //!
  //! - With a collection of mixed "inputs" and "expected" data (this is easier to read input and expected data on the same source line):
  //!   @code  {.cpp}
  //!     auto testFunction = [](int input, int expected)
  //!     {
  //!       auto computedValue = 3 * input;
  //!       TS_ASSERT_EQUALS (computedValue, expected);
  //!     };
  //!
  //!     auto data =
  //!     {
  //!       make_tuple(0,      1),
  //!       make_tuple(1,      3),
  //!       make_tuple(2,      7),
  //!       make_tuple(-1000, -3000),
  //!     };
  //!
  //!     TS_DATA_DRIVEN_TEST_MIX(testFunction, data);
  //!
  //!   @endcode
  //!
  //!   @note TS_DATA_DRIVEN_TEST_MIX must be only used once per scope (or a name conflict will be reported by the compiler)
//+  auto UNIQUE_NAME(splitDataVar) = CxxTest::split(dataSet); CxxTest::doDataDrivenTests(__FILE__, __LINE__, (paramerizedTestFunction), std::get<0>(UNIQUE_NAME(splitDataVar)), std::get<1>(UNIQUE_NAME(splitDataVar)))
  #define TS_DATA_DRIVEN_TEST_MIX(paramerizedTestFunction, dataSet)    \
  CxxTest::countAssert();                                              \
  auto _cxxtest_splitDataVar_ = CxxTest::split(dataSet); \
  CxxTest::doDataDrivenTests(__FILE__, __LINE__, (paramerizedTestFunction), std::get<0>(_cxxtest_splitDataVar_), std::get<1>(_cxxtest_splitDataVar_))

} // End of namespace CxxTest


#endif  // not defined TESTSUITEEX_H__CA77FCBF_9F3E_47D6_C7B8_53C26335F341__INCLUDED_
//===========================================================================
// End of TestSuiteEx.h
//===========================================================================


