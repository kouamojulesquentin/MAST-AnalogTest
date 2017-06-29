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
#include "g3log/g3log.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>

using std::string;
using std::experimental::string_view;
using std::vector;
using std::ifstream;
using std::ofstream;

using namespace mast;


//! Returns last token from a string_view
//!
//! @note No trimming is done
//!
//! @param text       Text for which last token is requested
//! @param separator  Token separator (may be several character width)
//!
//! @return Last token or given text when there is no separator in text
string_view Utility::BackToken (string_view text, string_view separator)
{
  if (text.empty() || separator.empty())
  {
    return text;
  }

  string_view token;
  auto        foundPos = text.rfind(separator);

  if (foundPos == std::string::npos)
  {
    token = text;
  }
  else
  {
    foundPos += separator.length();
    token    =  text.substr(foundPos);
  }

  return token;
}
//
//  End of: Utility::BackToken
//---------------------------------------------------------------------------


//! Clears file content if the file exists
//!
//! @param filePath File path
//!
void Utility::ClearFile (string_view filePath)
{
  if (FileExists(filePath))
  {
    ofstream ofs;
    ofs.open(filePath.data());
  }
}
//
//  End of: Utility::ClearFile
//---------------------------------------------------------------------------


//! Returns whether some text ends with some sub-string
//!
//! @note This is a case sensitive comparison
//!
//! @param text       Text to compare its ends to
//! @param substring  Sub string to compare at end of text
//!
//! @return true when substring is not empty and it is found at the very end of text, false otherwise
//!
bool Utility::EndsWith (string_view text, string_view substring)
{
  if (substring.empty())
  {
    return false;
  }

  if (substring.length() > text.length())
  {
    return false;
  }

  text.remove_prefix(text.length() - substring.length());
  return text == substring;
}
//
//  End of: Utility::EndsWith
//---------------------------------------------------------------------------


//! Extracts file (or directory) parent directory path
//!
//! @param filePath  A file path
//!
//! @return Extracted path (with trailing path separator) or "." when no path has been found
string Utility::ExtractDirectoryPath (const string& filePath)
{
  // ---------------- Set dirPath with only directory path of runner application
  //
  auto dirPath       = filePath;
  auto sepPosLinux   = dirPath.rfind("/");  // Search for last Linux directory separator
  auto sepPosWindows = dirPath.rfind("\\"); // Search for last Windows directory separator
  auto sepPos        = std::string::npos;

  if (sepPosLinux == std::string::npos)
  {
    sepPos = sepPosWindows;
  }
  else if (sepPosWindows == std::string::npos)
  {
    sepPos = sepPosLinux;
  }
  else  // Both are valid
  {
    sepPos = (sepPosLinux > sepPosWindows) ? sepPosLinux : sepPosWindows;
  }

  if (sepPos != std::string::npos)
  {
    dirPath.erase(sepPos + 1);           // Remove file name (keeping only directory path)
  }
  else
  {
    dirPath = ".";                   // When no separator ==> there is only file name ==> treat it as if in current directory
  }
  return dirPath;
}
//
//  End of: Utility::ExtractApplicationDirectoryPath
//---------------------------------------------------------------------------


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



//! Returns all tokens from a string_view except last one
//!
//! @note No trimming is done
//!
//! @param text       Text for which last token is "discarded"
//! @param separator  Token separator (may be several character width)
//!
//! @return First tokens without the last one or given text
string_view Utility::FrontTokens (string_view text, string_view separator)
{
  if (text.empty())
  {
    return text;
  }

  auto count  = text.length();

  if (!separator.empty())
  {
    auto endPos = text.rfind(separator);
    if (endPos != string_view::npos)
    {
      count = endPos;  // Count ignore (discard) the separator
    }
  }

  auto tokens = text.substr(0, count);
  return tokens;
}
//
//  End of: Utility::FrontTokens
//---------------------------------------------------------------------------


