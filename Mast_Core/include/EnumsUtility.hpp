//===========================================================================
//                           EnumsUtility.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file EnumsUtility.hpp
//!
//! Defines utilities related to enum usage
//!
//===========================================================================

#ifndef ENUMSUTILITIES_H__13235EC3_2471_48EF_EDA3_FF3B79E0AFCE__INCLUDED_
  #define ENUMSUTILITIES_H__13235EC3_2471_48EF_EDA3_FF3B79E0AFCE__INCLUDED_

#include <string>
#include <experimental/string_view>
#include <sstream>
#include <type_traits>

namespace mast
{

//! Utility helpers related to enums
//!
//!
struct EnumsUtility final
{
  //! Returns scope part of a scoped name (with the final '::')
  //!
  //! @note Cannot be constexpr yet because currently using Gcc 4.9.3 that does not declared rfind as constexpr
  //!
  static inline std::experimental::string_view ExtractedScope(std::experimental::string_view scopedName)
  {
    auto endOfScopePos = scopedName.rfind("::");
    if (endOfScopePos == std::experimental::string_view::npos)
    {
      return std::experimental::string_view();
    }
    return scopedName.substr(0, endOfScopePos + 2u);
  }
  //
  //  End of ExtractedScope
  //---------------------------------------------------------------------------


  //! Returns name part of a scoped name
  //!
  //! @note Cannot be constexpr yet because currently using Gcc 4.9.3 that does not declared rfind as constexpr
  //!
  static inline std::experimental::string_view RemovedScope(std::experimental::string_view scopedName)
  {
    auto endOfScopePos = scopedName.rfind("::");
    if (endOfScopePos == std::experimental::string_view::npos)
    {
      return scopedName;
    }
    return scopedName.substr(endOfScopePos + 2u);
  }
};

//! Combines enum flags (bitwise or)
//!
template<typename ENUM_T>
constexpr ENUM_T operator | (ENUM_T X, ENUM_T Y)
{
  return static_cast<ENUM_T>(  static_cast<std::underlying_type_t<ENUM_T>>(X)
                             | static_cast<std::underlying_type_t<ENUM_T>>(Y)
                            );
}

//! Returns ored enum flags
//!
//+template<typename T>
//+inline constexpr T operator|(T lhs, T rhs)
//+{
//+  return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) | static_cast<std::underlying_type_t<T>>(rhs));
//+}


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

} // End of namespace mast

// Helpers to convert some enum to strings
//
// It Supports C++ scoped enums

//! Default template that cannot be instantiated
//!
template<typename TYPE> class EnumTraits
{
  ~EnumTraits() = delete;
  EnumTraits()  = delete;
};


//! Macro to specify members of the enum
//!
#define ENUM_MEMBER(MEMBER) case MEMBER: \
        _scopeString = mast::EnumsUtility::ExtractedScope(#MEMBER); \
        _nameString  = mast::EnumsUtility::RemovedScope(#MEMBER);   \
        break;

//! Specialization for specified enum type
//!
//! @note it support enum values specified by VALUES (using ENUM_MEMBER macro)
//! Example:
//! Must be defined in the GLOBAL namespace
//! =======================================
//!
//! @code {.cpp}
//!     ENUM_TRAITS(Kind,
//!                 ENUM_MEMBER(Kind::Undefined)
//!                 ENUM_MEMBER(Kind::Alias)
//!                 ENUM_MEMBER(Kind::Module)
//!                 ...
//!                 ENUM_MEMBER(Kind::Attribute)
//!                 ENUM_MEMBER(Kind::Parameter)
//!                 );
//! @endcode
//!
#define ENUM_TRAITS(TYPE, VALUES) \
template<>       \
class EnumTraits<TYPE> \
{ \
    std::experimental::string_view _scopeString; \
    std::experimental::string_view _nameString; \
    bool                           _isUnknown = false; \
  public: \
    EnumTraits(TYPE value) \
    { \
      switch (value) \
      { \
          VALUES \
          default: \
            _isUnknown = true; \
            _scopeString = #TYPE "::"; \
            _nameString  = #TYPE "::Unknown_enum_value"; \
      } \
    } \
    bool IsUnknown() const { return _isUnknown; } \
    std::experimental::string_view Scope(void) const \
    { \
      return _scopeString;   \
    } \
    std::experimental::string_view Name(void) const \
    { \
      return _nameString;   \
    } \
}; \
   \
inline std::experimental::string_view ScopeString(TYPE value) \
{                                                             \
  return EnumTraits<TYPE>(value).Scope();                     \
}                                                             \
inline std::experimental::string_view NameString(TYPE value)  \
{                                                             \
  return EnumTraits<TYPE>(value).Name();                      \
}                                                             \
inline std::string ScopedNameString(TYPE value)               \
{                                                             \
  auto name = EnumTraits<TYPE>(value).Name();                 \
  return std::string(EnumTraits<TYPE>(value).Scope())         \
             .append(name.cbegin(), name.cend());             \
}                                                             \
   \
inline std::string ToStdString(TYPE value)                                        \
{                                                                                 \
  auto trait = EnumTraits<TYPE>(value);                                           \
  if (trait.IsUnknown())                                                          \
  {                                                                               \
    std::ostringstream os;                                                        \
    os << #TYPE "(" << static_cast<std::underlying_type_t<TYPE>>(value) << ")";   \
    return os.str();                                                              \
  }                                                                               \
  return std::string(trait.Name());                                               \
}                                                                                 \
template<typename _CharT, typename _Traits>                                                                 \
inline std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>& __os, TYPE __x) \
{ return (__os << ToStdString(__x)); }



#endif  // not defined ENUMSUTILITIES_H__13235EC3_2471_48EF_EDA3_FF3B79E0AFCE__INCLUDED_

//===========================================================================
// End of EnumsUtility.h
//===========================================================================

