//===========================================================================
//                           Cpp_11_Traits.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Cpp_11_Traits.hpp
//!
//! Declares Cxxtest traits for C++11 struct/class/enum
//!
//===========================================================================


#ifndef CPP_11_TRAITS_H__1951B757_552B_4FDD_DB97_BDC09CBC67CD__INCLUDED_
  #define CPP_11_TRAITS_H__1951B757_552B_4FDD_DB97_BDC09CBC67CD__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <typeinfo>

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<std::type_info>
  {
    public:
    ValueTraits(const std::type_info& instance)
    {
      m_asString = ToStdString(instance);
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const std::type_info& instance)
    {
      using std::string;
      std::ostringstream os;
      os << string("type_info: \"") << instance.name() << "\"";
      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest

#endif  // not defined CPP_11_TRAITS_H__1951B757_552B_4FDD_DB97_BDC09CBC67CD__INCLUDED_
//===========================================================================
// End of Cpp_11_Traits.hpp
//===========================================================================



