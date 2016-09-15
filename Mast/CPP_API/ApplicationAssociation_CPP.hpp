//===========================================================================
//                           ApplicationAssociation_CPP.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ApplicationAssociation_CPP.hpp
//!
//! Declares ApplicationAssociation class
//!
//===========================================================================


#ifndef APPLICATIONASSOCIATION_CPP_H__63BEBF56_89D4_4EC6_FFBF_B574CCDCB07C__INCLUDED_
  #define APPLICATIONASSOCIATION_CPP_H__63BEBF56_89D4_4EC6_FFBF_B574CCDCB07C__INCLUDED_

#include <string>
#include <functional>

namespace mast
{
  //! Associates application functions (algorithm) with some node path and optionally an application name
  //!
  struct ApplicationAssociation final
  {
    using Application_t = std::function<void()>;

    Application_t appFunction;
    std::string   topNodePath;
    std::string   appName;

    ApplicationAssociation(Application_t p_appFunction, std::string p_topNodePath, std::string p_appName = "")
      : appFunction (p_appFunction)
      , topNodePath (std::move(p_topNodePath))
      , appName     (std::move(p_appName))
    {}
  };
  //
  //  End of ApplicationAssociation struct declaration
  //---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined APPLICATIONASSOCIATION_CPP_H__63BEBF56_89D4_4EC6_FFBF_B574CCDCB07C__INCLUDED_

//===========================================================================
// End of ApplicationAssociation_CPP.hpp
//===========================================================================
