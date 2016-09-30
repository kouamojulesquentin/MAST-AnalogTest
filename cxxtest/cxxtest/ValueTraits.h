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

#ifndef __cxxtest__ValueTraits_h__
#define __cxxtest__ValueTraits_h__

//
// ValueTraits are used by CxxTest to convert arbitrary
// values used in TS_ASSERT_EQUALS() to a string representation.
//
// This header file contains value traits for builtin integral types.
// To declare value traits for new types you should instantiate the class
// ValueTraits<YourClass>.
//

#include <cxxtest/Flags.h>

#ifdef _CXXTEST_OLD_TEMPLATE_SYNTAX
#   define CXXTEST_TEMPLATE_INSTANTIATION
#else // !_CXXTEST_OLD_TEMPLATE_SYNTAX
#   define CXXTEST_TEMPLATE_INSTANTIATION template<>
#endif // _CXXTEST_OLD_TEMPLATE_SYNTAX

#ifdef _CXXTEST_HAVE_STD
#include <cmath>
#include <cstring>
#include <string>
#else
#include <math.h>
#endif

namespace CxxTest
{
//
// This is how we use the value traits
//
#define TS_AS_STRING(x)           CxxTest::traits(x).asString()
#define TS_AS_STRING_NO_QUOTES(x) CxxTest::traits_no_quotes(x).asString()

//! Non visible and some other character like quotes, backslash... are remapped to equivalent escaped string char.
//! This enum defines which kind of characters are remapped
//!
//! @see charToString
enum  CharacterMapping
{
  MAP_CHARS_FULL,     //!< By default there is 9 characters that are re-mapped to escaped and non ASCII and control are replaced with their hex value
  MAP_CHARS_MINIMAL,  //!< Only re-maps those that would make the display "unreadable"
};

CharacterMapping charactersMapping();                                 //!< Returns current character mapping
CharacterMapping setCharactersMapping(CharacterMapping newMapping);   //!< Changes characters mapping mode
CharacterMapping setDefaultCharactersMapping();                       //!< Restores default characters mapping mode
bool             stringResultsOnNewLine();                            //!< Returns true when a new line is used (instead of quote) when displaying string results (either got and expected)
bool             setStringResultsOnNewLine(bool newState);            //!< Tells whether a new line is used (instead of quote) when displaying string results
bool             displayUnsignedAsHex();                              //!< Returns true when a requested to display unsigned int value as hex string (instead of decimal)
bool             setDisplayUnsignedAsHex(bool newState);              //!< Sets whether requesting to display unsigned int value as hex string (instead of decimal)

// Char representation of a digit
//
char digitToChar(unsigned digit);

// Convert bytes value to hex digits
//
// Returns pointer to asHex array
//
char* byteToHex(unsigned char byte, char asHex[3]);

//
// Convert byte values to string
// Returns one past the copied data
//
char* bytesToString(const unsigned char* bytes, unsigned numBytes, unsigned maxBytes, char* s);

//
// Copy a string.
// Returns one past the end of the destination string
// Remember -- we can't use the standard library!
//
char* copyString(char* dst, const char* src);

//
// Copy a string (limiting to a number of characters).
// Returns one past the end of the destination string
// Remember -- we can't use the standard library!
//
char* copyStringN(char* dst, const char* src, size_t maxBytes);

//! Converts, a pointer to its string representation
//!
char* pointerToString(const void* pointer, size_t maxBytes, char* dest);

//
// Compare two strings.
// Remember -- we can't use the standard library!
//
bool stringsEqual(const char* s1, const char* s2);

//
// Represent a character value as a string
// Returns one past the end of the string
// This will be the actual char if printable or '\xXXXX' otherwise
//
char* charToString(unsigned long c, char* s);

//
// Prevent problems with negative (signed char)s
//
char* charToString(char c, char* s);

//! Returns true if some text pointer is null or the text is empty
//!
inline bool isNullOrEmpty (const char* text)
{
  return !text || (std::strlen(text) == 0);
}

//
// The default ValueTraits class dumps up to 8 bytes as hex values
//
template <typename T>
class ValueTraits
{
    enum { MAX_BYTES = 8, BUFFER_SIZE = sizeof("{ ") + sizeof("XX ") * MAX_BYTES + sizeof("... }") };
    char m_valueAsString[BUFFER_SIZE];

