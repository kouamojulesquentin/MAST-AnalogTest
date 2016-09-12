//===========================================================================
//                           Zybo.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Zybo.cpp
//!
//! Implements functions for demos using Zybo board
//!
//===========================================================================

#include "Startup.hpp"
#include "ApplicationAssociation_CPP.hpp"
#include "PDL_Adapter_CPP.hpp"
//+#include "g3log/g3log.hpp"
#include "Zybo.hpp"

#include <string>

using std::vector;
using std::string;
using namespace mast;


//! Returns descriptions for applications to run on Zybo board
//!
//! @note Adapt this function as necessary
//!       It must nevertheless be compliant with SystemModel (possibly with 'SIT' file)
vector<ApplicationAssociation> test::Zybo_CreateApplicationsDescriptor ()
{
  auto app = []()
  {
    auto     registerPath = "reg_0";
    auto     loopCount    = 891u;
    uint16_t initialValue = 1u;

    while (loopCount)
    {
      iWrite(registerPath, initialValue);
      iApply();

      ++initialValue;
      --loopCount;
    }
  };

  vector<ApplicationAssociation> associations;

  associations.emplace_back(app, ".", "Zybo");

  return associations;
}
//
//  End of: test::Zybo_CreateApplicationsDescriptor
//---------------------------------------------------------------------------


//===========================================================================
// End of Zybo.cpp
//===========================================================================
