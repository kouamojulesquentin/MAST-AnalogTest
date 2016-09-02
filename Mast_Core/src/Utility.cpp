//===========================================================================
//                           Utility.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Utility.cpp
//!
//! Implements Utility class methods
//!
//===========================================================================

#include "Utility.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>

using std::string;
using std::experimental::string_view;
using std::vector;
using std::ifstream;

using namespace mast;



//! Tests if a file exists
//!
//! @param filePath File path
//!
//! @return true when file can be opened, false otherwise
bool Utility::FileExists (string_view filePath)
{
  ifstream ifs;
  ifs.open(filePath.data());
  return ifs.is_open();
}
//
//  End of: Utility::FileExists
//---------------------------------------------------------------------------


//! Formats message for (std) exception
//!
//! @note It can also be used for non std::exception derived class
//! @note Format example:
//!   std::invalid_argument: Number of append bits cannot exceed number of bits of value.
//!   In File:     C:\Users\Jean-Francois\Documents\Tima\NewMast\Lib\src\BinaryVector.cpp
//!   In Function:
//!   At Line:     87'
//!
string Utility::MakeExceptionMessage(const char*                    file,
                                     const char*                    function,
                                     uint32_t                       line,
                                     std::experimental::string_view issueKind,
                                     std::experimental::string_view userMessage)
{
  std::ostringstream os;

  os << issueKind << ": " << userMessage << "." << std::endl;
  os << "In file:     "  << file << std::endl;
  if (function)
  {
    os << "In function: "  << function << std::endl;
  }
  os << "At line:     "  << line << std::endl;

  return os.str();
}


//! Returns the minimal number of bits to encode a binary value
//!
//! @note This is not optimized (is faster for small value than for large one)
//!
//! @param value  A value for which the minimal number of bits is required
//!
uint32_t Utility::MinimalBitsForValue (uint32_t value)
{
  constexpr std::array<uint32_t, 16> bitsForVal =
  {
    1,  // 00
    1,  // 01
    2,  // 02
    2,  // 03
    3,  // 04
    3,  // 05
    3,  // 06
    3,  // 07
    4,  // 08
    4,  // 09
    4,  // 10
    4,  // 11
    4,  // 12
    4,  // 13
    4,  // 14
    4,  // 15
  };

  uint32_t bitsCount = 0;

  while (value > 15)
  {
    bitsCount +=  4;
    value     >>= 4;
  }

  bitsCount += bitsForVal[value];

  return bitsCount;
}
//
//  End of: Utility::MinimalBitsForValue
//---------------------------------------------------------------------------


//! Splits a string_view into bunch of string_view
//!
//! @note No trimming is done
//!
//! @param text         Text to spli
//! @param separator    Chunk separator (may be several character width)
//!
//! @return A list of string_view (tokens) without the separators
vector<string_view> Utility::Split (string_view text, string_view separator)
{
  vector<string_view> views;

  if (!text.empty())
  {
    if (separator.empty())
    {
      views.emplace_back(text);
    }
    else
    {
      string_view::size_type startPos = 0;
      string_view::size_type endPos   = 0;

      while ((endPos = text.find(separator, startPos)) != std::string::npos)
      {
        auto count = endPos - startPos;                    // Count ignore separator
        views.emplace_back(text.substr(startPos, count));
        startPos = endPos + separator.length();            // Prepare skiping of separator
      }

      views.emplace_back(text.substr(startPos));  // Last chunk
    }
  }

  return views;
}
//
//  End of: Utility::Split
//---------------------------------------------------------------------------



//! Trims leading space characters (including \t)
//!
//! @param text A string_view to trim
//!
void Utility::TrimLeft (string_view& text)
{
  if (!text.empty())
  {
    if ((text[0] == ' ') || (text[0] == '\t'))
    {
      auto startPos  = text.find_first_not_of(" \t");
      auto trimCount = (startPos == text.npos) ? text.length() : startPos;
      text.remove_prefix(trimCount);
    }
  }
}
//
//  End of: Utility::TrimLeft
//---------------------------------------------------------------------------


//! Trims trailing space characters (including \t)
//!
//! @param text A string_view to trim
//!
void Utility::TrimRight (string_view& text)
{
  if (!text.empty())
  {
    if ((text.back() == ' ') || (text.back() == '\t'))
    {
      auto startPos  = text.find_last_not_of(" \t");
      auto trimCount = (startPos == text.npos) ? text.length() : text.length() - ++startPos;
      text.remove_suffix(trimCount);
    }
  }
}
//
//  End of: Utility::TrimLeft
//---------------------------------------------------------------------------



//===========================================================================
// End of Utility.cpp
//===========================================================================
