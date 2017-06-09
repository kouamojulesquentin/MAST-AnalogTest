//===========================================================================
//                           MastEnvironment.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastEnvironment.cpp
//!
//! Implements class MastEnvironment
//!
//===========================================================================

#include "MastEnvironment.hpp"
#include "MastConfiguration.hpp"
#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "g3log/loglevels.hpp"
#include "CustomFileSink.h"
#include "LoggerSinks.h"
#include "Utility.hpp"
#include "Plugins.hpp"

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;

using namespace mast;


//! Initializes default logger if not in unit tests context
//!
//! @note In unit tests context, the logger is already initialized (and managed by unit tests main)
//!
//! @param unitTestContext  True when in unit tests context
//!
MastEnvironment::MastEnvironment (bool unitTestContext)
  : m_unitTestsContext(unitTestContext)
{
  if (!m_unitTestsContext)
  {
    InitializeLogger();
  }
}
//
//  End of: MastEnvironment::MastEnvironment
//---------------------------------------------------------------------------


//! Configures logger according to parsed options
//!
void MastEnvironment::ConfigureLogger ()
{
  namespace g3l = g3::only_change_at_initialization;

  if (!m_configuration->LoggerEnabled())  // Always keep error level!
  {
//+    g3::logEnabled(false);
    g3l::setLogLevel(DEBUG,   false);
    g3l::setLogLevel(INFO,    false);
    g3l::setLogLevel(WARNING, false);
  }
  else
  {
    auto shownItems = LoggerShownItems();

    m_logFormatter->ShowDate         (IsSet(shownItems, mast::LoggerShownItems::Date));
    m_logFormatter->ShowTime         (IsSet(shownItems, mast::LoggerShownItems::Time));
    m_logFormatter->ShowMicroseconds (IsSet(shownItems, mast::LoggerShownItems::Microseconds));
    m_logFormatter->ShowFileName     (IsSet(shownItems, mast::LoggerShownItems::FileName));
    m_logFormatter->ShowFunctionName (IsSet(shownItems, mast::LoggerShownItems::FunctionName));
    m_logFormatter->ShowLineNumber   (IsSet(shownItems, mast::LoggerShownItems::LineNumber));


    g3::only_change_at_initialization::setLogLevel(ERROR_LVL, true);
    switch (m_configuration->LoggerLevel())
    {
      case mast::LoggerLevel::Debug:
        g3l::setLogLevel(DEBUG,     true);
        g3l::setLogLevel(INFO,      true);
        g3l::setLogLevel(WARNING,   true);
        break;
      case mast::LoggerLevel::Info:
        g3l::setLogLevel(DEBUG,     false);
        g3l::setLogLevel(INFO,      true);
        g3l::setLogLevel(WARNING,   true);
        break;
      case mast::LoggerLevel::Warning:
        g3l::setLogLevel(DEBUG,     false);
        g3l::setLogLevel(INFO,      false);
        g3l::setLogLevel(WARNING,   true);
        break;
      case mast::LoggerLevel::Error:
        g3l::setLogLevel(DEBUG,     false);
        g3l::setLogLevel(INFO,      false);
        g3l::setLogLevel(WARNING,   false);
        break;
      default:
        break;
    }

    if (m_configuration->LoggerKind() == mast::LoggerKind::Std)
    {
      // ---------------- Sink for logging to file
      //
      auto customSink = std::make_unique<g3::CustomFileSink>(m_configuration->LoggerFilePath(),
                                                             g3::CustomFileSink::FlushMode::AutoBackground,
                                                             *m_logFormatter);

      customSink->Clear();
      m_logger->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);
    }
    else if (m_configuration->LoggerKind() == mast::LoggerKind::CopyAllOnCout)
    {
      auto coutSink = std::make_unique<g3::CoutLoggerSink>();
      m_logger->addSink(std::move(coutSink), &g3::CoutLoggerSink::ReceiveLogMessage);
    }
  }
}
//
//  End of: MastEnvironment::ConfigureLogger
//---------------------------------------------------------------------------


//! Initializes logger
//!
void MastEnvironment::InitializeLogger ()
{
  m_logger       = g3::LogWorker::createLogWorker();
  m_logFormatter = make_shared<g3::LogFormatter>();

  m_logFormatter->ShowDate(false);
  m_logFormatter->ShowTime(true);
  m_logFormatter->ShowFileName(false);
  m_logFormatter->ShowFunctionName(true);
  m_logFormatter->ShowLineNumber(false);

  // ---------------- Sink for logging errors to std::cerr
  //
  auto cerrSink = std::make_unique<g3::ErrorsOnCerrLoggerSink>();
  m_logger->addSink(std::move(cerrSink), &g3::ErrorsOnCerrLoggerSink::ReceiveLogMessage);

  //+ (begin JFC June/09/2017): for debug purpose
  auto coutSink = std::make_unique<g3::CoutLoggerSink>();
  m_logger->addSink(std::move(coutSink), &g3::CoutLoggerSink::ReceiveLogMessage);
  //+ (end   JFC June/09/2017):

  g3::initializeLogging(m_logger.get());
  g3::logEnabled(true);
  LOG(DEBUG) << "Mast started";
}
//
//  End of MastEnvironment::InitializeLogger
//---------------------------------------------------------------------------


//! Loads plugins defined by options
//!
void MastEnvironment::LoadPlugins ()
{
  CHECK_VALUE_NOT_NULL(m_configuration, "Configuration (options) must have been parsed before loading plugin(s)");

  const auto& plugins = m_configuration->PluginDLLs();

  for (const auto& name : plugins)
  {
    if (Utility::FileExists(name))
    {
      LOG(INFO) << "Will try to load plugin: " << name;
      Plugins::LoadPlugin(name);
    }
    else
    {
      LOG(INFO) << "Will try to load plugin(s) from directory: " << name;
      Plugins::LoadPlugins(name);
    }
  }
}
//
//  End of: MastEnvironment::LoadPlugins
//---------------------------------------------------------------------------



//! Parses options - from C-Style command line arguments
//!
//! @param argc   Number of arguments (first one is application name)
//! @param argv   Array of arguments
//!
void MastEnvironment::ParseOptions (int argc, const char* argv[])
{
  vector<string> arguments;

  for (int ii = 0 ; ii < argc ; ++ii)
  {
    arguments.emplace_back(argv[ii]);
  }
  ParseOptions(arguments);
}
//
//  End of: MastEnvironment::ParseOptions
//---------------------------------------------------------------------------


//! Parses options - from list of command line arguments
//!
//! @param arguments  Command line arguments (first one is application name)
//!
void MastEnvironment::ParseOptions (vector<string> arguments)
{
  if (!m_configuration)
  {
    m_configuration = make_shared<MastConfiguration>();
  }
  m_configuration->Update(arguments);
  LOG(INFO) << "Options parsed";

  if (!m_unitTestsContext)
  {
    ConfigureLogger();
    LOG(INFO) << "Logger (re)configured";
  }
}
//
//  End of: MastEnvironment::ParseOptions
//---------------------------------------------------------------------------





//===========================================================================
// End of MastEnvironment.cpp
//===========================================================================
