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

#include "Platform.hpp"
#include <stdexcept>
#include <string>
#include <experimental/string_view>

//+#define __func__ nullptr // For compiler that does not support __func__

namespace mast
{
//! Provides functions utilities that are not related to a specific class hierarchy
//!
class DLL_EXPORT Utility final
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

  //! Trims leading space characters (including \t)
  //!
  static void TrimLeft(std::experimental::string_view& text);

  //! Formats message for (std) exception
  //!
  static std::string MakeExceptionMessage(const char*                    file,
                                          const char*                    function,
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
#define THROW_IMPL(exc,msg)         throw exc(mast::Utility::MakeExceptionMessage(__FILE__, __func__, __LINE__, #exc, msg))

#define THROW_RUNTIME_ERROR(msg)    THROW_IMPL(std::runtime_error,    msg)
#define THROW_INVALID_ARGUMENT(msg) THROW_IMPL(std::invalid_argument, msg)
#define THROW_LOGIC_ERROR(msg)      THROW_IMPL(std::logic_error,      msg)
#define THROW_OUT_OF_RANGE(msg)     THROW_IMPL(std::out_of_range,     msg)

//! Checks that a pointer (parameter) is not nullptr, otherwise it throws an exception
//!
//! @return given parameter if not nullptr
template<typename T>
T CheckParameterIsNotNullptr(const char* file, const char* function, int line, T ptr, std::experimental::string_view  msg)
{
  if (!ptr)
  {
    throw std::invalid_argument(mast::Utility::MakeExceptionMessage(file, function, line, "std::invalid_argument", msg));
  }
  return ptr;
}

//! Checks that a parameter value is != 0, otherwise it throws an exception
//!
//! @return given parameter if not zero
template<typename T>
T CheckParameterIsNotZero(const char* file, const char* function, int line, T value, std::experimental::string_view  msg)
{
  if (value == 0)
  {
    throw std::invalid_argument(mast::Utility::MakeExceptionMessage(file, function, line, "std::invalid_argument", msg));
  }
  return value;
}

//! Checks that a value (parameter) meet a condition, otherwise it throws an exception
//!
//! @return given parameter if not zero
template<typename T>
T CheckParameterCondition(const char* file, const char* function, int line, T value, bool conditionMet, std::experimental::string_view  msg)
{
  if (!conditionMet)
  {
    throw std::invalid_argument(mast::Utility::MakeExceptionMessage(file, function, line, "std::invalid_argument", msg));
  }
  return value;
}

//! Checks that a parameter value is != 0, otherwise it throws an exception
//!
//! @return given parameter if not zero
template<typename T>
T CheckValueIsNotNullptr(const char* file, const char* function, int line, T value, std::experimental::string_view  msg)
{
  if (value == 0)
  {
    throw std::logic_error(mast::Utility::MakeExceptionMessage(file, function, line, "std::logic_error", msg));
  }
  return value;
}


#define CHECK_PARAMETER_NOT_NULL(ptr, msg) CheckParameterIsNotNullptr (__FILE__, __func__, __LINE__, ptr, msg)
#define CHECK_PARAMETER_NOT_ZERO(val, msg) CheckParameterIsNotZero    (__FILE__, __func__, __LINE__, val, msg)
#define CHECK_PARAMETER_GT(val, minVal, msg) CheckParameterCondition  (__FILE__, __func__, __LINE__, (val),(val > minVal), msg)

#define CHECK_VALUE_NOT_NULL(ptr, msg) CheckValueIsNotNullptr (__FILE__, __func__, __LINE__, ptr, msg)

#endif  // not defined UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_

//===========================================================================
// End of Utility.hpp
//===========================================================================
