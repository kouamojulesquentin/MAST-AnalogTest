//===========================================================================
//                           VirtualRegister_Traits.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file VirtualRegister_Traits.hpp
//!
//! Declares CxxTest traits for RegisterSlice class
//!
//===========================================================================

#ifndef VIRTUALREGISTER_TRAITS_H__91501161_801_40D4_3982_1F72412CF346__INCLUDED_
  #define VIRTUALREGISTER_TRAITS_H__91501161_801_40D4_3982_1F72412CF346__INCLUDED_

#include "VirtualRegister.hpp"

#include "IndexedRange_Traits.hpp"
#include <cxxtest/StdValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>


namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<mast::VirtualRegister::RegisterSlice>
  {
    public:
    ~ValueTraits() = default;

    ValueTraits(const mast::VirtualRegister::RegisterSlice& instance)
      : m_asString(ToStdString(instance))
    {
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const mast::VirtualRegister::RegisterSlice& instance)
    {
      using std::string;
      std::ostringstream os;

      if (instance.reg)
      {
        os << "RegisterSlice(Register(\"" << instance.reg->Name() << "\"), ";
      }
      else
      {
        os << "RegisterSlice(Register(nullptr), ";
      }

      os << traits(instance.range).asString() << ")";

      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest


#endif  // not defined VIRTUALREGISTER_TRAITS_H__91501161_801_40D4_3982_1F72412CF346__INCLUDED_
//===========================================================================
// End of VirtualRegister_Traits.hpp
//===========================================================================



