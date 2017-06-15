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
#include "FileSystem.hpp"
#include "Plugins.hpp"
#include "MastConfig.hpp"
#include "PDL_AlgorithmsRepository.hpp"
#include "SystemModel.hpp"
#include "SIT_reader.hpp"

#include <fstream>

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::ofstream;

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



//! Checks model reporting any issues
//! @note Either save report to user defined report file and log only minimal status
//!       or log complete status
//!       Optional report file MUST be different than log file
void MastEnvironment::CheckModel ()
{
  auto checkResult = m_sm->Check();

  const auto& checkFilePath = m_configuration->ModelCheckingFilePath();
  if (!checkFilePath.empty()) // Save report to specific file?
  {
    ofstream os(checkFilePath);
    if (os.is_open())
    {
      if (!checkResult.HasIssues())
      {
        auto message = "No issue found in model";
        LOG(INFO) << message;
        os        << message;
      }
      else
      {
        LOG(ERROR_LVL) << "Model is not correct. See file: \"" << checkFilePath << "\" for details";
        os << checkResult.MakeReport();
      }
    }
  }
  else // Only log status
  {
    if (checkResult.HasErrors())
    {
      LOG(ERROR_LVL) << checkResult.MakeReport();
    }
    else if (checkResult.HasWarnings())
    {
      LOG(WARNING) << checkResult.MakeReport();
    }
    else if (checkResult.HasIssues())
    {
      LOG(INFO) << checkResult.MakeReport();
    }
    else
    {
      LOG(INFO) << "No issue found in model";
    }
  }
}
//
//  End of: MastEnvironment::CheckModel
//---------------------------------------------------------------------------



//! Configures logger according to parsed options
//!
void MastEnvironment::ConfigureLogger ()
{
  namespace g3l = g3::only_change_at_initialization;

  if (!m_configuration->LoggerEnabled())  // Always keep error level!
  {
    g3l::setLogLevel(DEBUG,   false);
    g3l::setLogLevel(INFO,    false);
    g3l::setLogLevel(WARNING, false);
  }
  else
  {
    auto shownItems = m_configuration->LoggerShownItems();

    m_logFormatter->ShowDate         (IsSet(shownItems, mast::LoggerShownItems::Date));
    m_logFormatter->ShowTime         (IsSet(shownItems, mast::LoggerShownItems::Time));
    m_logFormatter->ShowMicroseconds (IsSet(shownItems, mast::LoggerShownItems::Microseconds));
    m_logFormatter->ShowLevel        (IsSet(shownItems, mast::LoggerShownItems::Level));
    m_logFormatter->ShowThreadId     (IsSet(shownItems, mast::LoggerShownItems::ThreadId));
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

    bool useCout =   (m_configuration->LoggerKind() == mast::LoggerKind::Cout)
                  || (m_configuration->LoggerKind() == mast::LoggerKind::CopyAllOnCout);

    bool useFile =   (m_configuration->LoggerKind() == mast::LoggerKind::Std)
                  || (m_configuration->LoggerKind() == mast::LoggerKind::CopyAllOnCout);

    if (useFile)
    {
      // ---------------- Sink for logging to file
      //
      auto customSink = std::make_unique<g3::CustomFileSink>(m_configuration->LoggerFilePath(),
                                                             g3::CustomFileSink::FlushMode::AutoBackground,
                                                             *m_logFormatter);

      customSink->Clear();
      m_logger->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);
    }

    if (useCout)
    {
      auto coutSink = std::make_unique<g3::CoutLoggerSink>(*m_logFormatter);
      m_logger->addSink(std::move(coutSink), &g3::CoutLoggerSink::ReceiveLogMessage);
    }
  }
}
//
//  End of: MastEnvironment::ConfigureLogger
//---------------------------------------------------------------------------



//! Creates system model using parsed options and loaded plugins
//!
void MastEnvironment::CreateSystemModel ()
{
  const auto& algos = PDL_AlgorithmsRepository::Instance();

  CHECK_VALUE_NOT_ZERO(algos.RegisteredAlgorithmCount(), "A plugin must be loaded with at least one registered PDL algorithm function");

  auto sitFile = m_configuration->SitFilePath();
  CHECK_VALUE_NOT_EMPTY(sitFile, "A valid (non empty) SIT file path must be provided");

  sitFile = GetActualSitFilePath(sitFile);
  CHECK_FILE_EXISTS(sitFile);
  LOG(INFO) << "Using SIT file: " << sitFile;

  LOG(INFO) << "Creating SystemModel";
  m_sm = make_shared<SystemModel>();

  auto reader = SIT::SIT_Reader(m_sm);

  reader.parse(sitFile);

  auto topNode = dynamic_pointer_cast<ParentNode>(reader.ParsedSystemModel());

  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: "s + sitFile);
  LOG(INFO) << "SIT has been parsed successfully";

  m_sm->ReplaceRoot(topNode, false);

  if (m_configuration->ModelChecking())
  {
    CheckModel();
  }

//+  const auto& namesAndNodes = reader.namesAndNodes;
}
//
//  End of: MastEnvironment::CreateSystemModel
//---------------------------------------------------------------------------



