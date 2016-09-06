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
  //! Associates functions (applications) with some node path and optionally a (debug) name
  //!
  struct ApplicationAssociation final
  {
    using Application_t = std::function<void()>;

    Application_t function;
    std::string   topNodePath;
    std::string   debugName;

    ApplicationAssociation(Application_t p_function, std::string p_topNodePath, std::string p_debugName = "")
      : function    (p_function)
      , topNodePath (std::move(p_topNodePath))
      , debugName   (std::move(p_debugName))
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
