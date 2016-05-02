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

using std::string;

using namespace mast;

//! Formats message for (std) exception
//!
//! @note It can also be used for non std::exception derived class
//! @note Format example:
//!   std::invalid_argument: Number of append bits cannot exceed number of bits of value.
//!   File: C:\Users\Jean-Francois\Documents\Tima\NewMast\Lib\src\BinaryVector.cpp
//!   Line: 87'
//!
string Utility::MakeExceptionMessage(const char*                    file,
                                     uint32_t                       line,
                                     std::experimental::string_view issueKind,
                                     std::experimental::string_view userMessage)
{
  std::ostringstream os;

  os << issueKind << ": " << userMessage << "." << std::endl;
  os << "File: "  << file << std::endl;
  os << "Line: "  << line;

  return os.str();
}



//===========================================================================
// End of Utility.cpp
//===========================================================================