    ValueTraits() {}
public:
    ~ValueTraits() {}   // To force presence of explicit destructor (February/18/2015: to avoid using destructor of a specialization with VS2012 !!)
    ValueTraits(const T& value)
    {
      m_valueAsString[0] = '\0';
      const T* const pValue = &value;

      bytesToString(reinterpret_cast<const unsigned char *>(pValue), sizeof(T), MAX_BYTES, m_valueAsString);
    }
    const char* asString(void) const { return m_valueAsString; }
};

//! Specialization for value that are already strings
//!
template <>
class ValueTraits<char*>
{
    enum { BUFFER_SIZE = 2000 };        // Found that 1000 was not always enough
    char m_valueAsString[BUFFER_SIZE];

    void fillBuffer(const char* pText)
    {
      if (!pText)
      {
        copyStringN(m_valueAsString, "NULL", BUFFER_SIZE);
      }
      else
      {

        const char* bound    = stringResultsOnNewLine() ? "\n" : "\"";

        const char* pEndBuf  = m_valueAsString + BUFFER_SIZE;
        char*       pFreePos = m_valueAsString;

        pFreePos = copyStringN(pFreePos, bound, pEndBuf - pFreePos);
        pFreePos = copyStringN(pFreePos, pText, pEndBuf - pFreePos);
        pFreePos = copyStringN(pFreePos, bound, pEndBuf - pFreePos);
      }
    }

    public:
    ~ValueTraits() {}
    ValueTraits() {}
    ValueTraits(const char* pText) { fillBuffer(pText); }
    ValueTraits(char*       pText) { fillBuffer(pText); }

    const char* asString(void) const { return m_valueAsString; }
};

//! Partial specialization for pointers
//!
template <typename T>
class ValueTraits<T*>
{
    enum { BUFFER_SIZE = sizeof("0x") +  2 * sizeof(void*) + 1 };
    char m_valueAsString[BUFFER_SIZE];

    ValueTraits() {}
    public:
    ~ValueTraits() {}
    ValueTraits(const T* pointer)
    {
      std::memset (m_valueAsString, 0, BUFFER_SIZE);
      pointerToString(reinterpret_cast<const void *>(pointer), BUFFER_SIZE, m_valueAsString);
    }
    const char* asString(void) const { return m_valueAsString; }
};


//! Specialization for value that are array of characters
//!
template<size_t N>
class ValueTraits<char const[N]>
{
    enum { BUFFER_SIZE = N + 1 };
    char m_valueAsString[BUFFER_SIZE];

    public:
    ~ValueTraits() {}
    ValueTraits() {}
    ValueTraits(const char (&val)[N])
    {
      std::memcpy  (m_valueAsString, val, N);
      m_valueAsString[BUFFER_SIZE - 1] = '\0';
    }
    const char* asString(void) const { return m_valueAsString; }
};

//
// traits( T t )
// Creates an object of type ValueTraits<T>
//
template <class T>
inline ValueTraits<T> traits(const T& t)
{
    return ValueTraits<T>(t);
}

// Overloads "traits" function template for plain strings
//
inline ValueTraits<char*> traits(const char* pText)
{
    return ValueTraits<char*>(pText);
}

// Overloads "traits" function template for plain strings
//
inline ValueTraits<char*> traits(char* pText)
{
    return ValueTraits<char*>(pText);
}


// Creates an object of type ValueTraits<T>
//
template <class T>
inline ValueTraits<T> traits_no_quotes(const T& t)
{
    return CxxTest::traits(t);
}


inline const char* asCStyleString(const char* text)        { return text ? text : "null char*"; }

class StringTraitsNoQuote
{
    enum { BUFFER_SIZE = 1000 };
    char m_valueAsString[BUFFER_SIZE];

