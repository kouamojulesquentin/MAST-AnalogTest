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

  //! Returns the minimal number of bits to encode a binary value
  //!
  static uint32_t MinimalBitsForValue(uint32_t value);

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

template <typename F>
struct ScopeExit
{
  ScopeExit (F f)
    : f(f)
  {
  }

  ~ScopeExit ()
  {
    f ();
  }
  F f;
};

template <typename F>
ScopeExit<F> MakeScopeExit (F f)
{
  return ScopeExit<F>(f);
}

} // End of namespace mast

#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)
#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define SCOPE_EXIT(code) \
    auto STRING_JOIN2(scope_exit_, __LINE__) = MakeScopeExit([&](){code;})

#define AT_SCOPE_EXIT(code) \
    auto STRING_JOIN2(scope_exit_, __LINE__) = MakeScopeExit(code)

//! Helper to format message and throw exception
//!
#define THROW_IMPL(exc,msg)         throw exc(mast::Utility::MakeExceptionMessage(__FILE__, __LINE__, #exc, msg))

#define THROW_INVALID_ARGUMENT(msg) THROW_IMPL(std::invalid_argument, msg)
#define THROW_LOGIC_ERROR(msg)      THROW_IMPL(std::logic_error,      msg)
#define THROW_OUT_OF_RANGE(msg)     THROW_IMPL(std::out_of_range,     msg)


#endif  // not defined UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_

//===========================================================================
// End of Utility.hpp
//===========================================================================
