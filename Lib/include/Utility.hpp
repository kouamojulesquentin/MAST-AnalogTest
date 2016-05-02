//===========================================================================
//                           Utility.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Utility.hpp
//!
//! Declares class Utility
//!
//===========================================================================

#ifndef UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_
  #define UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_

#include <stdexcept>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Provides functions utilities that are not related to a specific class hierarchy
//!
class Utility final
{
  // ---------------- Public  Methods
  //
  public:
  ~Utility() = delete;
  Utility()  = delete;

  //! Computes the minimal number of bytes to hold a number of bits
  //!
  static uint32_t BytesCountFromBitsCount(uint32_t bitsCount) { return (bitsCount + 7) / 8; }

  //! Formats message for (std) exception
  //!
  static std::string MakeExceptionMessage(const char*                    file,
                                          uint32_t                       line,
                                          std::experimental::string_view issueKind,
                                          std::experimental::string_view userMessage);
};
//
//  End of Utility class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


//! Helper to format message and throw exception
//!
#define THROW_IMPL(exc,msg)         throw exc(mast::Utility::MakeExceptionMessage(__FILE__, __LINE__, #exc, msg))

#define THROW_INVALID_ARGUMENT(msg) THROW_IMPL(std::invalid_argument, msg)
#define THROW_OUT_OF_RANGE(msg)     THROW_IMPL(std::out_of_range,     msg)


#endif  // not defined UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_

//===========================================================================
// End of Utility.hpp
//===========================================================================
