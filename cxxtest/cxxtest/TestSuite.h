/*
-------------------------------------------------------------------------
 CxxTest: A lightweight C++ unit testing library.
 Copyright (c) 2008 Sandia Corporation.
 This software is distributed under the LGPL License v3
 For more information, see the COPYING file in the top CxxTest directory.
 Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------
*/

#ifndef __cxxtest__TestSuite_h__
#define __cxxtest__TestSuite_h__

//
// class TestSuite is the base class for all test suites.
// To define a test suite, derive from this class and add
// member functions called void test*();
//

#include <cxxtest/Flags.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/Descriptions.h>
#include <cxxtest/ValueTraits.h>
#include <cxxtest/StdValueTraits.h>
#include <sstream>
#include <cstring>

#if defined(_CXXTEST_HAVE_STD)
#   include <stdexcept>
#endif // _CXXTEST_HAVE_STD

namespace CxxTest
{
class TestSuite
{
public:
    virtual ~TestSuite();
    virtual void setUp();
    virtual void tearDown();
};

class AbortTest {};
class SkipTest {};
void doAbortTest();
#   define TS_ABORT() CxxTest::doAbortTest()

#define TS_NAME  CxxTest::tracker().test().testName() //!< Gets test method name (no class nor namespace scope)

class StringMerger
{
  enum { BUFFER_SIZE = 500 };

  char* m_pNextChar;
  char  m_mergedStrings[BUFFER_SIZE];

  public:
  StringMerger(const char* str1 = NULL, const char* str2 = NULL);

  //! Appends a string to current one
  //!
  void append(const char* str);

