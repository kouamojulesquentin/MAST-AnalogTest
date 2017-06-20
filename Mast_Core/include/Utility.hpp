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

#include "Mast_Core_export.hpp"
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <experimental/string_view>
#include <type_traits>    // For enum item manipulation

using namespace std::string_literals;

//+#define __func__ nullptr // For compiler that does not support __func__

namespace mast
{
//! Provides functions utilities that are not related to a specific class hierarchy
//!
class MAST_CORE_EXPORT Utility final
{
  // ---------------- Public  Methods
  //
  public:
  ~Utility() = delete;
  Utility()  = delete;

  //! Extracts file (or directory) parent directory path
  //!
  static std::string ExtractDirectoryPath (const std::string& filePath);

  //! Clears file content if the file exists
  //!
  static void ClearFile (std::experimental::string_view filePath);

  //! Tests if a file exists
  //!
  static bool FileExists (std::experimental::string_view filePath);

  //! Reads a text file into a string
  //!
  static std::string ReadTextFile (const std::string& filePath);

  //! Returns the minimal number of bits to encode a binary value
  //!
  static uint32_t MinimalBitsForValue(uint32_t value);

  //! Computes the minimal number of bytes to hold a number of bits
  //!
  static uint32_t BytesCountFromBitsCount(uint32_t bitsCount) { return (bitsCount + 7) / 8; }

  //! Returns whether some text ends with some sub-string
  //!
  static bool EndsWith (std::experimental::string_view text, std::experimental::string_view substring);

  //! Returns whether some text start with some sub-string
  //!
  static bool StartsWith (std::experimental::string_view text, std::experimental::string_view substring);

  //! Trims both leading and trailing space characters (including \\t)
  //!
  static void TrimBoth(std::experimental::string_view& text) { TrimLeft(text); TrimRight(text); }

  //! Trims leading space characters (including \\t)
  //!
  static void TrimLeft(std::experimental::string_view& text);

  //! Trims trailing space characters (including \\t)
  //!
  static void TrimRight(std::experimental::string_view& text);

  //! Surrounds some not empty text with single quotes and a space after the closing quote
  //!
  static std::string IfNotEmpty_SingleQuoteAndSuffixWithSpace (std::experimental::string_view text);

  //! Surrounds some text with single quotes
  //!
  static std::string SingleQuote (std::experimental::string_view text);

  //! Splits a string_view into bunch of string_view
  //!
  static std::vector<std::experimental::string_view> Split(std::experimental::string_view text, std::experimental::string_view separator);

  //! Returns last token from a string_view
  //!
  static std::experimental::string_view BackToken(std::experimental::string_view text, std::experimental::string_view separator);

  //! Returns all tokens from a string_view except last one
  //!
  static std::experimental::string_view FrontTokens(std::experimental::string_view text, std::experimental::string_view separator);

  //! Counts the number of tokens in a string_view
  //!
  static size_t TokensCount (std::experimental::string_view text, std::experimental::string_view separator);


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

//! Concatenates a string and a string_view
//!
//! @return A string with the concatenation
inline std::string operator+(const std::string& lhs, std::experimental::string_view rhs) { return std::string (lhs).append(rhs.cbegin(), rhs.cend()); }

//! Appends a string_view to a string
//!
//! @return Left hand side string appended with right hand side string_view
inline std::string& operator+=(std::string& lhs, std::experimental::string_view rhs) { return lhs.append(rhs.cbegin(), rhs.cend()); }

//! Concatenates a string_view and a string
//!
//! @return A string with the concatenation
inline std::string operator+(std::experimental::string_view lhs, const std::string& rhs) { return std::string(lhs.cbegin(), lhs.cend()).append(rhs); }

//! Combines enum flags (bitwise or)
//!
template<typename ENUM_T>
constexpr ENUM_T operator | (ENUM_T X, ENUM_T Y)
{
  return static_cast<ENUM_T>(  static_cast<std::underlying_type_t<ENUM_T>>(X)
                             | static_cast<std::underlying_type_t<ENUM_T>>(Y)
                            );
}


//! Combines enum flags type enum items in place (bitwise or)
//!
template<typename ENUM_T>
ENUM_T& operator |= (ENUM_T& X, ENUM_T Y)
{
  auto result = static_cast<ENUM_T>(  static_cast<std::underlying_type_t<ENUM_T>>(X)
                                    | static_cast<std::underlying_type_t<ENUM_T>>(Y)
                                   );
  X = result;
  return X;
}

//! Forces an enum value (casting away enum type)
//!
template<typename ENUM_T>
ENUM_T& ForceValue(ENUM_T& X, int value)
{
  X = static_cast<ENUM_T>(static_cast<std::underlying_type_t<ENUM_T>>(value));
  return X;
}


//! Tests of flag type enum item
//!
template<typename ENUM_T>
constexpr bool IsSet(ENUM_T value, ENUM_T flags)
{
  return (    static_cast<std::underlying_type_t<ENUM_T>>(value)
            & static_cast<std::underlying_type_t<ENUM_T>>(flags)
         ) == static_cast<std::underlying_type_t<ENUM_T>>(flags);
}


//! Helper class to set a value for current scope and restore it at scope exit
//!
template <typename T>
struct SetForScope
{
  SetForScope() = delete;
  SetForScope(const SetForScope&) = default;
  SetForScope& operator=(const SetForScope&) = default;

