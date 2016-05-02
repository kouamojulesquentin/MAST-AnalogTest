//===========================================================================
//                           BinaryVector_Traits.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BinaryVector_Traits.hpp
//!
//! Declares CxxTest traits for class mast::BinaryVector
//!
//===========================================================================

#ifndef BINARYVECTOR_TRAITS_H__B089B88E_3F1_45CA_6298_5B65FD15418__INCLUDED_
  #define BINARYVECTOR_TRAITS_H__B089B88E_3F1_45CA_6298_5B65FD15418__INCLUDED_

#include <cxxtest/StdValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "BinaryVector.hpp"

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<mast::BinaryVector>
  {
    public:
    ValueTraits(const mast::BinaryVector& instance)
    {
      m_asString = ToStdString(instance);
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const mast::BinaryVector& instance)
    {
      using std::string;
      std::ostringstream os;

      auto bitsCount  = instance.BitsCount();
      auto bytesCount = instance.BytesCount();

      if (bitsCount > 16)
      {
        os << std::endl;
      }
      os << string("BinaryVector(");
      os << "b:" << bitsCount << "/B:" << bytesCount << "/";
      if (bitsCount > 80)
      {
        os << std::endl;
      }
      os << instance.DataAsBinaryString(":", "_", 80, ",");
      os << string(")");
      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest


#endif  // not defined BINARYVECTOR_TRAITS_H__B089B88E_3F1_45CA_6298_5B65FD15418__INCLUDED_
//===========================================================================
// End of BinaryVector_Traits.hpp
//===========================================================================
