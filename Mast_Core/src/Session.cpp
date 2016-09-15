//===========================================================================
//                           Session.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Session.cpp
//!
//! Implements class Session
//!
//===========================================================================

#include "Session.hpp"
#include "Startup.hpp"
#include "SystemModelManager.hpp"
#include "SystemModelManagerMonitor.hpp"

using std::shared_ptr;
using namespace mast;

//! Release usage of SystemModel and SystemModelManager
Session::~Session ()
{
  Startup::ForgetSystemModel();
  Startup::ForgetManager();
  SystemModelNode::ResetNodeIdentifier();
  Startup::StopLogger();
}
//
//  End of: Session::~Session
//---------------------------------------------------------------------------


//! Gets pointers to SystemModel and SystemModelManager
//!
//! @param enableLog  When true, log facility is initialized and enabled, otherwise it is not started
//!
Session::Session (bool enableLog)
  : logger  (enableLog ? Startup::GetLogger() : nullptr)
  , sm      (Startup::GetSystemModel())
  , manager (Startup::GetManager())
{
}
//
//  End of: Session::Session
//---------------------------------------------------------------------------


//! Gets pointers to SystemModel and SystemModelManager and set monitor for SystemModelManager
//!
Session::Session (shared_ptr<SystemModelManagerMonitor> monitor)
  : Session(true)
{
  manager->Monitor(monitor);
}
//
//  End of: Session::Session
//---------------------------------------------------------------------------



//===========================================================================
// End of Session.cpp
//===========================================================================
