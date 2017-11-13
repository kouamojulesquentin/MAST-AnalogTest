//===========================================================================
//                           CheckResult_Traits.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file CheckResult_Traits.hpp
//!
//! Declares CxxTest traits for class CheckResult
//!
//===========================================================================


#ifndef SYSTEMMODELCHECKRESULT_TRAITS_H__39DF941F_6202_4023_283_D67548BF3A0E__INCLUDED_
  #define SYSTEMMODELCHECKRESULT_TRAITS_H__39DF941F_6202_4023_283_D67548BF3A0E__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "CheckResult.hpp"

using mast::CheckResult;

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<CheckResult>
  {
    public:
    ValueTraits(const CheckResult& instance)
    {
      m_asString = ToStdString(instance);
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const CheckResult& instance)
    {
      using std::string;
      std::ostringstream os;
      os << string("CheckResult(") << std::endl << instance.MakeReport() << string(")");
      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest


#endif  // not defined SYSTEMMODELCHECKRESULT_TRAITS_H__39DF941F_6202_4023_283_D67548BF3A0E__INCLUDED_
//===========================================================================
// End of CheckResult_Traits.hpp
//===========================================================================