//! Formats message for (std) exception
//!
//! @note It can also be used for non std::exception derived class
//! @note Format example:
//!   std::invalid_argument: Number of append bits cannot exceed number of bits of value.
//!   In File:     C:\\Users\\Jean-Francois\\Documents\\Tima\\NewMast\\Lib\\src\\BinaryVector.cpp
//!   In Function:
//!   At Line:     87
//!
string Utility::MakeExceptionMessage(const char*                    file,
                                     const char*                    function,
                                     uint32_t                       line,
                                     std::experimental::string_view issueKind,
                                     std::experimental::string_view userMessage)
{
  // ---------------- Log any exception user message
  //
  IF_CAN_LOG(ERROR_LVL) LogCapture(file, line, function, ERROR_LVL).stream() << userMessage.data();

  // ---------------- Create effective exception message
  //
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


//! Surrounds some, not empty, text with single quotes and a space after the closing quote
//!
//! @param text   Text to surround with quotes and space
//!
//! @return When not empty text parameter surrounded with single quotes and space,
//!         empty string otherwise
string Utility::IfNotEmpty_SingleQuoteAndSuffixWithSpace (string_view text)
{
  string result;

  if (!text.empty())
  {
    result.append("'").append(text.cbegin(), text.cend()).append("' ");
  }
  return result;
}
//
//  End of: Utility::IfNotEmpty_SingleQuoteAndSuffixWithSpace
//---------------------------------------------------------------------------



//! Reads a text file into a string
//!
//! @note There is no function to do such thing in the STL !
//!
//! @param filePath Path to file to read
//!
//! @return Read file content
string Utility::ReadTextFile (const string& filePath)
{
  CHECK_FILE_EXISTS(filePath);

  ifstream ifs(filePath);
  CHECK_TRUE(ifs.is_open(), "Failed to open file: "s + filePath);

  string content;

  ifs.seekg(0, std::ios::end);
  content.reserve(ifs.tellg());
  ifs.seekg(0, std::ios::beg);

  content.assign((std::istreambuf_iterator<char>(ifs)),
                  std::istreambuf_iterator<char>());
  return content;
}
//
//  End of: Utility::ReadTextFile
//---------------------------------------------------------------------------



//! Surrounds some text with single quotes
//!
//! @param text   Text to surround with quotes
//!
//! @return Text surrounded with single quotes
//!
string Utility::SingleQuote (string_view text)
{
  string result;
  result.append("'").append(text.cbegin(), text.cend()).append("'");
  return result;
}
//
//  End of: Utility::IfNotEmpty_SingleQuoteAndSuffixWithSpace
//---------------------------------------------------------------------------

//! Splits a string_view into bunch of string_view
//!
//! @note No trimming is done
//!
//! @param text         Text to split
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

      if (startPos < text.length())
      {
        views.emplace_back(text.substr(startPos));  // Last chunk
      }
    }
  }

  return views;
}
//
//  End of: Utility::Split
//---------------------------------------------------------------------------


//! Returns whether some text starts with some sub-string
//!
//! @note This is a case sensitive comparison
//!
//! @param text       Text to compare its ends to
//! @param substring  Sub string to compare at start of text
//!
//! @return true when substring is not empty and it is found at the very beginning of text, false otherwise
//!
bool Utility::StartsWith (string_view text, string_view substring)
{
  if (substring.empty())
  {
    return false;
  }

  if (substring.length() > text.length())
  {
    return false;
  }

  text.remove_suffix(text.length() - substring.length());
  return text == substring;
}
//
//  Start of: Utility::StartsWith
//---------------------------------------------------------------------------





//! Counts the number of tokens in a string_view
//!
//! @param text       Text for which to count tokens
//! @param separator  Token separator (may be several character width)
//!
//! @return Tokens count
size_t Utility::TokensCount (string_view text, string_view separator)
{
  if (text.empty())
  {
    return 0;
  }

  size_t count = 1u;  // There is at least one token if there is no separators
  auto   separatorLength = separator.length();
  if (separatorLength != 0)
  {
    string_view::size_type pos = 0;
    do
    {
      pos = text.find(separator, pos);
      if (pos == string_view::npos)
      {
        break;
      }

      pos += separatorLength;
      ++count;
    } while (true);
  }
  return count;
}
//
//  End of: Utility::TokensCount
//---------------------------------------------------------------------------

//! Converts string to amount of milliseconds
//!
//! @note - Support only 'ms' and 's' suffixes
//!       - Number must be expressed as a positive (or no sign) integer
//!       - Suffix can be separated from number by spaces
//!
//! @param durationStr  Duration expressed by a string
//!
std::chrono::milliseconds Utility::ToMilliseconds(string_view durationStr)
{
  TrimBoth(durationStr);

  CHECK_PARAMETER_NOT_EMPTY (durationStr,              "Cannot convert empty string to a duration");
  CHECK_PARAMETER_NEQ       (durationStr.front(), '-', "Duration can be expressed only with positive value");
  CHECK_PARAMETER_EQ        (durationStr.back(),  's', "Duration must me specified with 'ms' or 'suffix'");

  durationStr.remove_suffix(1u);   // Remove the 's' at the end

  // ---------------- Check it start with an integral number
  //
  if (durationStr.front() == '+')
  {
    durationStr.remove_prefix(1u);   // Remove the '+' at the begining
    TrimLeft(durationStr);
  }
  CHECK_PARAMETER_TRUE(std::isdigit(durationStr.front()), "Duration must be expressed with a positive integral number");

  unsigned long long factor = 1000u; // By default, tell it is specified using seconds

  if (durationStr.back() == 'm')
  {
    factor = 1u;   // This is in fact 'ms'
    durationStr.remove_suffix(1u);
  }
  TrimRight(durationStr);  // Support spaces between number and suffix

  CHECK_PARAMETER_TRUE(std::isdigit(durationStr.back()), "Duration must be expressed with only a positive integral number");

  unsigned long long duration = std::stoull(string(durationStr));

  return std::chrono::milliseconds(duration * factor);
}
//
//  End of Utility::ToMilliseconds
//---------------------------------------------------------------------------


//! Trims leading space characters (including \\t)
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


//! Trims trailing space characters (including \\t)
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
