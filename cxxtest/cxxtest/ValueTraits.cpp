//-------------------------------------------------------------------------
// CxxTest: A lightweight C++ unit testing library.
// Copyright (c) 2008 Sandia Corporation.
// This software is distributed under the LGPL License v3
// For more information, see the COPYING file in the top CxxTest directory.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//-------------------------------------------------------------------------

#ifndef __cxxtest__ValueTraits_cpp__
#define __cxxtest__ValueTraits_cpp__

#include <cxxtest/ValueTraits.h>

namespace CxxTest
{

static CharacterMapping currentCharacterMapping = MAP_CHARS_FULL;
static bool             stringResultsOnNewLines = false;

//! Returns current character mapping
CharacterMapping charactersMapping ()
{
  return currentCharacterMapping;
}


//! Changes characters mapping mode
//! @see charToString
CharacterMapping setCharactersMapping (CharacterMapping newMapping)
{
  CharacterMapping prev   = currentCharacterMapping;

  currentCharacterMapping = newMapping;

  return prev;
}

//! Restores default characters mapping mode
CharacterMapping setDefaultCharactersMapping ()
{
  return setCharactersMapping(MAP_CHARS_FULL);
}

//!< Returns true when a new line is used (instead of quote) when displaying string
//!< results (either got and expected)
//!<
bool stringResultsOnNewLine() { return stringResultsOnNewLines; }

//!< Tells whether a new line is used (instead of quote) when displaying string results
//!<
bool setStringResultsOnNewLine(bool newState)
{
  bool previousState = stringResultsOnNewLines;
  stringResultsOnNewLines = newState;
  return previousState;
}

//
// Non-inline functions from ValueTraits.h
//

char digitToChar(unsigned int digit)
{
    if (digit < 10)
    {
        return (char)('0' + digit);
    }
    if (digit <= 10 + 'Z' - 'A')
    {
        return (char)('A' + digit - 10);
    }
    return '?';
}

char* byteToHex(unsigned char byte, char asHex[3])
{
    asHex[0] = digitToChar(byte >> 4);
    asHex[1] = digitToChar(byte & 0x0F);
    asHex[2] = '\0';
    return asHex;
}

char* copyString(char* dst, const char* src)
{
    while ((*dst = *src) != '\0')
    {
        ++ dst;
        ++ src;
    }
    return dst;
}

// Copies a string (limiting to a number of characters).
// Remember -- we can't use the standard library!
// Note: After return, dst will be null terminated (unless maxBytes was zero)
//
// Returns one past the end of the destination string (it points to NULL character)
//
char* copyStringN(char* dst, const char* src, size_t maxBytes)
{
    while ((maxBytes > 1) && ((*dst = *src) != '\0'))
    {
        ++dst;
        ++src;
        --maxBytes;
    }

    if (maxBytes == 1)
    {
      *dst = '\0';  // Force string terminator (to avoid undefined behaviour)
    }
    return dst;
}

bool stringsEqual(const char* s1, const char* s2)
{
    char c;
    while ((c = *s1++) == *s2++)
    {
        if (c == '\0')
        {
            return true;
        }
    }
    return false;
}

char* charToString(unsigned long c, char* s)
{
    // ---------------- Minimal character mapping
    //
    switch (c)
    {
      case '\0': return copyString(s, "\\0");
      case '\a': return copyString(s, "\\a");
      case '\b': return copyString(s, "\\b");
      default : break;
    }

    bool keepCharAsIs = false;
    if (currentCharacterMapping == MAP_CHARS_FULL)
    {
      switch (c)
      {
        case '\\': return copyString(s, "\\\\");
        case '\"': return copyString(s, "\\\"");
        case '\'': return copyString(s, "\\\'");
        case '\n': return copyString(s, "\\n");
        case '\r': return copyString(s, "\\r");
        case '\t': return copyString(s, "\\t");
        default : break;
      }
    }
    else
    {
      switch (c)
      {
        case '\n':
        case '\r':
        case '\t':
          keepCharAsIs = true;
          break;
        default : break;
      }
    }

    if ((c >= 32 && c <= 127) || keepCharAsIs)
    {
        s[0] = (char)c;
        s[1] = '\0';
        return s + 1;
    }

    s[0] = '\\';
    s[1] = 'x';
    if (c < 0x10)
    {
        s[2] = '0';
        ++ s;
    }
    return numberToString(c, s + 2, 16UL);
}

char* charToString(char c, char* s)
{
    return charToString((unsigned long)(unsigned char)c, s);
}

//! Converts, N first, bytes value to their string representation
//!
char* bytesToString(const unsigned char* bytes, unsigned numBytes, unsigned maxBytes, char* s)
{
    bool truncate = (numBytes > maxBytes);
    if (truncate)
    {
        numBytes = maxBytes;
    }

    char asHex[3];

    s = copyString(s, "{ ");
    for (unsigned i = 0; i < numBytes; ++i, ++bytes)
    {
      unsigned char nextByte = *bytes;
      byteToHex(nextByte, asHex);
      s = copyString(s, asHex);
      s = copyString(s, " ");
    }

    if (truncate)
    {
      s = copyString(s, "...");
    }

    return copyString(s, " }");
}

//! Converts, a pointer to its string representation
//!
char* pointerToString(const void* pointer, size_t maxBytes, char* dest)
{
    static const size_t sizeRequirement = sizeof("0x") +  2 * sizeof(void*) + 1;

    if (maxBytes < sizeRequirement)
    {
        if (maxBytes != 0)
        {
            std::memset (dest, '?', maxBytes);
            dest[maxBytes - 1] = '\0';
            dest += maxBytes;
        }
    }
    else
    {
        static const size_t loopCount = sizeof(void*);

        dest = copyString(dest, "0x");

        unsigned int pointerValue = reinterpret_cast<unsigned int>(pointer);
        size_t       bitsShift    = (sizeof(void*) - 1) * 8;

        char asHex[3];
        for (size_t ii = 0; ii < loopCount; ++ii)
        {
            unsigned int byte = (pointerValue >> bitsShift) & 0xff;

            byteToHex(static_cast<unsigned char>(byte), asHex);
            dest = copyString(dest, asHex);

            bitsShift -= 8;
        }
    }

    return dest;
}


#ifndef CXXTEST_USER_VALUE_TRAITS
unsigned ValueTraits<double>::requiredDigitsOnLeft(double t)
{
    unsigned digits = 1;
    for (t = (t < 0.0) ? -t : t; t > 1.0; t /= BASE)
    {
        ++ digits;
    }
    return digits;
}

char* ValueTraits<double>::doNegative(double& t)
{
    if (t >= 0)
    {
        return _asString;
    }
    _asString[0] = '-';
    t = -t;
    return _asString + 1;
}

void ValueTraits<double>::hugeNumber(double t)
{
    char* s = doNegative(t);
    s = doubleToString(t, s, 0, 1);
    s = copyString(s, ".");
    s = doubleToString(t, s, 1, MAX_DIGITS_ON_RIGHT);
    s = copyString(s, "E");
    s = numberToString(requiredDigitsOnLeft(t) - 1, s);
}

void ValueTraits<double>::normalNumber(double value)
{
    // ---------------- Manage negative value
    //
    char* s = doNegative(value);

    // ---------------- Convert integral part
    //
    s = doubleToString(value, s);

    s = copyString(s, ".");

    // ---------------- Convert decimal part
    //
    int integralPart = static_cast<unsigned long>(value);
    value -= static_cast<double>(integralPart);
    for (unsigned i = 0; i < MAX_DIGITS_ON_RIGHT; ++ i)
    {
        value *= BASE;
        unsigned int digit = (unsigned int)(value) % BASE;
        s = numberToString(digit, s);
        value -= static_cast<double>(digit);
    }

    // ---------------- Remove useless trailing zeros
    //
    char* pLast         = s     -1;
    char* pSecondToLast = pLast -1;
    while ((*pLast == '0') && (*pSecondToLast != '.'))
    {
        *pLast = '\0';
        --pLast;
        --pSecondToLast;
    }
}

void ValueTraits<double>::nonFiniteNumber(double t)
{
    char* s = _asString;
    if (t != t)
    {
        s = copyString(s, "nan");
    }
    //else if ( t == 1.0/0.0 )
    else if (t >= HUGE_VAL)
    {
        s = copyString(s, "-inf");
    }
    else if (t <= -HUGE_VAL)
    {
        //else if ( t == -1.0/0.0 )
        s = copyString(s, "inf");
    }
}

char* ValueTraits<double>::doubleToString(double t, char* s, unsigned skip, unsigned max)
{
    return numberToString<double>(t, s, BASE, skip, max);
}
#endif // !CXXTEST_USER_VALUE_TRAITS
}

#endif // __cxxtest__ValueTraits_cpp__
