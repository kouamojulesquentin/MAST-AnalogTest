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
#include "SystemModelManagerMonitor.hpp"
#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "CustomFileSink.h"
#include "LoggerSinks.h"


using std::shared_ptr;
using std::make_shared;

using namespace mast;

shared_ptr<g3::LogWorker>      Startup::sm_logger;
shared_ptr<SystemModel>        Startup::sm_systemModel;
shared_ptr<SystemModelManager> Startup::sm_manager;

namespace
{
//! Initializes logger facility
//!
std::unique_ptr<g3::LogWorker> InitializeLogger ()
{
  auto logworker    = g3::LogWorker::createLogWorker();
  auto logFormatter = g3::LogFormatter();

  logFormatter.ShowDate(false);
  logFormatter.ShowTime(true);
  logFormatter.ShowFileName(false);
  logFormatter.ShowFunctionName(true);
  logFormatter.ShowLineNumber(false);

  // ---------------- Sink for logging to file
  //
  auto customSink = std::make_unique<g3::CustomFileSink>("Log.txt", g3::CustomFileSink::FlushMode::AutoBackground, logFormatter);
  customSink->Clear();

  logworker->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);

  // ---------------- Sink for logging errors to std::cerr
  //
  auto cerrSink = std::make_unique<g3::ErrorsOnCerrLoggerSink>();
  logworker->addSink(std::move(cerrSink), &g3::ErrorsOnCerrLoggerSink::ReceiveLogMessage);

  g3::initializeLogging(logworker.get());
  g3::logEnabled(true);
  LOG(DEBUG) << "Logger started";
  return logworker;
}
//
//  End of: InitializeLogger
//---------------------------------------------------------------------------
} // End of unnamed namespace

//! Gets rid of common SystemModel
//!
void Startup::ForgetSystemModel ()
{
  LOG(DEBUG) << "Get rid of SystemModel";
  sm_systemModel.reset();
}
//
//  End of: Startup::ForgetSystemModel
//---------------------------------------------------------------------------


//! Gets rid of common SystemModelManager
//!
void Startup::ForgetManager ()
{
  if (sm_manager)
  {
    LOG(DEBUG) << "Stopping and getting rid of manager";
    sm_manager->Stop();
    sm_manager->Monitor(nullptr);
    sm_manager.reset();
  }
}
//
//  End of: Startup::ForgetManager
//---------------------------------------------------------------------------


//! Returns shared logger
//!
shared_ptr<g3::LogWorker> Startup::GetLogger ()
{
  if (!sm_logger)
  {
    sm_logger = InitializeLogger();
  }
  return sm_logger;
}
//
//  End of: Startup::GetManager
//---------------------------------------------------------------------------



//! Returns shared SystemModelManager
//!
shared_ptr<SystemModelManager> Startup::GetManager ()
{
  if (!sm_manager)
  {
    auto sm = GetSystemModel();

    LOG(INFO) << "Creating SystemModelManager";
    auto defaultConfigAlgo = make_shared<ConfigureAlgorithm_LastOrDefault>();
    auto monitor           = make_shared<SystemModelManagerMonitor>(ManagerMonitorOptions::Std);
    sm_manager             = make_shared<mast::SystemModelManager>(*sm, defaultConfigAlgo, monitor);
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
    LOG(INFO) << "Creating new SystemModel";
    sm_systemModel = make_shared<mast::SystemModel>();
  }

  return sm_systemModel;
}
//
//  End of: Startup::GetSystemModel
//---------------------------------------------------------------------------


//! Initializes logger facility
//!
void Startup::StartLogger ()
{
  if (!sm_logger)
  {
    sm_logger = InitializeLogger();
  }
}
//
//  End of: Startup::GetSystemModel
//---------------------------------------------------------------------------


//! Initializes logger facility
//!
void Startup::StopLogger ()
{
  if (sm_logger)
  {
    LOG(INFO) << "Stopping logger";
    g3::logEnabled(false);
    sm_logger.reset();
  }
}
//
//  End of: Startup::GetSystemModel
//---------------------------------------------------------------------------

//===========================================================================
// End of Startup.cpp
//===========================================================================