    void fillBuffer(const char* pText)
    {
      if (!pText)
      {
        copyStringN(m_valueAsString, "null char*", BUFFER_SIZE);
      }
      else
      {
        copyStringN(m_valueAsString, pText, BUFFER_SIZE);
      }
    }

    public:
    ~StringTraitsNoQuote() {}
    StringTraitsNoQuote() {}
    StringTraitsNoQuote(const char* pText) { fillBuffer(pText); }
    StringTraitsNoQuote(char*       pText) { fillBuffer(pText); }

    const char* asString(void) const { return m_valueAsString; }
};

// Returns string without quotes for "c" style strings
//
inline StringTraitsNoQuote traits_no_quotes(const char* pText)
{
    return CxxTest::StringTraitsNoQuote(pText);
}

// Returns string without quotes for "c" style strings
//
inline StringTraitsNoQuote traits_no_quotes(char* pText)
{
    return CxxTest::StringTraitsNoQuote(pText);
}



//
// You can duplicate the implementation of an existing ValueTraits
//
#   define CXXTEST_COPY_TRAITS(CXXTEST_NEW_CLASS, CXXTEST_OLD_CLASS) \
    CXXTEST_TEMPLATE_INSTANTIATION \
    class ValueTraits< CXXTEST_NEW_CLASS > \
    { \
        ValueTraits< CXXTEST_OLD_CLASS > _old; \
    public: \
        ValueTraits( CXXTEST_NEW_CLASS n ) : _old( static_cast<CXXTEST_OLD_CLASS>(n) ) {} \
        const char* asString( void ) const { return _old.asString(); } \
    };



//
// Certain compilers need separate declarations for T and const T
//
#   ifdef _CXXTEST_NO_COPY_CONST
#       define CXXTEST_COPY_CONST_TRAITS(CXXTEST_CLASS)
#   else // !_CXXTEST_NO_COPY_CONST
#       define CXXTEST_COPY_CONST_TRAITS(CXXTEST_CLASS)              \
        CXXTEST_TEMPLATE_INSTANTIATION                               \
        class ValueTraits< const CXXTEST_CLASS >                     \
        {                                                            \
            ValueTraits< CXXTEST_CLASS > _old;                       \
        public:                                                      \
            ValueTraits( const CXXTEST_CLASS n ) : _old((n)) {}      \
            ~ValueTraits() {}                                        \
            const char* asString() const { return _old.asString(); } \
        };

#   endif // _CXXTEST_NO_COPY_CONST

//
// Avoid compiler warnings about unsigned types always >= 0
//
template<class N> inline bool negative(N n) { return n < 0; }
template<class N> inline N abs(N n) { return negative(n) ? -n : n; }

#   define CXXTEST_NON_NEGATIVE(Type) \
    CXXTEST_TEMPLATE_INSTANTIATION \
    inline bool negative<Type>( Type ) { return false; } \
    CXXTEST_TEMPLATE_INSTANTIATION \
    inline Type abs<Type>( Type value ) { return value; }

CXXTEST_NON_NEGATIVE(bool)
CXXTEST_NON_NEGATIVE(unsigned char)
CXXTEST_NON_NEGATIVE(unsigned short int)
CXXTEST_NON_NEGATIVE(unsigned int)
CXXTEST_NON_NEGATIVE(unsigned long int)
#   ifdef _CXXTEST_LONGLONG
CXXTEST_NON_NEGATIVE(unsigned _CXXTEST_LONGLONG)
#   endif // _CXXTEST_LONGLONG

//
// Represent integral number as a string
//           ========
// Returns one past the end of the string
// Remember -- we can't use the standard library!
//
template<class N>
char* numberToString(N n, char* s,
                     N base = 10,
                     unsigned int skipDigits = 0,
                     unsigned int maxDigits  = static_cast<unsigned int>(-1))
{
    if (negative(n))
    {
        *s++ = '-';
        n = abs(n);
    }

    N digit = 1;
    while (digit <= (n / base))
    {
        digit *= base;
    }
    N digitValue;
    for (; digit >= 1 && skipDigits; n -= digit * digitValue, digit /= base, -- skipDigits)
    {
        digitValue = (unsigned)(n / digit);
    }
    for (; digit >= 1 && maxDigits; n -= digit * digitValue, digit /= base, -- maxDigits)
    {
        *s++ = digitToChar((unsigned)(digitValue = (unsigned)(n / digit)));
    }

    *s = '\0';
    return s;
}

//
// All the specific ValueTraits follow.
// You can #define CXXTEST_USER_VALUE_TRAITS if you don't want them
//

#ifndef CXXTEST_USER_VALUE_TRAITS
//
// ValueTraits: const char * const &
// This is used for printing strings, as in TS_FAIL( "Message" )
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<const char * const &>
{
    ValueTraits& operator=(const ValueTraits &);
    const char* _asString;

public:
    ValueTraits(const char * const& value) : _asString(value) {}
    ValueTraits(const ValueTraits& other)  : _asString(other._asString) {}
    ~ValueTraits() {}

    const char* asString(void) const { return _asString; }
};

//+CXXTEST_COPY_TRAITS(const char *, const char * const &)
//+CXXTEST_COPY_TRAITS(char *, const char * const &)

//
// ValueTraits: bool
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<bool>
{
    const bool _value;

public:
    ValueTraits(bool value) : _value(value) {}
    ~ValueTraits() {}
    const char* asString(void) const { return _value ? "true" : "false"; }
};

#   ifdef _CXXTEST_LONGLONG
//
// ValueTraits: signed long long
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<signed _CXXTEST_LONGLONG>
{
    typedef _CXXTEST_LONGLONG T;
    char _asString[2 + 3 * sizeof(T)];
public:
    ValueTraits(T t) { numberToString<T>(t, _asString); }
   ~ValueTraits() {}
    const char* asString(void) const { return _asString; }
};

//
// ValueTraits: unsigned long long
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<unsigned _CXXTEST_LONGLONG>
{
    typedef unsigned _CXXTEST_LONGLONG T;
    char _asString[3 + 3 * sizeof(T)];
public:
    ValueTraits(T t)
    {
      if (displayUnsignedAsHex())
      {
        char* pDst = copyString(_asString, "0x");
        numberToString<T>(t, pDst, 16);
      }
      else
      {
        numberToString<T>(t, _asString);
      }
    }
    ~ValueTraits() {}
    const char* asString(void) const { return _asString; }
};

#   endif // _CXXTEST_LONGLONG

//
// ValueTraits: signed long
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<signed long int>
{
    typedef signed long int T;
    char _asString[2 + 3 * sizeof(T)];
public:
    ValueTraits(T t) { numberToString<T>(t, _asString); }
    ~ValueTraits() {}
    const char* asString(void) const { return _asString; }
};


//
// ValueTraits: unsigned long
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<unsigned long int>
{
    typedef unsigned long int T;
    char _asString[3 + 3 * sizeof(T)];
public:
    ValueTraits(T t)
    {
      if (displayUnsignedAsHex())
      {
        char* pDst = copyString(_asString, "0x");
        numberToString<T>(t, pDst, 16);
      }
      else
      {
        numberToString<T>(t, _asString);
      }
    }
    ~ValueTraits() {}
    const char* asString(void) const { return _asString; }
};


//
// All decimals are the same as the long version
//

CXXTEST_COPY_TRAITS(signed int,         signed long int)
CXXTEST_COPY_TRAITS(unsigned int,       unsigned long int)
CXXTEST_COPY_TRAITS(signed short int,   signed long int)
CXXTEST_COPY_TRAITS(unsigned short int, unsigned long int)
CXXTEST_COPY_TRAITS(unsigned char,      unsigned long int)

//
// ValueTraits: char
// Returns 'x' for printable chars, '\x??' for others
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<char>
{
    char _asString[sizeof("'\\xXX'")];
public:
    ValueTraits(char c) { copyString(charToString(c, copyString(_asString, "'")), "'"); }
    ~ValueTraits() {}
    const char* asString(void) const { return _asString; }
};


//
// ValueTraits: signed char
// Same as char, some compilers need it
//
CXXTEST_COPY_TRAITS(signed char, char)

//
// ValueTraits: double
//
CXXTEST_TEMPLATE_INSTANTIATION
class ValueTraits<double>
{
public:
    ValueTraits(double t)
    {
        //+ (JCL February/24/2015): It does not handle properly number with negative exponent with abs(exponent) greater than MAX_DIGITS_ON_RIGHT
        if ((t != t) || (t >= HUGE_VAL) || (t == -HUGE_VAL))
        {
            nonFiniteNumber(t);
        }
        else if (requiredDigitsOnLeft(t) > MAX_DIGITS_ON_LEFT)
        {
            hugeNumber(t);
        }
        else
        {
            normalNumber(t);
        }
    }
    ~ValueTraits() {}

