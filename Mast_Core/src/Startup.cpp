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
using std::unique_ptr;
using std::make_shared;

using namespace mast;

shared_ptr<SystemModel>               Startup::sm_systemModel;
shared_ptr<SystemModelManager>        Startup::sm_manager;
shared_ptr<g3::LogWorker>             Startup::sm_logger;
shared_ptr<g3::LogFormatter>          Startup::sm_logFormatter;   //!< Logger message formatter
unique_ptr<Startup::CerrSinkHandle_t> Startup::sm_cerrSinkHandle; //!< Initial logger sink that is disabled once user requested sink are connected

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
    InitializeLogger();
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
void Startup::InitializeLogger ()
{
  sm_logger       = g3::LogWorker::createLogWorker();
  sm_logFormatter = make_shared<g3::LogFormatter>();

  sm_logFormatter->ShowDate         (false);
  sm_logFormatter->ShowTime         (true);
  sm_logFormatter->ShowFileName     (false);
  sm_logFormatter->ShowFunctionName (true);
  sm_logFormatter->ShowLineNumber   (false);
  sm_logFormatter->ShowLevel        (true);
  sm_logFormatter->ShowThreadId     (false);

  // ---------------- Sink for logging to file
  //
  auto customSink = std::make_unique<g3::CustomFileSink>("Log.txt", g3::CustomFileSink::FlushMode::AutoBackground, *sm_logFormatter);
  customSink->Clear();

  sm_logger->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);

  // ---------------- Sink for logging errors to std::cerr
  //
  auto cerrSink = std::make_unique<g3::ErrorsOnCerrLoggerSink>(*sm_logFormatter);
  sm_cerrSinkHandle = sm_logger->addSink(std::move(cerrSink), &g3::ErrorsOnCerrLoggerSink::ReceiveLogMessage);

  g3::initializeLogging(sm_logger.get());
  g3::logEnabled(true);
  LOG(DEBUG) << "Logger started";
}
//
//  End of: Startup::InitializeLogger
//---------------------------------------------------------------------------


//! Initializes logger facility
//!
void Startup::StartLogger ()
{
  if (!sm_logger)
  {
    InitializeLogger();
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
    sm_cerrSinkHandle.reset();
    sm_logFormatter.reset();
  }
}
//
//  End of: Startup::GetSystemModel
//---------------------------------------------------------------------------

//===========================================================================
// End of Startup.cpp
//===========================================================================