//! Tries to find actual SIT file path from its name
//!
//! @param sitFile  A SIT file name (or path)
//!
//! @return sitFile if it represents a file that can be open from current working directory, first found actual path otherwise
string MastEnvironment::GetActualSitFilePath (const string& sitFile) const
{
  if (Utility::FileExists(sitFile))
  {
    return sitFile;
  }

  // ---------------- Extract directories paths of loaded plugins (at least one should be associated with PDL algorithms)
  //
  vector<string> candidateDirs {
                                 ".",
                               };

  for (const auto& pluginPath : m_loadedPluginsPath)
  {
    // ---------------- Extract directory path
    //
    auto directoryPath = Utility::ExtractDirectoryPath(pluginPath);

    if (std::find(candidateDirs.cbegin(), candidateDirs.cend(), directoryPath) == candidateDirs.cend())
    {
      candidateDirs.emplace_back(directoryPath);
    }
  }

  for (const auto& hintDir : candidateDirs)
  {
    auto dirPath = hintDir.empty() ? sitFile
                                   : hintDir + DIRECTORY_SEPARATOR + sitFile;
    // Try with hint directory path
    if (Utility::FileExists(dirPath))
    {
      return dirPath;
    }

    // Try with default extension
    dirPath.append(".sit");
    if (Utility::FileExists(dirPath))
    {
      return dirPath;
    }
  }

  return sitFile;
}
//
//  End of: MastEnvironment::GetActualSitFilePath
//---------------------------------------------------------------------------


//! Initializes logger
//!
void MastEnvironment::InitializeLogger ()
{
  m_logger       = g3::LogWorker::createLogWorker();
  m_logFormatter = make_shared<g3::LogFormatter>();

  m_logFormatter->ShowDate         (false);
  m_logFormatter->ShowTime         (true);
  m_logFormatter->ShowFileName     (false);
  m_logFormatter->ShowFunctionName (true);
  m_logFormatter->ShowLineNumber   (false);
  m_logFormatter->ShowLevel        (true);
  m_logFormatter->ShowThreadId     (false);

  // ---------------- Sink for logging errors to std::cerr
  //
  auto cerrSink = std::make_unique<g3::ErrorsOnCerrLoggerSink>(*m_logFormatter);
  m_logger->addSink(std::move(cerrSink), &g3::ErrorsOnCerrLoggerSink::ReceiveLogMessage);

  g3::initializeLogging(m_logger.get());
  g3::logEnabled(true);
  LOG(DEBUG) << "Mast started";
}
//
//  End of MastEnvironment::InitializeLogger
//---------------------------------------------------------------------------


//! Loads plugins defined by options
//!
//! @note Plugins are loaded in following order:
//!   - Plugins given at Command-line
//!   - Plugins contained in directories given at Command-line
//!   - Plugins given in Configuration file
//!   - Plugins contained in directory given in Configuration file
//!   - Plugins in current directory
//!   - Plugins in MAST root "Plugins" directory
//!
void MastEnvironment::LoadPlugins ()
{
  CHECK_VALUE_NOT_NULL(m_configuration, "Configuration (options) must have been parsed before loading plugin(s)");

  const auto cwd    = FileSystem::CurrentPath();
  const auto appDir = m_configuration->ApplicationPath();

  auto makePathWithDefaultPluginName = [](string basePath)
  {
    basePath.append(DIRECTORY_SEPARATOR).append(PLUGINS_DIRECTORY_NAME);
    return basePath;
  };

  // The directories from where plugins search is done
  const vector<string> candidateDirs {
                                       "",
                                       "."s,
                                       makePathWithDefaultPluginName("."),
                                       cwd,
                                       makePathWithDefaultPluginName(cwd),
                                       appDir,
                                       makePathWithDefaultPluginName(appDir),
                                     };

  m_loadedPluginsPath.clear();

  // ---------------- Plugins Files
  //
  const auto& plugins = m_configuration->PluginDLLs();
  for (const auto& name : plugins)
  {
    auto loaded = false;
    for (const auto& hintDir : candidateDirs)
    {
      LOG(DEBUG) << "Will try to load plugin: " << name << " - Trying in directory \"" << hintDir << "\" -";
      auto effectiveDllPath = Plugins::TryLoadPlugin(hintDir, name);
      if (!effectiveDllPath.empty())
      {
        loaded = true;
        LOG(INFO) << "Loaded plugin: " << effectiveDllPath;
        m_loadedPluginsPath.emplace_back(std::move(effectiveDllPath));
        break;
      }
    }

    if (!loaded)
    {
      LOG(INFO) << "Failed to load plugin: " << name;
    }
  }

  // ---------------- Plugins directories
  //
  const auto& pluginDirs = m_configuration->PluginDirectories();
  for (const auto& name : pluginDirs)
  {
    for (const auto& hintDir : candidateDirs)
    {
      auto dirPath = hintDir.empty() ? name
                                     : hintDir + DIRECTORY_SEPARATOR + name;

      if (FileSystem::IsDirectory(dirPath))
      {
        LOG(DEBUG) << "Will try to load plugin(s) from directory: " << dirPath;
        auto loaded = Plugins::LoadPluginsExcept(dirPath, m_loadedPluginsPath);
        LOG(INFO) << "Have loaded " << loaded.size() << " plugin(s) from directory: " << dirPath;

        // ---------------- Save loaded plugins
        //
        for (auto& loadedPlugin : loaded)
        {
          m_loadedPluginsPath.emplace_back(std::move(loadedPlugin));
        }
        break;
      }
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
    LOG(DEBUG) << "Logger (re)configured";
  }
}
//
//  End of: MastEnvironment::ParseOptions
//---------------------------------------------------------------------------





//===========================================================================
// End of MastEnvironment.cpp
//===========================================================================
