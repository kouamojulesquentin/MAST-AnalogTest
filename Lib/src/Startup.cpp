//===========================================================================
//                           Startup.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Startup.cpp
//!
//! Implements helpers to startup mast
//!
//===========================================================================


#include "Startup.hpp"
#include "SystemModel.hpp"
#include "SystemModelManager.hpp"


using std::shared_ptr;
using std::make_shared;

using namespace mast;

std::shared_ptr<SystemModel>        Startup::sm_systemModel;
std::shared_ptr<SystemModelManager> Startup::sm_manager;



//! Returns SystemModelManager singleton
//!
shared_ptr<SystemModelManager> Startup::GetManager ()
{
  if (!sm_manager)
  {
    sm_manager = make_shared<mast::SystemModelManager>(*GetSystemModel());
  }
  return sm_manager;
}
//
//  End of: Startup::GetManager
//---------------------------------------------------------------------------



//! Returns systemModel singleton
//!
shared_ptr<SystemModel> Startup::GetSystemModel ()
{
  if (!sm_systemModel)
  {
    sm_systemModel = make_shared<mast::SystemModel>();
  }

  return sm_systemModel;
}
//
//  End of: Startup::GetSystemModel
//---------------------------------------------------------------------------


//===========================================================================
// End of Startup.cpp
//===========================================================================