  const char* get() const
  {
    return m_mergedStrings;
  }
};

bool abortTestOnFail();
bool setAbortTestOnFail(bool value = CXXTEST_DEFAULT_ABORT);
bool abortDataDrivenTestsOnFailure();
bool setAbortDataDrivenTestsOnFailure(bool value = CXXTEST_DEFAULT_DDT_ABORT);


unsigned maxDumpSize();
void setMaxDumpSize(unsigned value = CXXTEST_MAX_DUMP_SIZE);

void doTrace            (const char* file, int line, const char* message);
void doWarn             (const char* file, int line, const char* message);
void doSkipTest         (const char* file, int line, const char* message);
void doFailTest         (const char* file, int line, const char* message);
void doAssertTrue       (const char* file, int line, const char* expression, const char* message, bool  expressionValue);
void doAssertFalse      (const char* file, int line, const char* expression, const char* message, bool  expressionValue);
void doAssertNullptr    (const char* file, int line, const char* expression, const char* message, bool  notNull);
void doAssertNotNullptr (const char* file, int line, const char* expression, const char* message, bool  notNull);
void countAssert        ();

//! For assertion outside of user test method, allow to disable tally of assertion to keep the
//! functionality of lack of assertion detection (in user test method)
//!
//! @note Just instantiate an instance of this class to disable tally of assertion, it will be
//!       automatically re-enabled at instance destruction
//!       @code {.cpp}
//!       {
//!         auto disabler = CxxTest::CountAssertDisabler();
//!         ...
//!       }
//!       @endcode
//! @note It is not thread safe !!!
//!
class CountAssertDisabler
{
  public:
  static bool disableAssertionCount;
   CountAssertDisabler() { disableAssertionCount = true; }
  ~CountAssertDisabler() { disableAssertionCount = false; }
};

template<class X, class Y>
inline bool areEqual(const X& x, const Y& y)
{
  return (x == y);
}

template<size_t N>
inline bool areEqual(const char x[N], const char y[N])
{
  return (CXXTEST_STD(strcmp(x, y)) == 0);
}

inline bool areEqual(const char* x, const char* y)
{
  if ((x != 0) && (y != 0))
  {
      return (CXXTEST_STD(strcmp(x, y)) == 0);
  }
  else
  {
      return (x == y);
  }
}

inline bool areEqual(char*       x, char*       y) { return areEqual(static_cast<const char*>(x), static_cast<const char*>(y)); }
inline bool areEqual(char*       x, const char* y) { return areEqual(static_cast<const char*>(x), y);                           }
inline bool areEqual(const char* x, char*       y) { return areEqual(x,                           static_cast<const char*>(y)); }



template<class X, class Y>
void doAssertEquals(const char *file, int line,
                    const char *xExpr, const X& x,
                    const char *yExpr, const Y& y,
                    const char *message)
{
    if (!CxxTest::areEqual(x, y))
    {
        tracker().failedAssertEquals(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertEquals(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
    }
}


//! Compares two pointers for equality
//!
//! @note This is, explicit, special comparison in order to only report their values as nullptr or not nullptr
//! @note It is compatible with smart pointer (there is no copy of them)
template<class X, class Y>
void doAssertEqualsPointers(const char *file,  int   line,
                            const char *xExpr, const X& x,
                            const char *yExpr, const Y& y,
                            const char *message)
{
    const char* asString_x = x ? "not nullptr" : "nullptr";
    const char* asString_y = y ? "not nullptr" : "nullptr";

    if (x != y)
    {
        tracker().failedAssertEquals(message, file, line, xExpr, yExpr, asString_x, asString_y);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertEquals(message, file, line, xExpr, yExpr, asString_x, asString_y);
    }
}


bool sameData(const void *x, const void *y, unsigned size);

void doAssertSameData(const char *file, int line,
                      const char *xExpr, const void *x,
                      const char *yExpr, const void *y,
                      const char *sizeExpr, unsigned size,
                      const char *message);

//#if defined(_CXXTEST_HAVE_STD)
bool sameFiles(const char* file1, const char* file2, std::ostringstream& explanation);
//#endif

template<class X, class Y>
void doAssertDiffers(const char *file, int line,
                     const char *xExpr, const X& x,
                     const char *yExpr, const Y& y,
                     const char *message)
{
    if (CxxTest::areEqual(x, y))
    {
        tracker().failedAssertDiffers(message, file, line, xExpr, yExpr, TS_AS_STRING(x));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertDiffers(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
    }
}

//! Compares two pointers for inequality
//!
//! @note This is, explicit, special comparison in order to only report their values as nullptr or not nullptr
//! @note It is compatible with smart pointer (there is no copy of them)
template<class X, class Y>
void doAssertDiffersPointers(const char *file,  int line,
                             const char *xExpr, X   x,
                             const char *yExpr, Y   y,
                             const char *message)
{
    const char* asString_x = x ? "not nullptr" : "nullptr";
    const char* asString_y = y ? "not nullptr" : "nullptr";

    if (x == y)
    {
        tracker().failedAssertDiffers(message, file, line, xExpr, yExpr, asString_x);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertDiffers(message, file, line, xExpr, yExpr, asString_x, asString_y);
    }
}

template<class X, class Y>
struct lessThan
{
    static bool test(const X& x, const Y& y)
    {
        return (x < y);
    }
};

template<class X, class Y>
void doAssertLessThan(const char *file, int line,
                      const char *xExpr, X x,
                      const char *yExpr, Y y,
                      const char *message)
{
    if (!lessThan<X, Y>::test(x, y))
    {
        tracker().failedAssertLessThan(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertLessThan(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
    }
}

template<class X, class Y>
struct lessThanEquals
{
    static bool test(const X& x, const Y& y)
    {
        return (x <= y);
    }
};

template<class X, class Y>
void doAssertLessThanEquals(const char *file, int line,
                            const char *xExpr, X x,
                            const char *yExpr, Y y,
                            const char *message)
{
    if (!lessThanEquals<X, Y>::test(x, y))
    {
        tracker().failedAssertLessThanEquals(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertLessThanEquals(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
    }
}

template<class X, class Y>
struct greaterThan
{
    static bool test(const X& x, const Y& y)
    {
        return (x > y);
    }
};

template<class X, class Y>
void doAssertGreaterThan(const char *file,  int line,
                         const char *xExpr, const X& x,
                         const char *yExpr, const Y& y,
                         const char *message)
{
    if (!greaterThan<X, Y>::test(x, y))
    {
        tracker().failedAssertGreaterThan(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertGreaterThan(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
    }
}

template<class X, class Y>
struct greaterThanEquals
{
    static bool test(const X& x, const Y& y)
    {
        return (x >= y);
    }
};

template<class X, class Y>
void doAssertGreaterThanEquals(const char *file, int line,
                               const char *xExpr, const X& x,
                               const char *yExpr, const Y& y,
                               const char *message)
{
    if (!greaterThanEquals<X, Y>::test(x, y))
    {
        tracker().failedAssertGreaterThanEquals(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertGreaterThanEquals(message, file, line, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
    }
}



template<class X, class P>
void doAssertPredicate(const char *file, int line,
                       const char *pExpr, const P& predicate,
                       const char *xExpr, const X& x,
                       const char *message)
{
    if (!predicate(x))
    {
        tracker().failedAssertPredicate(message, file, line, pExpr, xExpr, TS_AS_STRING(x));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertPredicate(message, file, line, pExpr, xExpr, TS_AS_STRING(x));
    }
}

template<class X, class Y, class R>
void doAssertRelation(const char *file, int line,
                      const char *rExpr, const R& relation,
                      const char *xExpr, const X& x,
                      const char *yExpr, const Y& y,
                      const char *message)
{
    if (!relation(x, y))
    {
        tracker().failedAssertRelation(message, file, line, rExpr, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertRelation(message, file, line, rExpr, xExpr, yExpr, TS_AS_STRING(x), TS_AS_STRING(y));
    }
}

// An indirection template so the compiler can determine what type
// "X +/- D" should be
template<class X, class Y>
bool delta_le_helper(const X& x, const Y& y)
{
    return lessThanEquals<X, Y>::test(x, y);
}

template<class X, class Y, class D>
struct delta
{
    static bool test(const X& x, const Y& y, const D& d)
    {
        return delta_le_helper(x, y + d) && delta_le_helper(y, x + d);
        //(y >= x - d) && (y <= x + d));
    }
};

template<class X, class Y, class D>
void doAssertDelta(const char *file, int line,
                   const char *xExpr, const X& x,
                   const char *yExpr, const Y& y,
                   const char *dExpr, const D& d,
                   const char *message)
{
    if (!delta<X, Y, D>::test(x, y, d))
    {

        tracker().failedAssertDelta(message, file, line, xExpr, yExpr, dExpr, TS_AS_STRING(x), TS_AS_STRING(y), TS_AS_STRING(d));
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertDelta(message, file, line, xExpr, yExpr, dExpr, TS_AS_STRING(x), TS_AS_STRING(y), TS_AS_STRING(d));
    }
}

void doFailAssertThrows     (const char* file, int line, const char* expr, const char* type, bool otherThrown, const char* message, const char* exception = 0);
void doAssertThrows         (const char* file, int line, const char* expression, const char* message, const char* expectedExceptionType, const char* exceptionMessage, bool hasThrown, bool otherThrown);
void doAssertThrowsNothing  (const char* file, int line, const char* expression, const char* message, const char* exceptionMessage, bool hasThrown);
void doAssertThrowsAnything (const char* file, int line, const char* expression, const char* message, bool        hasThrown);

void doAssertSameFiles     (const char* file, int line, const char* file1,       const char* file2,       const char* message);
void doAssertFileExists    (const char* file, int line, const char* checkedFile, const char* message);
void doAssertFileNotExists (const char* file, int line, const char* checkedFile, const char* message);
void doAssertFileSize      (const char* file, int line, const char* checkedFile, int         expecteSize,     const char* message);
void doAssertFileContent   (const char* file, int line, const char* filePath,    const char* expectedContentExpr, const char* expectedContent, const char* message);

template<typename TExc>
void doUnexpectedException(const char *file, int line, const char* typeName, const TExc& exc)
{
  CxxTest::tracker().unexpectExceptionFromTest(typeName, TS_AS_STRING(exc), file, line);
  TS_ABORT();
}

inline void doUnexpectedException(const char *file, int line, const char* typeName, const std::exception& exc)
{
  CxxTest::tracker().unexpectExceptionFromTest(typeName, exc.what(), file, line);
  TS_ABORT();
}

inline void doUnexpectedException(const char *file, int line, const char* typeName, const char* exc)
{
  CxxTest::tracker().unexpectExceptionFromTest(typeName, exc, file, line);
  TS_ABORT();
}



#   ifdef _CXXTEST_HAVE_EH
#       define _TS_TRY try
#       define _TS_CATCH_TYPE(t, b) catch t b
#       define _TS_CATCH_ABORT(b)   _TS_CATCH_TYPE( (const CxxTest::AbortTest &), b )
#       define _TS_CATCH_SKIPPED(b) _TS_CATCH_TYPE( (const CxxTest::SkipTest &), b )
#       define _TS_LAST_CATCH(b)    _TS_CATCH_TYPE( (...), b )
#       define _TS_CATCH_UNEXPECTED_WITH_TYPE(t,f,l) catch (const t& e) { CxxTest::doUnexpectedException(f, l, #t, e); }
#       define _TS_LAST_UNEXPECTED_CATCH(f,l)        _TS_LAST_CATCH(    { CxxTest::tracker().unexpectExceptionFromTest(NULL, NULL, f,l); TS_ABORT(); } )
#       ifdef _CXXTEST_HAVE_STD
#           define _TS_CATCH_STD(e,b) _TS_CATCH_TYPE( (const std::exception& e), b )
#       else // !_CXXTEST_HAVE_STD
#           define _TS_CATCH_STD(e,b)
#       endif // _CXXTEST_HAVE_STD
#       define _TS_CATCH_UNEXPECTED(f,l) \
            _TS_CATCH_UNEXPECTED_WITH_TYPE(bool,          f,l) \
            _TS_CATCH_UNEXPECTED_WITH_TYPE(float,         f,l) \
            _TS_CATCH_UNEXPECTED_WITH_TYPE(int,           f,l) \
            _TS_CATCH_UNEXPECTED_WITH_TYPE(unsigned int,  f,l) \
            _TS_CATCH_UNEXPECTED_WITH_TYPE(char*,         f,l) \
            _TS_CATCH_UNEXPECTED_WITH_TYPE(std::exception,f,l) \
            _TS_LAST_UNEXPECTED_CATCH(f,l)
#       define __TS_CATCH(f,l)               \
               _TS_CATCH_ABORT( { throw; } ) \
               _TS_CATCH_UNEXPECTED(f,l)
#       define _TS_CATCH __TS_CATCH(__FILE__,__LINE__)
#   else // !_CXXTEST_HAVE_EH
#       define _TS_TRY
#       define _TS_CATCH_UNEXPECTED(f,l)
#       define __TS_CATCH(f,l)
#       define _TS_CATCH
#       define _TS_CATCH_TYPE(t, b)
#       define _TS_LAST_CATCH(b)
#       define _TS_CATCH_STD(e,b)
#       define _TS_CATCH_ABORT(b)
#       define _TS_CATCH_SKIPPED(b)
#   endif // _CXXTEST_HAVE_EH

// TS_TRACE
#   define _TS_TRACE(f,l,e) CxxTest::doTrace( (f), (l), TS_AS_STRING_NO_QUOTES(e) )
#   define TS_TRACE(e) _TS_TRACE( __FILE__, __LINE__, e )

// TS_WARN
#   define _TS_WARN(f,l,e) CxxTest::doWarn( (f), (l), TS_AS_STRING_NO_QUOTES(e) )
#   define TS_WARN(e) _TS_WARN( __FILE__, __LINE__, e )

// TS_SKIP
#   define _TS_SKIP(f,l,e) CxxTest::doSkipTest( (f), (l), TS_AS_STRING_NO_QUOTES(e) )
#   define TS_SKIP(e) _TS_SKIP( __FILE__, __LINE__, e )

// TS_FAIL
#   define _TS_FAIL(f,l,e) CxxTest::doFailTest( (f), (l), TS_AS_STRING_NO_QUOTES(e) )
#   define TS_FAIL(e) _TS_FAIL( __FILE__, __LINE__, e )

// TS_ASSERT
#   define ___ETS_ASSERT(f,l,e,m) { CxxTest::countAssert(); CxxTest::doAssertTrue( (f), (l), #e, (m), (e));  }
#   define ___TS_ASSERT(f,l,e,m)  { _TS_TRY { ___ETS_ASSERT(f,l,e,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT(f,l,e) ___ETS_ASSERT(f,l,e,0)
#   define _TS_ASSERT(f,l,e) ___TS_ASSERT(f,l,e,0)
#   define _TS_ASSERT_TRUE(f,l,e) _TS_ASSERT(f,l,e)

#   define ETS_ASSERT(e)   _ETS_ASSERT(__FILE__,__LINE__,e)
#   define TS_ASSERT(e)     _TS_ASSERT(__FILE__,__LINE__,e)
#   define TS_ASSERT_TRUE(e) TS_ASSERT(e)

#   define _ETSM_ASSERT(f,l,m,e) ___ETS_ASSERT(f,l,e,TS_AS_STRING_NO_QUOTES(m) )
#   define _TSM_ASSERT(f,l,m,e)  ___TS_ASSERT(f,l,e,TS_AS_STRING_NO_QUOTES(m) )
#   define _TSM_ASSERT_THROWS_TRUE(f,l,m,e) __TSM_ASSERT(f,l,m,e)

#   define ETSM_ASSERT(m,e)   _ETSM_ASSERT(__FILE__,__LINE__,m,e)
#   define TSM_ASSERT(m,e)     _TSM_ASSERT(__FILE__,__LINE__,m,e)
#   define TSM_ASSERT_TRUE(m,e) TSM_ASSERT(m,e)

// TS_ASSERT_FALSE
#   define ___ETS_ASSERT_FALSE(f,l,e,m) { CxxTest::countAssert(); CxxTest::doAssertFalse( (f), (l), #e, (m), (e));  }
#   define ___TS_ASSERT_FALSE(f,l,e,m)  { _TS_TRY { ___ETS_ASSERT_FALSE(f,l,e,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_FALSE(f,l,e) ___ETS_ASSERT_FALSE(f,l,e,0)
#   define _TS_ASSERT_FALSE(f,l,e)   ___TS_ASSERT_FALSE(f,l,e,0)

#   define ETS_ASSERT_FALSE(e) _ETS_ASSERT_FALSE(__FILE__,__LINE__,e)
#   define TS_ASSERT_FALSE(e)   _TS_ASSERT_FALSE(__FILE__,__LINE__,e)

#   define _ETSM_ASSERT_FALSE(f,l,m,e) ___ETS_ASSERT_FALSE(f,l,e,TS_AS_STRING_NO_QUOTES(m) )
#   define _TSM_ASSERT_FALSE(f,l,m,e)  ___TS_ASSERT_FALSE(f,l,e,TS_AS_STRING_NO_QUOTES(m) )

#   define ETSM_ASSERT_FALSE(m,e) _ETSM_ASSERT_FALSE(__FILE__,__LINE__,m,e)
#   define TSM_ASSERT_FALSE(m,e)   _TSM_ASSERT_FALSE(__FILE__,__LINE__,m,e)

//! This helper is used to support testing different kind of pointer like objects
//!
template<typename T>
bool isNotNullPtr(const T& ptr)  { return bool(ptr); }


// TS_ASSERT_NULLPTR
// Checks that a pointer is nullptr (it is compatible with all kinds of pointers convertible to bool when true means a not nullptr)
//
#   define ___ETS_ASSERT_NULLPTR(f,l,e,m) { CxxTest::countAssert(); CxxTest::doAssertNullptr( (f), (l), #e, (m), CxxTest::isNotNullPtr(e));  }
#   define ___TS_ASSERT_NULLPTR(f,l,e,m)  { _TS_TRY { ___ETS_ASSERT_NULLPTR(f,l,e,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_NULLPTR(f,l,e) ___ETS_ASSERT_NULLPTR(f,l,e,0)
#   define _TS_ASSERT_NULLPTR(f,l,e)   ___TS_ASSERT_NULLPTR(f,l,e,0)

#   define ETS_ASSERT_NULLPTR(e) _ETS_ASSERT_NULLPTR(__FILE__,__LINE__,e)
#   define TS_ASSERT_NULLPTR(e)   _TS_ASSERT_NULLPTR(__FILE__,__LINE__,e)

#   define _ETSM_ASSERT_NULLPTR(f,l,m,e) ___ETS_ASSERT_NULLPTR(f,l,e,TS_AS_STRING_NO_QUOTES(m) )
#   define _TSM_ASSERT_NULLPTR(f,l,m,e)  ___TS_ASSERT_NULLPTR(f,l,e,TS_AS_STRING_NO_QUOTES(m) )

#   define ETSM_ASSERT_NULLPTR(m,e) _ETSM_ASSERT_NULLPTR(__FILE__,__LINE__,m,e)
#   define TSM_ASSERT_NULLPTR(m,e)   _TSM_ASSERT_NULLPTR(__FILE__,__LINE__,m,e)

// TS_ASSERT_NOT_NULLPTR
// Checks that a pointer is not nullptr (it is compatible with all kinds of pointers convertible to bool when true means a not nullptr)
//
#   define ___ETS_ASSERT_NOT_NULLPTR(f,l,e,m) { CxxTest::countAssert(); CxxTest::doAssertNotNullptr( (f), (l), #e, (m), CxxTest::isNotNullPtr(e));  }
#   define ___TS_ASSERT_NOT_NULLPTR(f,l,e,m)  { _TS_TRY { ___ETS_ASSERT_NOT_NULLPTR(f,l,e,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_NOT_NULLPTR(f,l,e) ___ETS_ASSERT_NOT_NULLPTR(f,l,e,0)
#   define _TS_ASSERT_NOT_NULLPTR(f,l,e)   ___TS_ASSERT_NOT_NULLPTR(f,l,e,0)

#   define ETS_ASSERT_NOT_NULLPTR(e) _ETS_ASSERT_NOT_NULLPTR(__FILE__,__LINE__,e)
#   define TS_ASSERT_NOT_NULLPTR(e)   _TS_ASSERT_NOT_NULLPTR(__FILE__,__LINE__,e)

#   define _ETSM_ASSERT_NOT_NULLPTR(f,l,m,e) ___ETS_ASSERT_NOT_NULLPTR(f,l,e,TS_AS_STRING_NO_QUOTES(m) )
#   define _TSM_ASSERT_NOT_NULLPTR(f,l,m,e)  ___TS_ASSERT_NOT_NULLPTR(f,l,e,TS_AS_STRING_NO_QUOTES(m) )

#   define ETSM_ASSERT_NOT_NULLPTR(m,e) _ETSM_ASSERT_NOT_NULLPTR(__FILE__,__LINE__,m,e)
#   define TSM_ASSERT_NOT_NULLPTR(m,e)   _TSM_ASSERT_NOT_NULLPTR(__FILE__,__LINE__,m,e)



// TS_ASSERT_EQUALS

//! This is one branch of a compile switch for comparing stuff with line, col, message and expression.
//! They are "captured" at user code level (for utility comparison tool or data driven assertion)
//!
//! @note This is design in order to have single evaluation of both left and right expressions
//! @note Rhs expression value must be convertible of lhs expression value type
//!
template<typename T>
class EqualTrait
{
    const T& m_lhs;
    public:
    EqualTrait(const T& lhs) : m_lhs(lhs) {}  // Save the reference to lhs expression value
    ~EqualTrait(){}

    void checkEqual(const char file[], int line, const char* message, const char* lhsStr, const char* rhsStr, const T& rhs)
    {
        countAssert();
        doAssertEquals(file, line, lhsStr, m_lhs, rhsStr, rhs, message);
    }
};

//! Overloads EqualTrait for comparing pointers
//!
//! @note    Pointer should not be compares with TS_ASSERT_EQUALS because values change at each run
//!
template<typename T>
class EqualTrait<T*>
{
    const T* m_pLhs;
    public:
    EqualTrait(const T* pLhs) : m_pLhs(pLhs) {}
    ~EqualTrait(){}

    //! Creates a pseudo expression text 'lhs == rhs' as if TS_ASSERT_EQUALS(lhs, rhs) was used,
    //! but effectively do like TS_ASSERT(lhs == rhs)
    //! Value are never printed but this is exactly what all this stuff is all about (displaying pointer value is usually useless)
    //!
    void checkEqual(const char file[], int line, const char* message, const char* lhsStr, const char* rhsStr, const T* pRhs)
    {
        enum { BUFFER_SIZE = 100 };
        char  m_comparisonAsString[BUFFER_SIZE];
        char* pEnd  = &m_comparisonAsString[BUFFER_SIZE - 1];
        char* pFree = m_comparisonAsString;

        pFree = copyStringN(pFree, lhsStr, pEnd - pFree);
        pFree = copyStringN(pFree, " == ", pEnd - pFree);
        pFree = copyStringN(pFree, rhsStr, pEnd - pFree);

        countAssert();
        doAssertTrue(file, line, m_comparisonAsString, message, m_pLhs == pRhs);
    }
};

//! Selects proper version of EqualTrait
//!
template<typename T>
inline EqualTrait<T> MakeEqualTrait(const T& x) { return EqualTrait<T>(x); }

#define _TSMA_ASSERT_EQUALS(f,l,m,x,y) { _TS_TRY { CxxTest::MakeEqualTrait(x).checkEqual((f), (l), (m), #x, #y, (y)); } __TS_CATCH(f,l) }
#define _TSA_ASSERT_EQUALS(f,l,x,y)   _TSMA_ASSERT_EQUALS(f, l, NULL, x, y)
#define TSMA_ASSERT_EQUALS(m,x,y)     _TSMA_ASSERT_EQUALS(__FILE__, __LINE__, m,  x, y)
#define TSA_ASSERT_EQUALS(x,y)        _TSMA_ASSERT_EQUALS(__FILE__, __LINE__, NULL, x, y)




#   define ___ETS_ASSERT_EQUALS(f,l,x,y,m) CxxTest::countAssert(); CxxTest::doAssertEquals( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_EQUALS(f,l,x,y,m) { _TS_TRY { ___ETS_ASSERT_EQUALS(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_EQUALS(f,l,x,y) ___ETS_ASSERT_EQUALS(f,l,x,y,0)
#   define _TS_ASSERT_EQUALS(f,l,x,y) ___TS_ASSERT_EQUALS(f,l,x,y,0)

#   define ETS_ASSERT_EQUALS(x,y) _ETS_ASSERT_EQUALS(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_EQUALS(x,y) _TS_ASSERT_EQUALS(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_EQUALS(f,l,m,x,y) ___ETS_ASSERT_EQUALS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_EQUALS(f,l,m,x,y) ___TS_ASSERT_EQUALS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_EQUALS(m,x,y) _ETSM_ASSERT_EQUALS(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_EQUALS(m,x,y) _TSM_ASSERT_EQUALS(__FILE__,__LINE__,m,x,y)

// TS_ASSERT_EQUALS_PTR
//
// Checks that two pointers are equal reporting actual values as "nullptr" or "not nullptr"
//
#   define ___ETS_ASSERT_EQUALS_PTR(f,l,x,y,m) CxxTest::countAssert(); CxxTest::doAssertEqualsPointers( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_EQUALS_PTR(f,l,x,y,m) { _TS_TRY { ___ETS_ASSERT_EQUALS_PTR(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_EQUALS_PTR(f,l,x,y) ___ETS_ASSERT_EQUALS_PTR(f,l,x,y,0)
#   define _TS_ASSERT_EQUALS_PTR(f,l,x,y) ___TS_ASSERT_EQUALS_PTR(f,l,x,y,0)

#   define ETS_ASSERT_EQUALS_PTR(x,y) _ETS_ASSERT_EQUALS_PTR(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_EQUALS_PTR(x,y) _TS_ASSERT_EQUALS_PTR(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_EQUALS_PTR(f,l,m,x,y) ___ETS_ASSERT_EQUALS_PTR(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_EQUALS_PTR(f,l,m,x,y) ___TS_ASSERT_EQUALS_PTR(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_EQUALS_PTR(m,x,y) _ETSM_ASSERT_EQUALS_PTR(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_EQUALS_PTR(m,x,y) _TSM_ASSERT_EQUALS_PTR(__FILE__,__LINE__,m,x,y)


// Special floating point values support: REQUIRE C++11 or above and <cmath> header
#   define ETS_ASSERT_IS_NAN(x)  _ETS_ASSERT(__FILE__,__LINE__,CXXTEST_STD(isnan(x)))
#   define TS_ASSERT_IS_NAN(x)   _TS_ASSERT(__FILE__,__LINE__,CXXTEST_STD(isnan(x)))

#   define ETSM_ASSERT_IS_NAN(m,x) _ETSM_ASSERT(__FILE__,__LINE__,m,CXXTEST_STD(isnan(x)))
#   define TSM_ASSERT_IS_NAN(m,x) _TSM_ASSERT(__FILE__,__LINE__,m,CXXTEST_STD(isnan(x)))

#   define ETS_ASSERT_IS_INFINITE(x)  _ETS_ASSERT(__FILE__,__LINE__,CXXTEST_STD(isinf(x)))
#   define TS_ASSERT_IS_INFINITE(x)   _TS_ASSERT(__FILE__,__LINE__,CXXTEST_STD(isinf(x)))

#   define ETSM_ASSERT_IS_INFINITE(m,x) _ETSM_ASSERT(__FILE__,__LINE__,m,CXXTEST_STD(isinf(x)))
#   define TSM_ASSERT_IS_INFINITE(m,x) _TSM_ASSERT(__FILE__,__LINE__,m,CXXTEST_STD(isinf(x)))

// TS_ASSERT_SAME_DATA
#   define ___ETS_ASSERT_SAME_DATA(f,l,x,y,s,m) CxxTest::countAssert(); CxxTest::doAssertSameData( (f), (l), #x, (x), #y, (y), #s, (s), (m) )
#   define ___TS_ASSERT_SAME_DATA(f,l,x,y,s,m) { _TS_TRY { ___ETS_ASSERT_SAME_DATA(f,l,x,y,s,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_SAME_DATA(f,l,x,y,s) ___ETS_ASSERT_SAME_DATA(f,l,x,y,s,0)
#   define _TS_ASSERT_SAME_DATA(f,l,x,y,s) ___TS_ASSERT_SAME_DATA(f,l,x,y,s,0)

#   define ETS_ASSERT_SAME_DATA(x,y,s) _ETS_ASSERT_SAME_DATA(__FILE__,__LINE__,x,y,s)
#   define TS_ASSERT_SAME_DATA(x,y,s) _TS_ASSERT_SAME_DATA(__FILE__,__LINE__,x,y,s)

#   define _ETSM_ASSERT_SAME_DATA(f,l,m,x,y,s) ___ETS_ASSERT_SAME_DATA(f,l,x,y,s,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_SAME_DATA(f,l,m,x,y,s) ___TS_ASSERT_SAME_DATA(f,l,x,y,s,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_SAME_DATA(m,x,y,s) _ETSM_ASSERT_SAME_DATA(__FILE__,__LINE__,m,x,y,s)
#   define TSM_ASSERT_SAME_DATA(m,x,y,s) _TSM_ASSERT_SAME_DATA(__FILE__,__LINE__,m,x,y,s)

// TS_ASSERT_DIFFERS
#   define ___ETS_ASSERT_DIFFERS(f,l,x,y,m) CxxTest::countAssert(); CxxTest::doAssertDiffers( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_DIFFERS(f,l,x,y,m) { _TS_TRY { ___ETS_ASSERT_DIFFERS(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_DIFFERS(f,l,x,y) ___ETS_ASSERT_DIFFERS(f,l,x,y,0)
#   define _TS_ASSERT_DIFFERS(f,l,x,y) ___TS_ASSERT_DIFFERS(f,l,x,y,0)

#   define ETS_ASSERT_DIFFERS(x,y) _ETS_ASSERT_DIFFERS(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_DIFFERS(x,y) _TS_ASSERT_DIFFERS(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_DIFFERS(f,l,m,x,y) ___ETS_ASSERT_DIFFERS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_DIFFERS(f,l,m,x,y) ___TS_ASSERT_DIFFERS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_DIFFERS(m,x,y) _ETSM_ASSERT_DIFFERS(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_DIFFERS(m,x,y) _TSM_ASSERT_DIFFERS(__FILE__,__LINE__,m,x,y)


// TS_ASSERT_DIFFERS_PTR
//
// Checks that two pointers are not equal reporting actual values as "nullptr" or "not nullptr"
//
#   define ___ETS_ASSERT_DIFFERS_PTR(f,l,x,y,m) CxxTest::countAssert(); CxxTest::doAssertDiffersPointers( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_DIFFERS_PTR(f,l,x,y,m) { _TS_TRY { ___ETS_ASSERT_DIFFERS_PTR(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_DIFFERS_PTR(f,l,x,y) ___ETS_ASSERT_DIFFERS_PTR(f,l,x,y,0)
#   define _TS_ASSERT_DIFFERS_PTR(f,l,x,y) ___TS_ASSERT_DIFFERS_PTR(f,l,x,y,0)

#   define ETS_ASSERT_DIFFERS_PTR(x,y) _ETS_ASSERT_DIFFERS_PTR(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_DIFFERS_PTR(x,y) _TS_ASSERT_DIFFERS_PTR(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_DIFFERS_PTR(f,l,m,x,y) ___ETS_ASSERT_DIFFERS_PTR(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_DIFFERS_PTR(f,l,m,x,y) ___TS_ASSERT_DIFFERS_PTR(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_DIFFERS_PTR(m,x,y) _ETSM_ASSERT_DIFFERS_PTR(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_DIFFERS_PTR(m,x,y) _TSM_ASSERT_DIFFERS_PTR(__FILE__,__LINE__,m,x,y)


// TS_ASSERT_LESS_THAN
#   define ___ETS_ASSERT_LESS_THAN(f,l,x,y,m) CxxTest::countAssert(); CxxTest::doAssertLessThan( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_LESS_THAN(f,l,x,y,m) { _TS_TRY { ___ETS_ASSERT_LESS_THAN(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_LESS_THAN(f,l,x,y) ___ETS_ASSERT_LESS_THAN(f,l,x,y,0)
#   define _TS_ASSERT_LESS_THAN(f,l,x,y) ___TS_ASSERT_LESS_THAN(f,l,x,y,0)

#   define ETS_ASSERT_LESS_THAN(x,y) _ETS_ASSERT_LESS_THAN(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_LESS_THAN(x,y) _TS_ASSERT_LESS_THAN(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_LESS_THAN(f,l,m,x,y) ___ETS_ASSERT_LESS_THAN(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_LESS_THAN(f,l,m,x,y) ___TS_ASSERT_LESS_THAN(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_LESS_THAN(m,x,y) _ETSM_ASSERT_LESS_THAN(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_LESS_THAN(m,x,y) _TSM_ASSERT_LESS_THAN(__FILE__,__LINE__,m,x,y)

// TS_ASSERT_LESS_THAN_EQUALS
#   define ___ETS_ASSERT_LESS_THAN_EQUALS(f,l,x,y,m) \
        CxxTest::countAssert(); CxxTest::doAssertLessThanEquals( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_LESS_THAN_EQUALS(f,l,x,y,m) \
        { _TS_TRY { ___ETS_ASSERT_LESS_THAN_EQUALS(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_LESS_THAN_EQUALS(f,l,x,y) ___ETS_ASSERT_LESS_THAN_EQUALS(f,l,x,y,0)
#   define _TS_ASSERT_LESS_THAN_EQUALS(f,l,x,y) ___TS_ASSERT_LESS_THAN_EQUALS(f,l,x,y,0)

#   define ETS_ASSERT_LESS_THAN_EQUALS(x,y) _ETS_ASSERT_LESS_THAN_EQUALS(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_LESS_THAN_EQUALS(x,y) _TS_ASSERT_LESS_THAN_EQUALS(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_LESS_THAN_EQUALS(f,l,m,x,y) ___ETS_ASSERT_LESS_THAN_EQUALS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_LESS_THAN_EQUALS(f,l,m,x,y) ___TS_ASSERT_LESS_THAN_EQUALS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_LESS_THAN_EQUALS(m,x,y) _ETSM_ASSERT_LESS_THAN_EQUALS(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_LESS_THAN_EQUALS(m,x,y) _TSM_ASSERT_LESS_THAN_EQUALS(__FILE__,__LINE__,m,x,y)


// TS_ASSERT_GREATER_THAN
#   define ___ETS_ASSERT_GREATER_THAN(f,l,x,y,m) CxxTest::countAssert(); CxxTest::doAssertGreaterThan( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_GREATER_THAN(f,l,x,y,m) { _TS_TRY { ___ETS_ASSERT_GREATER_THAN(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_GREATER_THAN(f,l,x,y) ___ETS_ASSERT_GREATER_THAN(f,l,x,y,0)
#   define _TS_ASSERT_GREATER_THAN(f,l,x,y) ___TS_ASSERT_GREATER_THAN(f,l,x,y,0)

#   define ETS_ASSERT_GREATER_THAN(x,y) _ETS_ASSERT_GREATER_THAN(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_GREATER_THAN(x,y) _TS_ASSERT_GREATER_THAN(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_GREATER_THAN(f,l,m,x,y) ___ETS_ASSERT_GREATER_THAN(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_GREATER_THAN(f,l,m,x,y) ___TS_ASSERT_GREATER_THAN(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_GREATER_THAN(m,x,y) _ETSM_ASSERT_GREATER_THAN(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_GREATER_THAN(m,x,y) _TSM_ASSERT_GREATER_THAN(__FILE__,__LINE__,m,x,y)

// TS_ASSERT_GREATER_THAN_EQUALS
#   define ___ETS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y,m) \
        CxxTest::countAssert(); CxxTest::doAssertGreaterThanEquals( (f), (l), #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y,m) \
        { _TS_TRY { ___ETS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y) ___ETS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y,0)
#   define _TS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y) ___TS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y,0)

#   define ETS_ASSERT_GREATER_THAN_EQUALS(x,y) _ETS_ASSERT_GREATER_THAN_EQUALS(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_GREATER_THAN_EQUALS(x,y) _TS_ASSERT_GREATER_THAN_EQUALS(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_GREATER_THAN_EQUALS(f,l,m,x,y) ___ETS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_GREATER_THAN_EQUALS(f,l,m,x,y) ___TS_ASSERT_GREATER_THAN_EQUALS(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_GREATER_THAN_EQUALS(m,x,y) _ETSM_ASSERT_GREATER_THAN_EQUALS(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_GREATER_THAN_EQUALS(m,x,y) _TSM_ASSERT_GREATER_THAN_EQUALS(__FILE__,__LINE__,m,x,y)


// TS_ASSERT_PREDICATE
#   define ___ETS_ASSERT_PREDICATE(f,l,p,x,m) \
        CxxTest::countAssert(); CxxTest::doAssertPredicate( (f), (l), #p, p, #x, (x), (m) )
#   define ___TS_ASSERT_PREDICATE(f,l,p,x,m) \
        { _TS_TRY { ___ETS_ASSERT_PREDICATE(f,l,p,x,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_PREDICATE(f,l,p,x) ___ETS_ASSERT_PREDICATE(f,l,p,x,0)
#   define _TS_ASSERT_PREDICATE(f,l,p,x) ___TS_ASSERT_PREDICATE(f,l,p,x,0)

#   define ETS_ASSERT_PREDICATE(p,x) _ETS_ASSERT_PREDICATE(__FILE__,__LINE__,p,x)
#   define TS_ASSERT_PREDICATE(p,x) _TS_ASSERT_PREDICATE(__FILE__,__LINE__,p,x)

#   define _ETSM_ASSERT_PREDICATE(f,l,m,p,x) ___ETS_ASSERT_PREDICATE(f,l,p,x,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_PREDICATE(f,l,m,p,x) ___TS_ASSERT_PREDICATE(f,l,p,x,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_PREDICATE(m,p,x) _ETSM_ASSERT_PREDICATE(__FILE__,__LINE__,m,p,x)
#   define TSM_ASSERT_PREDICATE(m,p,x) _TSM_ASSERT_PREDICATE(__FILE__,__LINE__,m,p,x)

// TS_ASSERT_RELATION
#   define ___ETS_ASSERT_RELATION(f,l,r,x,y,m) \
        CxxTest::countAssert(); CxxTest::doAssertRelation( (f), (l), #r, r, #x, (x), #y, (y), (m) )
#   define ___TS_ASSERT_RELATION(f,l,r,x,y,m) \
        { _TS_TRY { ___ETS_ASSERT_RELATION(f,l,r,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_RELATION(f,l,r,x,y) ___ETS_ASSERT_RELATION(f,l,r,x,y,0)
#   define _TS_ASSERT_RELATION(f,l,r,x,y) ___TS_ASSERT_RELATION(f,l,r,x,y,0)

#   define ETS_ASSERT_RELATION(r,x,y) _ETS_ASSERT_RELATION(__FILE__,__LINE__,r,x,y)
#   define TS_ASSERT_RELATION(r,x,y) _TS_ASSERT_RELATION(__FILE__,__LINE__,r,x,y)

#   define _ETSM_ASSERT_RELATION(f,l,m,r,x,y) ___ETS_ASSERT_RELATION(f,l,r,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_RELATION(f,l,m,r,x,y) ___TS_ASSERT_RELATION(f,l,r,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_RELATION(m,r,x,y) _ETSM_ASSERT_RELATION(__FILE__,__LINE__,m,r,x,y)
#   define TSM_ASSERT_RELATION(m,r,x,y) _TSM_ASSERT_RELATION(__FILE__,__LINE__,m,r,x,y)

// TS_ASSERT_DELTA
#   define ___ETS_ASSERT_DELTA(f,l,x,y,d,m) CxxTest::countAssert(); CxxTest::doAssertDelta( (f), (l), #x, (x), #y, (y), #d, (d), (m) )
#   define ___TS_ASSERT_DELTA(f,l,x,y,d,m) { _TS_TRY { ___ETS_ASSERT_DELTA(f,l,x,y,d,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_DELTA(f,l,x,y,d) ___ETS_ASSERT_DELTA(f,l,x,y,d,0)
#   define _TS_ASSERT_DELTA(f,l,x,y,d) ___TS_ASSERT_DELTA(f,l,x,y,d,0)

#   define ETS_ASSERT_DELTA(x,y,d) _ETS_ASSERT_DELTA(__FILE__,__LINE__,x,y,d)
#   define TS_ASSERT_DELTA(x,y,d) _TS_ASSERT_DELTA(__FILE__,__LINE__,x,y,d)

#   define _ETSM_ASSERT_DELTA(f,l,m,x,y,d) ___ETS_ASSERT_DELTA(f,l,x,y,d,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_DELTA(f,l,m,x,y,d) ___TS_ASSERT_DELTA(f,l,x,y,d,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_DELTA(m,x,y,d) _ETSM_ASSERT_DELTA(__FILE__,__LINE__,m,x,y,d)
#   define TSM_ASSERT_DELTA(m,x,y,d) _TSM_ASSERT_DELTA(__FILE__,__LINE__,m,x,y,d)

// TS_ASSERT_SAME_FILES
#   define ___ETS_ASSERT_SAME_FILES(f,l,x,y,m) CxxTest::countAssert(); CxxTest::doAssertSameFiles( (f), (l), CxxTest::asCStyleString(x), CxxTest::asCStyleString(y), (m) )
#   define ___TS_ASSERT_SAME_FILES(f,l,x,y,m) { _TS_TRY { ___ETS_ASSERT_SAME_FILES(f,l,x,y,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_SAME_FILES(f,l,x,y) ___ETS_ASSERT_SAME_FILES(f,l,x,y,0)
#   define _TS_ASSERT_SAME_FILES(f,l,x,y) ___TS_ASSERT_SAME_FILES(f,l,x,y,0)

#   define ETS_ASSERT_SAME_FILES(x,y) _ETS_ASSERT_SAME_FILES(__FILE__,__LINE__,x,y)
#   define TS_ASSERT_SAME_FILES(x,y) _TS_ASSERT_SAME_FILES(__FILE__,__LINE__,x,y)

#   define _ETSM_ASSERT_SAME_FILES(f,l,m,x,y) ___ETS_ASSERT_SAME_FILES(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_SAME_FILES(f,l,m,x,y) ___TS_ASSERT_SAME_FILES(f,l,x,y,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_SAME_FILES(m,x,y) _ETSM_ASSERT_SAME_FILES(__FILE__,__LINE__,m,x,y)
#   define TSM_ASSERT_SAME_FILES(m,x,y) _TSM_ASSERT_SAME_FILES(__FILE__,__LINE__,m,x,y)

// TS_ASSERT_FILE_EXISTS
#   define ___ETS_ASSERT_FILE_EXISTS(f,l,x,m) CxxTest::countAssert(); CxxTest::doAssertFileExists( (f), (l), CxxTest::asCStyleString(x), (m) )
#   define ___TS_ASSERT_FILE_EXISTS(f,l,x,m) { _TS_TRY { ___ETS_ASSERT_FILE_EXISTS(f,l,x,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_FILE_EXISTS(f,l,x) ___ETS_ASSERT_FILE_EXISTS(f,l,x,0)
#   define _TS_ASSERT_FILE_EXISTS(f,l,x) ___TS_ASSERT_FILE_EXISTS(f,l,x,0)

#   define ETS_ASSERT_FILE_EXISTS(x) _ETS_ASSERT_FILE_EXISTS(__FILE__,__LINE__,x)
#   define TS_ASSERT_FILE_EXISTS(x) _TS_ASSERT_FILE_EXISTS(__FILE__,__LINE__,x)

#   define _ETSM_ASSERT_FILE_EXISTS(f,l,m,x) ___ETS_ASSERT_FILE_EXISTS(f,l,x,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_FILE_EXISTS(f,l,m,x) ___TS_ASSERT_FILE_EXISTS(f,l,x,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_FILE_EXISTS(m,x) _ETSM_ASSERT_FILE_EXISTS(__FILE__,__LINE__,m,x)
#   define TSM_ASSERT_FILE_EXISTS(m,x) _TSM_ASSERT_FILE_EXISTS(__FILE__,__LINE__,m,x)

// TS_ASSERT_FILE_NOT_EXISTS
#   define ___ETS_ASSERT_FILE_NOT_EXISTS(f,l,x,m) CxxTest::countAssert(); CxxTest::doAssertFileNotExists((f), (l), CxxTest::asCStyleString(x), (m) )
#   define ___TS_ASSERT_FILE_NOT_EXISTS(f,l,x,m) { _TS_TRY { ___ETS_ASSERT_FILE_NOT_EXISTS(f,l,x,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_FILE_NOT_EXISTS(f,l,x) ___ETS_ASSERT_FILE_NOT_EXISTS(f,l,x,0)
#   define _TS_ASSERT_FILE_NOT_EXISTS(f,l,x) ___TS_ASSERT_FILE_NOT_EXISTS(f,l,x,0)

#   define ETS_ASSERT_FILE_NOT_EXISTS(x) _ETS_ASSERT_FILE_NOT_EXISTS(__FILE__,__LINE__,x)
#   define TS_ASSERT_FILE_NOT_EXISTS(x) _TS_ASSERT_FILE_NOT_EXISTS(__FILE__,__LINE__,x)

#   define _ETSM_ASSERT_FILE_NOT_EXISTS(f,l,m,x) ___ETS_ASSERT_FILE_NOT_EXISTS(f,l,x,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_FILE_NOT_EXISTS(f,l,m,x) ___TS_ASSERT_FILE_NOT_EXISTS(f,l,x,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_FILE_NOT_EXISTS(m,x) _ETSM_ASSERT_FILE_NOT_EXISTS(__FILE__,__LINE__,m,x)
#   define TSM_ASSERT_FILE_NOT_EXISTS(m,x) _TSM_ASSERT_FILE_NOT_EXISTS(__FILE__,__LINE__,m,x)


// TS_ASSERT_FILE_SIZE
#   define ___ETS_ASSERT_FILE_SIZE(f,l,x,s,m) CxxTest::countAssert(); CxxTest::doAssertFileSize((f), (l), CxxTest::asCStyleString(x), (s), (m))
#   define ___TS_ASSERT_FILE_SIZE(f,l,x,s,m) { _TS_TRY { ___ETS_ASSERT_FILE_SIZE(f,l,x,s,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_FILE_SIZE(f,l,x,s) ___ETS_ASSERT_FILE_SIZE(f,l,x,s,0)
#   define _TS_ASSERT_FILE_SIZE(f,l,x,s) ___TS_ASSERT_FILE_SIZE(f,l,x,s,0)

#   define ETS_ASSERT_FILE_SIZE(x,s) _ETS_ASSERT_FILE_SIZE(__FILE__,__LINE__,x,s)
#   define TS_ASSERT_FILE_SIZE(x,s) _TS_ASSERT_FILE_SIZE(__FILE__,__LINE__,x,s)

#   define _ETSM_ASSERT_FILE_SIZE(f,l,m,x,s) ___ETS_ASSERT_FILE_SIZE(f,l,x,s,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_FILE_SIZE(f,l,m,x,s) ___TS_ASSERT_FILE_SIZE(f,l,x,s,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_FILE_SIZE(m,x,s) _ETSM_ASSERT_FILE_SIZE(__FILE__,__LINE__,m,x,s)
#   define TSM_ASSERT_FILE_SIZE(m,x,s) _TSM_ASSERT_FILE_SIZE(__FILE__,__LINE__,m,x,s)

// TS_ASSERT_FILE_EMPTY
#   define ___ETS_ASSERT_FILE_EMPTY(f,l,x,m) CxxTest::countAssert(); CxxTest::doAssertFileSize((f), (l), CxxTest::asCStyleString(x), 0, (m))
#   define ___TS_ASSERT_FILE_EMPTY(f,l,x,m) { _TS_TRY { ___ETS_ASSERT_FILE_EMPTY(f,l,x,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_FILE_EMPTY(f,l,x) ___ETS_ASSERT_FILE_EMPTY(f,l,x,0)
#   define _TS_ASSERT_FILE_EMPTY(f,l,x) ___TS_ASSERT_FILE_EMPTY(f,l,x,0)

#   define ETS_ASSERT_FILE_EMPTY(x) _ETS_ASSERT_FILE_EMPTY(__FILE__,__LINE__,x)
#   define TS_ASSERT_FILE_EMPTY(x) _TS_ASSERT_FILE_EMPTY(__FILE__,__LINE__,x)

#   define _ETSM_ASSERT_FILE_EMPTY(f,l,m,x) ___ETS_ASSERT_FILE_EMPTY(f,l,x,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_FILE_EMPTY(f,l,m,x) ___TS_ASSERT_FILE_EMPTY(f,l,x,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_FILE_EMPTY(m,x) _ETSM_ASSERT_FILE_EMPTY(__FILE__,__LINE__,m,x)
#   define TSM_ASSERT_FILE_EMPTY(m,x) _TSM_ASSERT_FILE_EMPTY(__FILE__,__LINE__,m,x)

// TS_ASSERT_FILE_CONTENT
// Only compare text file content (not binary)
// Ignore new lines at end of file
//
#   define ___ETS_ASSERT_FILE_CONTENT(f,l,cfp,efc,m) CxxTest::countAssert(); CxxTest::doAssertFileContent((f), (l), CxxTest::asCStyleString(cfp), #efc, CxxTest::asCStyleString(efc), (m))
#   define ___TS_ASSERT_FILE_CONTENT(f,l,cfp,efc,m) { _TS_TRY { ___ETS_ASSERT_FILE_CONTENT(f,l,cfp,efc,m); } __TS_CATCH(f,l) }

#   define _ETS_ASSERT_FILE_CONTENT(f,l,cfp,efc) ___ETS_ASSERT_FILE_CONTENT(f,l,cfp,efc,0)
#   define _TS_ASSERT_FILE_CONTENT(f,l,cfp,efc) ___TS_ASSERT_FILE_CONTENT(f,l,cfp,efc,0)

#   define ETS_ASSERT_FILE_CONTENT(cfp,efc) _ETS_ASSERT_FILE_CONTENT(__FILE__,__LINE__,cfp,efc)
#   define TS_ASSERT_FILE_CONTENT(cfp,efc) _TS_ASSERT_FILE_CONTENT(__FILE__,__LINE__,cfp,efc)

#   define _ETSM_ASSERT_FILE_CONTENT(f,l,m,cfp,efc) ___ETS_ASSERT_FILE_CONTENT(f,l,cfp,efc,TS_AS_STRING_NO_QUOTES(m))
#   define _TSM_ASSERT_FILE_CONTENT(f,l,m,cfp,efc) ___TS_ASSERT_FILE_CONTENT(f,l,cfp,efc,TS_AS_STRING_NO_QUOTES(m))

#   define ETSM_ASSERT_FILE_CONTENT(m,cfp,efc) _ETSM_ASSERT_FILE_CONTENT(__FILE__,__LINE__,m,cfp,efc)
#   define TSM_ASSERT_FILE_CONTENT(m,cfp,efc) _TSM_ASSERT_FILE_CONTENT(__FILE__,__LINE__,m,cfp,efc)




// TS_ASSERT_THROWS
#   define ___TS_ASSERT_THROWS(f,l,e,t,m) ___TS_ASSERT_THROWS_ASSERT(f,l,e,t,(void)0,m)

#   define _TS_ASSERT_THROWS(f,l,e,t) ___TS_ASSERT_THROWS(f,l,e,t,0)
#   define TS_ASSERT_THROWS(e,t) _TS_ASSERT_THROWS(__FILE__,__LINE__,e,t)

#   define _TSM_ASSERT_THROWS(f,l,m,e,t) ___TS_ASSERT_THROWS(f,l,e,t,TS_AS_STRING_NO_QUOTES(m))
#   define TSM_ASSERT_THROWS(m,e,t) _TSM_ASSERT_THROWS(__FILE__,__LINE__,m,e,t)

// TS_ASSERT_THROWS_ASSERT
#   define ___TS_ASSERT_THROWS_ASSERT(f,l,e,t,a,m) \
          { \
            bool hasThrown = false;       \
            CxxTest::countAssert();       \
            _TS_TRY \
            { \
              try{ e; } \
              _TS_CATCH_TYPE( (t), { a; hasThrown = true; CxxTest::doAssertThrows((f), (l), #e, (m), #t, NULL, hasThrown,  false); } ) \
            } \
            _TS_CATCH_ABORT( { throw; } ) \
            _TS_CATCH_STD( ex, { hasThrown = true; CxxTest::doAssertThrows((f), (l), #e, (m), #t, ex.what(), hasThrown,  true ); } ) \
            _TS_LAST_CATCH(    { hasThrown = true; CxxTest::doAssertThrows((f), (l), #e, (m), #t, NULL,      hasThrown,  true ); } ) \
                                                                   \
            if (!hasThrown)                                        \
            {                                                      \
              CxxTest::doAssertThrows((f), (l), #e, (m), #t, NULL, false, false);  \
            }                                                      \
          }

#   define _TS_ASSERT_THROWS_ASSERT(f,l,e,t,a) ___TS_ASSERT_THROWS_ASSERT(f,l,e,t,a,0)
#   define TS_ASSERT_THROWS_ASSERT(e,t,a) _TS_ASSERT_THROWS_ASSERT(__FILE__,__LINE__,e,t,a)

#   define _TSM_ASSERT_THROWS_ASSERT(f,l,m,e,t,a) ___TS_ASSERT_THROWS_ASSERT(f,l,e,t,a,TS_AS_STRING_NO_QUOTES(m))
#   define TSM_ASSERT_THROWS_ASSERT(m,e,t,a) _TSM_ASSERT_THROWS_ASSERT(__FILE__,__LINE__,m,e,t,a)

// TS_ASSERT_THROWS_EQUALS
#   define TS_ASSERT_THROWS_EQUALS(e,t,x,y) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_EQUALS(x,y))
#   define TSM_ASSERT_THROWS_EQUALS(m,e,t,x,y) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_EQUALS(m,x,y))

// Special foating point values support
#   define TS_ASSERT_THROWS_IS_NAN(e,t,x) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_IS_NAN(x))
#   define TSM_ASSERT_THROWS_IS_NAN(m,e,t,x) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_IS_NAN(m,x))

#   define TS_ASSERT_THROWS_IS_INFINITE(e,t,x) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_IS_INFINITE(x))
#   define TSM_ASSERT_THROWS_IS_INFINITE(m,e,t,x) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_IS_INFINITE(m,x))

// TS_ASSERT_THROWS_DIFFERS
#   define TS_ASSERT_THROWS_DIFFERS(e,t,x,y) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_DIFFERS(x,y))
#   define TSM_ASSERT_THROWS_DIFFERS(m,e,t,x,y) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_DIFFERS(m,x,y))

// TS_ASSERT_THROWS_DELTA
#   define TS_ASSERT_THROWS_DELTA(e,t,x,y,d) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_DELTA(x,y,d))
#   define TSM_ASSERT_THROWS_DELTA(m,e,t,x,y,d) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_DELTA(m,x,y,d))

// TS_ASSERT_THROWS_SAME_DATA
#   define TS_ASSERT_THROWS_SAME_DATA(e,t,x,y,s) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_SAME_DATA(x,y,s))
#   define TSM_ASSERT_THROWS_SAME_DATA(m,e,t,x,y,s) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_SAME_DATA(m,x,y,s))

// TS_ASSERT_THROWS_LESS_THAN
#   define TS_ASSERT_THROWS_LESS_THAN(e,t,x,y) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_LESS_THAN(x,y))
#   define TSM_ASSERT_THROWS_LESS_THAN(m,e,t,x,y) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_LESS_THAN(m,x,y))

// TS_ASSERT_THROWS_LESS_THAN_EQUALS
#   define TS_ASSERT_THROWS_LESS_THAN_EQUALS(e,t,x,y) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_LESS_THAN_EQUALS(x,y))
#   define TSM_ASSERT_THROWS_LESS_THAN_EQUALS(m,e,t,x,y) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_LESS_THAN_EQUALS(m,x,y))

// TS_ASSERT_THROWS_PREDICATE
#   define TS_ASSERT_THROWS_PREDICATE(e,t,p,v) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_PREDICATE(p,v))
#   define TSM_ASSERT_THROWS_PREDICATE(m,e,t,p,v) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_PREDICATE(m,p,v))

// TS_ASSERT_THROWS_RELATION
#   define TS_ASSERT_THROWS_RELATION(e,t,r,x,y) TS_ASSERT_THROWS_ASSERT(e,t,TS_ASSERT_RELATION(r,x,y))
#   define TSM_ASSERT_THROWS_RELATION(m,e,t,r,x,y) TSM_ASSERT_THROWS_ASSERT(m,e,t,TSM_ASSERT_RELATION(m,r,x,y))

// TS_ASSERT_THROWS_ANYTHING
#   define ___TS_ASSERT_THROWS_ANYTHING(f,l,e,m) \
          { \
            CxxTest::countAssert(); \
            bool _ts_threw = false; \
            _TS_TRY { e; } \
            _TS_LAST_CATCH( { _ts_threw = true; } ) \
            CxxTest::doAssertThrowsAnything((f), (l), #e, (m), _ts_threw);  \
          }

#   define _TS_ASSERT_THROWS_ANYTHING(f,l,e) ___TS_ASSERT_THROWS_ANYTHING(f,l,e,0)
#   define TS_ASSERT_THROWS_ANYTHING(e) _TS_ASSERT_THROWS_ANYTHING(__FILE__, __LINE__, e)

#   define _TSM_ASSERT_THROWS_ANYTHING(f,l,m,e) ___TS_ASSERT_THROWS_ANYTHING(f,l,e,TS_AS_STRING_NO_QUOTES(m))
#   define TSM_ASSERT_THROWS_ANYTHING(m,e) _TSM_ASSERT_THROWS_ANYTHING(__FILE__,__LINE__,m,e)

// TS_ASSERT_THROWS_NOTHING
#   define ___TS_ASSERT_THROWS_NOTHING(f,l,e,m) \
          { \
            CxxTest::countAssert(); \
            _TS_TRY { e; } \
            _TS_CATCH_ABORT( { throw; } ) \
            _TS_CATCH_STD(ex, { CxxTest::doAssertThrowsNothing( (f), (l), #e, (m), ex.what(), true); } ) \
            _TS_LAST_CATCH(   { CxxTest::doAssertThrowsNothing( (f), (l), #e, (m), 0, true); } ) \
            CxxTest::doAssertThrowsNothing( (f), (l), #e, (m), 0, false); \
          }

#   define _TS_ASSERT_THROWS_NOTHING(f,l,e) ___TS_ASSERT_THROWS_NOTHING(f,l,e,0)
#   define TS_ASSERT_THROWS_NOTHING(e) _TS_ASSERT_THROWS_NOTHING(__FILE__,__LINE__,e)

#   define _TSM_ASSERT_THROWS_NOTHING(f,l,m,e) ___TS_ASSERT_THROWS_NOTHING(f,l,e,TS_AS_STRING_NO_QUOTES(m))
#   define TSM_ASSERT_THROWS_NOTHING(m,e) _TSM_ASSERT_THROWS_NOTHING(__FILE__,__LINE__,m,e)


//
// This takes care of "signed <-> unsigned" warnings
//
#   define CXXTEST_COMPARISONS(CXXTEST_X, CXXTEST_Y, CXXTEST_T) \
    template<> inline bool areEqual<CXXTEST_X, CXXTEST_Y>(const CXXTEST_X& x, const CXXTEST_Y& y) { return static_cast<CXXTEST_T>(x) == static_cast<CXXTEST_T>(y); } \
    template<> inline bool areEqual<CXXTEST_Y, CXXTEST_X>(const CXXTEST_Y& x, const CXXTEST_X& y) { return static_cast<CXXTEST_T>(x) == static_cast<CXXTEST_T>(y); } \
    template<> struct lessThan<CXXTEST_X,CXXTEST_Y> {                                         \
        static bool test(CXXTEST_X x,CXXTEST_Y y) {                                           \
            return lessThan<CXXTEST_T,CXXTEST_T>::test((CXXTEST_T)x,(CXXTEST_T)y); } };       \
    template<> struct lessThan<CXXTEST_Y,CXXTEST_X> {                                         \
        static bool test(CXXTEST_Y x,CXXTEST_X y) {                                           \
            return lessThan<CXXTEST_T,CXXTEST_T>::test((CXXTEST_T)x,(CXXTEST_T)y); } };       \
    template<> struct lessThanEquals<CXXTEST_X,CXXTEST_Y> {                                   \
        static bool test(CXXTEST_X x,CXXTEST_Y y) {                                           \
            return lessThanEquals<CXXTEST_T,CXXTEST_T>::test((CXXTEST_T)x,(CXXTEST_T)y); } }; \
    template<> struct lessThanEquals<CXXTEST_Y,CXXTEST_X> {                                   \
        static bool test(CXXTEST_Y x,CXXTEST_X y) {                                           \
            return lessThanEquals<CXXTEST_T,CXXTEST_T>::test((CXXTEST_T)x,(CXXTEST_T)y); } }

// ---------------- Plain char conversion with unsigned int
//
template<> inline bool areEqual<char, unsigned int>(const char& x, const unsigned int& y) { return static_cast<unsigned int>(x) == y; } \


#   define CXXTEST_INTEGRAL(CXXTEST_T) \
    CXXTEST_COMPARISONS( signed CXXTEST_T, unsigned CXXTEST_T, unsigned CXXTEST_T )

CXXTEST_INTEGRAL(char);
CXXTEST_INTEGRAL(short);
CXXTEST_INTEGRAL(int);
CXXTEST_INTEGRAL(long);
#   ifdef _CXXTEST_LONGLONG
CXXTEST_INTEGRAL(_CXXTEST_LONGLONG);
#   endif // _CXXTEST_LONGLONG

#   define CXXTEST_SMALL_BIG(CXXTEST_SMALL, CXXTEST_BIG) \
    CXXTEST_COMPARISONS(unsigned CXXTEST_SMALL, signed   CXXTEST_BIG,   unsigned CXXTEST_BIG); \
    CXXTEST_COMPARISONS(signed   CXXTEST_SMALL, unsigned CXXTEST_BIG,   unsigned CXXTEST_BIG)

CXXTEST_SMALL_BIG(char,  short);
CXXTEST_SMALL_BIG(char,  int);
CXXTEST_SMALL_BIG(short, int);
CXXTEST_SMALL_BIG(char,  long);
CXXTEST_SMALL_BIG(short, long);
CXXTEST_SMALL_BIG(int,   long);

#   ifdef _CXXTEST_LONGLONG
CXXTEST_SMALL_BIG(char,  _CXXTEST_LONGLONG);
CXXTEST_SMALL_BIG(short, _CXXTEST_LONGLONG);
CXXTEST_SMALL_BIG(int,   _CXXTEST_LONGLONG);
CXXTEST_SMALL_BIG(long,  _CXXTEST_LONGLONG);
#   endif // _CXXTEST_LONGLONG
}

#ifdef _CXXTEST_HAVE_STD
#   include <cxxtest/StdTestSuite.h>
#endif // _CXXTEST_HAVE_STD

#if (__cplusplus >= 201103L)
#include <cxxtest/TestSuiteEx.h>
#endif

#endif // __cxxtest__TestSuite_h__

