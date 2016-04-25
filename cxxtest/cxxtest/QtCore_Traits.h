//===========================================================================
//                           QtCore_Traits.h
//===========================================================================
// Copyright © 2014 Jean-Francois Coulon. All rights reserved.
//
// Project : Multi-Edit extension DLL
//
//! @file QtCore_Traits.h
//!
//! Declares traits classes for Qt core classes
//!
// Some Tags are inserted for Doxygen (JavaDoc like) doc generator.
//===========================================================================
// Created by JFC on June/11/2014
//===========================================================================


#ifndef QTCORE_TRAITS_H__939B5C02_48A1_46F4_2DA7_E3FBA150A570__INCLUDED_
  #define QTCORE_TRAITS_H__939B5C02_48A1_46F4_2DA7_E3FBA150A570__INCLUDED_

#include <string>
#include <QString>
#include <cxxtest/ValueTraits.h>

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<QString>
  {
    public:
    ValueTraits(const QString& instance)
    {
      m_asString = ToStdString(instance);
    }
    const char *asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const QString& instance)
    {
      return std::string("\"") + instance.toStdString() + std::string("\"");
    }
    std::string m_asString;
  };
} // End of namespace CxxTest

#endif  // not defined QTCORE_TRAITS_H__939B5C02_48A1_46F4_2DA7_E3FBA150A570__INCLUDED_
//===========================================================================
// End of QtCore_Traits.h
//===========================================================================


