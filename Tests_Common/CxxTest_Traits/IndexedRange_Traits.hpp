//===========================================================================
//                           IndexedRange_Traits.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file IndexedRange_Traits.hpp
//!
//! Declares CxxTest traits for IndexedRange class
//!
//===========================================================================

#ifndef INDEXEDRANGE_TRAITS_H__88800AF7_AF0A_43D2_7DA6_604A613849B6__INCLUDED_
  #define INDEXEDRANGE_TRAITS_H__88800AF7_AF0A_43D2_7DA6_604A613849B6__INCLUDED_

#include "IndexedRange.hpp"

#include <cxxtest/StdValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>


namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<mast::IndexedRange>
  {
    public:
    ~ValueTraits() = default;

    ValueTraits(const mast::IndexedRange& instance)
      : m_asString(ToStdString(instance))
    {
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const mast::IndexedRange& instance)
    {
      using std::string;
      std::ostringstream os;

      os << "IndexedRange(" << instance.left << "," << instance.right << ")";

      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest


#endif  // not defined INDEXEDRANGE_TRAITS_H__88800AF7_AF0A_43D2_7DA6_604A613849B6__INCLUDED_
//===========================================================================
// End of IndexedRange_Traits.hpp
//===========================================================================



