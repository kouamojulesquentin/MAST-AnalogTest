//===========================================================================
//                           AppFunctionNameAndNode_Traits.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AppFunctionNameAndNode_Traits.hpp
//!
//! Declares traits for AppFunctionNameAndNode class
//!
//===========================================================================


#ifndef APPFUNCTIONNAMEANDNODE_TRAITS_H__E7ACA99_B07E_4880_8596_8D278A19B3AC__INCLUDED_
  #define APPFUNCTIONNAMEANDNODE_TRAITS_H__E7ACA99_B07E_4880_8596_8D278A19B3AC__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>

#include "AppFunctionNameAndNode.hpp"
#include "ParentNode_Traits.hpp"

using mast::AppFunctionNameAndNode;

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<AppFunctionNameAndNode>
  {
    public:
    ValueTraits(const AppFunctionNameAndNode& instance)
    {
      m_asString = ToStdString(instance);
    }
    const char* asString() const { return m_asString.c_str(); }

    private:
    static std::string ToStdString(const AppFunctionNameAndNode& instance)
    {
      using std::string;
      std::ostringstream os;
      os << "AppFunctionNameAndNode(" << instance.appName << ", ";

      if (instance.node)
      {
        const auto node = instance.node.get();
        os << traits(*node).asString();
      }
      else
      {
        os << "nullptr";
      }
      os << ", " << instance.line << ")";
      return os.str();
    }
    std::string m_asString;
  };
} // End of namespace CxxTest

#endif  // not defined APPFUNCTIONNAMEANDNODE_TRAITS_H__E7ACA99_B07E_4880_8596_8D278A19B3AC__INCLUDED_
//===========================================================================
// End of AppFunctionNameAndNode_Traits.hpp
//===========================================================================