  SetForScope (T& v, T nv)
    : vr(v), ov(v)
  {
    vr = nv;
  }

  ~SetForScope ()
  {
    vr = ov;
  }
  T& vr;
  T  ov;
};

//! Helper function to create a SetForScope instance
//!
template <typename T>
SetForScope<T> MakeSetForScope (T& v, T nv)
{
  return SetForScope<T>(v, nv);
}
#define SET_FOR_SCOPE(var, varValue) \
    auto STRING_JOIN2(set_in_scope, __LINE__) = MakeSetForScope(var, varValue)



// ---------------- Define behaviour (functor call) at end of scope
//
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
#define THROW_IMPL_(file, fct, line, exc, msg) throw exc(mast::Utility::MakeExceptionMessage(file, fct, line, #exc, msg))
#define THROW_IMPL(exc,msg)                    THROW_IMPL_(__FILE__, __func__, __LINE__, exc, msg)

#define THROW_INVALID_ARGUMENT(msg) THROW_IMPL(std::invalid_argument, msg)
#define THROW_LOGIC_ERROR(msg)      THROW_IMPL(std::logic_error,      msg)
#define THROW_OUT_OF_RANGE(msg)     THROW_IMPL(std::out_of_range,     msg)
#define THROW_RUNTIME_ERROR(msg)    THROW_IMPL(std::runtime_error,    msg)

#define RETHROW_WITH_NESTED_IMPL(file, fct, line, action, exc_t, msg) \
try             \
{               \
  action;       \
}               \
catch(exc_t&)   \
{               \
  std::throw_with_nested(exc_t(mast::Utility::MakeExceptionMessage(file, fct, line, #exc_t, msg))); \
}

#define RETHROW_WITH_NESTED(action, exc_t, msg) RETHROW_WITH_NESTED_IMPL(__FILE__, __func__, __LINE__, action, exc_t, msg)

#define RETHROW_INVALID_ARGUMENT(action, msg) RETHROW_WITH_NESTED(action, std::invalid_argument, msg)
#define RETHROW_LOGIC_ERROR(action, msg)      RETHROW_WITH_NESTED(action, std::logic_error,      msg)
#define RETHROW_OUT_OF_RANGE(action, msg)     RETHROW_WITH_NESTED(action, std::out_of_range,     msg)
#define RETHROW_RUNTIME_ERROR(action, msg)    RETHROW_WITH_NESTED(action, std::runtime_error,    msg)

//! Checks that a pointer (parameter) IS, semantically, nullptr, otherwise it throws an exception
//!
//! @note It is mainly intended to report that some parameter is not supported (in overriding context)
//!
//! @return given parameter if "nullptr"
template<typename T>
T CheckParameterIsNullptr(const char* file, const char* function, int line, T ptr, std::experimental::string_view  msg)
{
  if (ptr)
  {
    throw std::invalid_argument(mast::Utility::MakeExceptionMessage(file, function, line, "std::invalid_argument", msg));
  }
  return ptr;
}


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

//! Checks that a pointer IS, semantically, nullptr, otherwise it throws an exception
//!
//! @return nullptr
template<typename T>
T CheckValueIsNullptr(const char* file, const char* function, int line, T ptr, std::experimental::string_view  msg)
{
  if (ptr != nullptr)
  {
    throw std::runtime_error(mast::Utility::MakeExceptionMessage(file, function, line, "std::runtime_error", msg));
  }
  return ptr;
}

//! Checks that a pointer value is not nullptr, otherwise it throws an exception
//!
//! @return given pointer if not nullptr
template<typename T>
T CheckValueIsNotNullptr(const char* file, const char* function, int line, T ptr, std::experimental::string_view  msg)
{
  if (ptr == nullptr)
  {
    throw std::runtime_error(mast::Utility::MakeExceptionMessage(file, function, line, "std::runtime_error", msg));
  }
  return ptr;
}

//! Checks that a value is != 0, otherwise it throws an exception
//!
//! @return given value if not zero
template<typename T>
T CheckValueIsNotZero(const char* file, const char* function, int line, T value, std::experimental::string_view  msg)
{
  if (value == 0)
  {
    throw std::runtime_error(mast::Utility::MakeExceptionMessage(file, function, line, "std::runtime_error", msg));
  }
  return value;
}

//! Checks that a value (not a parameter) meet a condition, otherwise it throws an exception
//!
//! @return given value if not zero
template<typename T>
T CheckValueCondition(const char* file, const char* function, int line, T value, bool conditionMet, std::experimental::string_view  msg)
{
  if (!conditionMet)
  {
    throw std::runtime_error(mast::Utility::MakeExceptionMessage(file, function, line, "std::runtime_error", msg));
  }
  return value;
}

template<typename T, typename U, typename V>
bool InRange(const T& val, const U& minVal, const V& maxVal) { return (val >= minVal) && (val <= maxVal); }

#define CHECK_PARAMETER_NULL(ptr,     msg)    CheckParameterIsNullptr    (__FILE__, __func__, __LINE__, ptr, msg)
#define CHECK_PARAMETER_NOT_NULL(ptr, msg)    CheckParameterIsNotNullptr (__FILE__, __func__, __LINE__, ptr, msg)
#define CHECK_PARAMETER_NOT_ZERO(val, msg)    CheckParameterIsNotZero    (__FILE__, __func__, __LINE__, val, msg)

#define CHECK_PARAMETER_NOT_EMPTY(val, msg)   CheckParameterCondition  (__FILE__, __func__, __LINE__, (val), !(val).empty(), msg)

#define CHECK_PARAMETER_EQ(val,  refVal, msg) CheckParameterCondition (__FILE__, __func__, __LINE__, (val), (val == refVal), msg)
#define CHECK_PARAMETER_NEQ(val, refVal, msg) CheckParameterCondition (__FILE__, __func__, __LINE__, (val), (val != refVal), msg)
#define CHECK_PARAMETER_GT(val,  minVal, msg) CheckParameterCondition (__FILE__, __func__, __LINE__, (val), (val >  minVal), msg)
#define CHECK_PARAMETER_GTE(val, minVal, msg) CheckParameterCondition (__FILE__, __func__, __LINE__, (val), (val >= minVal), msg)
#define CHECK_PARAMETER_LT(val,  maxVal, msg) CheckParameterCondition (__FILE__, __func__, __LINE__, (val), (val <  maxVal), msg)
#define CHECK_PARAMETER_LTE(val, maxVal, msg) CheckParameterCondition (__FILE__, __func__, __LINE__, (val), (val <= maxVal), msg)
#define CHECK_PARAMETER_RANGE(val, minVal, maxVal, msg) CheckParameterCondition (__FILE__, __func__, __LINE__, (val), InRange((val), (minVal), (maxVal)), msg)

#define CHECK_VALUE_NULL(ptr,     msg) CheckValueIsNullptr    (__FILE__, __func__, __LINE__, ptr, msg)
#define CHECK_VALUE_NOT_NULL(ptr, msg) CheckValueIsNotNullptr (__FILE__, __func__, __LINE__, ptr, msg)
#define CHECK_VALUE_NOT_ZERO(val, msg) CheckValueIsNotZero    (__FILE__, __func__, __LINE__, val, msg)

#define CHECK_VALUE_NOT_EMPTY(val, msg)   CheckValueCondition (__FILE__, __func__, __LINE__, (val), !(val).empty(),  msg)
#define CHECK_VALUE_EQ(val,  refVal, msg) CheckValueCondition (__FILE__, __func__, __LINE__, (val), (val == refVal), msg)
#define CHECK_VALUE_NEQ(val, refVal, msg) CheckValueCondition (__FILE__, __func__, __LINE__, (val), (val != refVal), msg)
#define CHECK_VALUE_GT(val,  minVal, msg) CheckValueCondition (__FILE__, __func__, __LINE__, (val), (val >  minVal), msg)
#define CHECK_VALUE_GTE(val, minVal, msg) CheckValueCondition (__FILE__, __func__, __LINE__, (val), (val >= minVal), msg)
#define CHECK_VALUE_LT(val,  maxVal, msg) CheckValueCondition (__FILE__, __func__, __LINE__, (val), (val <  maxVal), msg)
#define CHECK_VALUE_LTE(val, maxVal, msg) CheckValueCondition (__FILE__, __func__, __LINE__, (val), (val <= maxVal), msg)
#define CHECK_VALUE_RANGE(val, minVal, maxVal, msg) CheckValueCondition (__FILE__, __func__, __LINE__, (val), InRange((val), (minVal), (maxVal)), msg)

#define CHECK_TRUE(expr, msg) if (!(expr))  throw std::runtime_error(mast::Utility::MakeExceptionMessage(__FILE__, __func__, __LINE__, "std::runtime_error", msg))
#define CHECK_FAILED(msg)                   throw std::runtime_error(mast::Utility::MakeExceptionMessage(__FILE__, __func__, __LINE__, "std::runtime_error", msg))

#define CHECK_FILE_EXISTS(filePath)         CHECK_TRUE(mast::Utility::FileExists(filePath), std::string("File: '") + filePath + "' does not exist (or cannot be opened)")
#define CHECK_FILE_EXISTS_EX(filePath, msg) CHECK_TRUE(mast::Utility::FileExists(filePath), std::string(msg)       + filePath + "' does not exist (or cannot be opened)")

#endif  // not defined UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_

//===========================================================================
// End of Utility.hpp
//===========================================================================
