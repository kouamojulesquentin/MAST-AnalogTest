//===========================================================================
//                           ApplicationDescriptor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ApplicationDescriptor.hpp
//!
//! Declares ApplicationDescriptor class
//!
//===========================================================================


#ifndef APPLICATIONDESCRIPTOR_H__D65D5AE3_DB9E_4C3B_C994_182499CDDAFE__INCLUDED_
  #define APPLICATIONDESCRIPTOR_H__D65D5AE3_DB9E_4C3B_C994_182499CDDAFE__INCLUDED_

#include <functional>
#include <string>

namespace test
{
//! Associates functions (applications) with some node path and optionally a (debug) name
//!
struct ApplicationDescriptor final
{
  using Application_t = std::function<void()>;

  Application_t function;
  std::string   topNodePath;
  std::string   debugName;

  ApplicationDescriptor(Application_t p_function, std::string p_topNodePath, std::string p_debugName = "")
    : function    (p_function)
    , topNodePath (std::move(p_topNodePath))
    , debugName   (std::move(p_debugName))
  {}
};
//
//  End of ApplicationDescriptor class declaration
//---------------------------------------------------------------------------
} // End of namespace test




#endif  // not defined APPLICATIONDESCRIPTOR_H__D65D5AE3_DB9E_4C3B_C994_182499CDDAFE__INCLUDED_

//===========================================================================
// End of ApplicationDescriptor.hpp
//===========================================================================
