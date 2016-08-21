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



//! Gets rid of common SystemModel
//!
void Startup::ForgetSystemModel ()
{
  sm_systemModel.reset();
}
//
//  End of: Startup::ForgetSystemModel
//---------------------------------------------------------------------------


//! Gets rid of common SystemModelManager
//!
void Startup::ForgetManager ()
{
  sm_manager.reset();
}
//
//  End of: Startup::ForgetManager
//---------------------------------------------------------------------------



//! Returns shared SystemModelManager
//!
shared_ptr<SystemModelManager> Startup::GetManager ()
{
  if (!sm_manager)
  {
    auto sm    = GetSystemModel();
    sm_manager = make_shared<mast::SystemModelManager>(*sm);
  }
  return sm_manager;
}
//
//  End of: Startup::GetManager
//---------------------------------------------------------------------------



//! Returns shared SystemModel
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
