//===========================================================================
//                           ParentNode_Traits.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ParentNode_Traits.hpp
//!
//! Declares traits for AppFunctionNameAndNode class
//!
//===========================================================================


#ifndef PARENTNODE_TRAITS_H__3DAE0D35_EB9B_41E4_7ABC_6D5F74B1D14D__INCLUDED_
  #define PARENTNODE_TRAITS_H__3DAE0D35_EB9B_41E4_7ABC_6D5F74B1D14D__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>

#include "ParentNode.hpp"

using mast::ParentNode;

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<ParentNode>
  {
    public:
    ValueTraits(const ParentNode& instance)
    {
      m_asString = ToStdString(instance);
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const ParentNode& instance)
    {
      std::ostringstream os;
      os << "ParentNode("
         << "Name:"      << instance.Name()                << ", "
         << "Id: "       << instance.Identifier()          << ", "
         << "Children: " << instance.DirectChildrenCount() << ")";

      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest

#endif  // not defined PARENTNODE_TRAITS_H__3DAE0D35_EB9B_41E4_7ABC_6D5F74B1D14D__INCLUDED_
//===========================================================================
// End of ParentNode_Traits.hpp
//===========================================================================



