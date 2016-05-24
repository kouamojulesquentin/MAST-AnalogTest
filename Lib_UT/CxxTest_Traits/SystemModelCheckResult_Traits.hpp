//===========================================================================
//                           SystemModelCheckResult_Traits.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckResult_Traits.hpp
//!
//! Declares CxxTest traits for class SystemModelCheckResult
//!
//===========================================================================


#ifndef SYSTEMMODELCHECKRESULT_TRAITS_H__39DF941F_6202_4023_283_D67548BF3A0E__INCLUDED_
  #define SYSTEMMODELCHECKRESULT_TRAITS_H__39DF941F_6202_4023_283_D67548BF3A0E__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "SystemModelCheckResult.hpp"

using mast::SystemModelCheckResult;

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<SystemModelCheckResult>
  {
    public:
    ValueTraits(const SystemModelCheckResult& instance)
    {
      m_asString = ToStdString(instance);
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const SystemModelCheckResult& instance)
    {
      using std::string;
      std::ostringstream os;
      os << string("SystemModelCheckResult(") << std::endl << instance.MakeReport() << string(")");
      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest


#endif  // not defined SYSTEMMODELCHECKRESULT_TRAITS_H__39DF941F_6202_4023_283_D67548BF3A0E__INCLUDED_
//===========================================================================
// End of SystemModelCheckResult_Traits.hpp
//===========================================================================
