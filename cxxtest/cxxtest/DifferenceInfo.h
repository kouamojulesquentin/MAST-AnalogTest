//===========================================================================
//                           DifferenceInfo.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file DifferenceInfo.h
//!
//! Declares class/struct to holds difference info needed to report some difference
//!
//===========================================================================
// Modifications history:
// Created by JFC on October/02/2015
//===========================================================================


#ifndef DIFFERENCEINFO_H__82343725_E1FC_4E0A_A5AE_8E13C49421F7__INCLUDED_
  #define DIFFERENCEINFO_H__82343725_E1FC_4E0A_A5AE_8E13C49421F7__INCLUDED_

#if defined(_CXXTEST_HAVE_STD)
#include <string>

namespace CxxTest
{
//! Summarized difference between two "entities"
//!
struct DifferenceInfo
{
  enum  DifferenceReason
  {
    ARE_EQUAL,            //!< Both entities compare equal
    LEFT_NOT_EXIST,       //!< Left  entity does not exist (usually a file), so no comparison is possible
    RIGHT_NOT_EXIST,      //!< Right entity does not exist (usually a file), so no comparison is possible
    LEFT_SHORTER,         //!< Left  entity is shorter than right entity
    LEFT_LARGER,          //!< Right entity is shorter than right entity
    DIFFER_AT_LINE        //!< Entities differs because at least one line differs
  };

  DifferenceInfo() : reason(DifferenceInfo::ARE_EQUAL), leftLine(0), rightLine(0) {}

  static std::string lineStr(int linesCount)
  {
    if (linesCount == 0)
    {
      return "empty";
    }
    else if (linesCount == 1)
    {
      return "1 line";
    }

    std::ostringstream os;
    os << linesCount << " lines";
    return os.str().c_str();
  }

  DifferenceReason reason;
  int              leftLine;              //!< Left entity line count OR first line in both entities that differ (1 based)
  int              rightLine;             //!< Right entity line count OR first line in both entities that differ (1 based)
  std::string      leftDifferingLine;     //!< Content of the first differing line in left  entity
  std::string      rightDifferingLine;    //!< Content of the first differing line in right entity
};


bool fileExists             (const char* filePath);
int  fileSize               (const char* filePath);
bool fileHasExpectedContent (const char* filePath, const std::string& expectedContent, DifferenceInfo& diff);

} // End of namespace CxxTest
#endif // defined _CXXTEST_HAVE_STD

#endif  // not defined DIFFERENCEINFO_H__82343725_E1FC_4E0A_A5AE_8E13C49421F7__INCLUDED_
//===========================================================================
// End of DifferenceInfo.h
//===========================================================================