    const char* asString(void) const { return _asString; }

private:
    enum { MAX_DIGITS_ON_LEFT = 24, MAX_DIGITS_ON_RIGHT = 16, BASE = 10 };
    char _asString[1 + MAX_DIGITS_ON_LEFT + 1 + MAX_DIGITS_ON_RIGHT + 1];

    static unsigned requiredDigitsOnLeft(double t);

    char* doNegative      (double& t);
    void  hugeNumber      (double  t);
    void  normalNumber    (double  t);
    void  nonFiniteNumber (double  t);
    char* doubleToString  (double  t, char* s, unsigned skip = 0, unsigned max = (unsigned) - 1);
};

//
// ValueTraits: float
//
CXXTEST_COPY_TRAITS(float, double)
#endif // !CXXTEST_USER_VALUE_TRAITS
}

#ifdef _CXXTEST_HAVE_STD
#   include <cxxtest/StdValueTraits.h>
#endif // _CXXTEST_HAVE_STD

namespace cxxtest_dummy_enum_ns {}


//! Customizable traits for enum
//!
//! @see CXXTEST_ENUM_TRAITS_END
//! @note  Support C++ scoped enums
//! @note must be followed with one case statement for each enum value, that associate an enum value with its string representation,
//!       then with  CXXTEST_ENUM_TRAITS_END macro
//!
//! Usage example:
//! @code {.cpp}
//!   CXXTEST_ENUM_TRAITS_BEGIN(clang::tok::TokenKind)
//!       #define TOK(X) case clang::tok::X: return "clang::tok::" # X ;
//!       #include "Clang/TokenKinds.def"
//!   CXXTEST_ENUM_TRAITS_END()
//! @endcode
//!
#define CXXTEST_ENUM_TRAITS_BEGIN( TYPE ) \
    namespace CxxTest \
    { \
        CXXTEST_TEMPLATE_INSTANTIATION \
        class ValueTraits<TYPE> \
        { \
            TYPE _value; \
            char _fallback[sizeof("(" #TYPE ")") + 3 * sizeof(TYPE)]; \
        public: \
            ValueTraits( TYPE value ) { \
                _value = value; \
                numberToString<unsigned long int>( static_cast<unsigned long int>(_value), copyString( _fallback, "(" #TYPE ")" ) ); \
            } \
            const char* asString( void ) const \
            { \
                switch ( _value ) \
                {                 \
                  using namespace cxxtest_dummy_enum_ns

#define CXXTEST_ENUM_TRAITS_END( ) \
                    default: return _fallback; \
                } \
            } \
        }; \
    } using namespace cxxtest_dummy_enum_ns   // This is support trailing semicolon (and hopefully to better locate syntax errors in clients code)

//! Traits for enum that maps an enum value with its string representation in test results
//!
//! @note  Support C++ scoped enums
#define CXXTEST_ENUM_TRAITS( TYPE, VALUES ) \
    CXXTEST_ENUM_TRAITS_BEGIN(TYPE); \
    VALUES \
    CXXTEST_ENUM_TRAITS_END()

#define CXXTEST_ENUM_MEMBER( MEMBER ) \
    case MEMBER: return #MEMBER;

#endif // __cxxtest__ValueTraits_h__
