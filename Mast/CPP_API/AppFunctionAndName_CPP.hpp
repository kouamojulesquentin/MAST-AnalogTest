//===========================================================================
//                           AppFunctionAndName_CPP.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AppFunctionAndName_CPP.hpp
//!
//! Declares AppFunctionAndName class
//!
//===========================================================================


#ifndef APPFUNCTIONANDNAME_H__B015061F_458B_44A7_F2AC_CB095D17494B__INCLUDED_
  #define APPFUNCTIONANDNAME_H__B015061F_458B_44A7_F2AC_CB095D17494B__INCLUDED_

#include <string>
#include <functional>

namespace mast
{
  //! Associates application function (an algorithm) with a name to identify it
  //!
  struct AppFunctionAndName final
  {
    using Application_t = std::function<void()>;

    Application_t appFunction;
    std::string   appName;

    AppFunctionAndName(Application_t p_appFunction, std::string p_appName)
      : appFunction (p_appFunction)
      , appName     (std::move(p_appName))
    {}
  };
  //
  //  End of AppFunctionAndName struct declaration
  //---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined APPFUNCTIONANDNAME_H__B015061F_458B_44A7_F2AC_CB095D17494B__INCLUDED_

//===========================================================================
// End of AppFunctionAndName_CPP.hpp
//===========================================================================
