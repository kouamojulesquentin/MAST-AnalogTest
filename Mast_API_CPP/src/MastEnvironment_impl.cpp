//===========================================================================
//                           MastEnvironment_impl.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastEnvironment_impl.cpp
//!
//! Implements class MastEnvironment_impl
//!
//===========================================================================

#include "MastEnvironment_impl.hpp"
#include "MastConfiguration.hpp"
#include "CustomFileSink.h"
#include "Utility.hpp"
#include "FileSystem.hpp"
#include "Plugins.hpp"
#include "MastConfig.hpp"
#include "PDL_AlgorithmsRepository.hpp"
#include "ConfigureAlgorithmFactory.hpp"
#include "AccessInterfaceProtocolFactory.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "SystemModel.hpp"
#include "SystemModelManager.hpp"
#include "SIT_reader.hpp"
#include "Startup.hpp"

#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "g3log/loglevels.hpp"

#include <fstream>

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::ofstream;

using namespace mast;

//! Only destructs members
//!
//! @note Not declared as default reduce included headers in header
MastEnvironment_impl::~MastEnvironment_impl ()
{
  LOG(DEBUG) << "Cleaning up MAST environment";
  Startup::ForgetSystemModel();
  Startup::ForgetManager();
  SystemModelNode::ResetNodeIdentifier();

  LOG(INFO) << "MAST Stopped";
  Startup::StopLogger();
}
//
//  End of: MastEnvironment_impl::~MastEnvironment_impl
//---------------------------------------------------------------------------


//! Initializes default logger if not in unit tests context
//!
//! @note In unit tests context, the logger is already initialized (and managed by unit tests main)
//!
//! @param unitTestContext  True when in unit tests context
//!
MastEnvironment_impl::MastEnvironment_impl (bool unitTestContext)
  : m_unitTestsContext(unitTestContext)
{
  if (!m_unitTestsContext)
  {
    InitializeLogger();
  }
}
//
//  End of: MastEnvironment_impl::MastEnvironment_impl
//---------------------------------------------------------------------------



//! Changes top level AccessInterface protocol with one defined by user
//!
//! @param protocolName Identifier of protocol ; it must be a built-in one or
//!                     have been registered by a plugin
//!
void MastEnvironment_impl::ChangeAccessInterfaceProtocol (const string& protocolName)
{
  LOG(DEBUG) << "Force protocol \"" << protocolName << "\" to top level access interface";

  auto factory  = AccessInterfaceProtocolFactory::Instance();
  auto protocol = factory.Create(protocolName);

  LOG(WARNING) << "Changing protocol is Not Yet Implemented !!!";
}
//
//  End of: MastEnvironment_impl::ChangeAccessInterfaceProtocol
//---------------------------------------------------------------------------



//! Checks model reporting any issues
//! @note Either save report to user defined report file and log only minimal status
//!       or log complete status
//!       Optional report file MUST be different than log file
void MastEnvironment_impl::CheckModel ()
{
  auto checkResult = Startup::sm_systemModel->Check();

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
        os << checkResult.MakeReport();
        CHECK_FAILED("Model is not correct. See file: \""s + checkFilePath + "\" for details");
      }
    }
  }
  else // Only log status
  {
    if (checkResult.HasErrors())
    {
      auto report = checkResult.MakeCompactErrorReport();
      std::replace(report.begin(), report.end(), '\n', ' ');
      CHECK_FAILED("Model is not correct:" + report);
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
//  End of: MastEnvironment_impl::CheckModel
//---------------------------------------------------------------------------



//! Configures logger according to parsed options
//!
void MastEnvironment_impl::ConfigureLogger ()
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

    auto logFormatter = Startup::sm_logFormatter;

    logFormatter->ShowDate         (IsSet(shownItems, mast::LoggerShownItems::Date));
    logFormatter->ShowTime         (IsSet(shownItems, mast::LoggerShownItems::Time));
    logFormatter->ShowMicroseconds (IsSet(shownItems, mast::LoggerShownItems::Microseconds));
    logFormatter->ShowLevel        (IsSet(shownItems, mast::LoggerShownItems::Level));
    logFormatter->ShowThreadId     (IsSet(shownItems, mast::LoggerShownItems::ThreadId));
    logFormatter->ShowFileName     (IsSet(shownItems, mast::LoggerShownItems::FileName));
    logFormatter->ShowFunctionName (IsSet(shownItems, mast::LoggerShownItems::FunctionName));
    logFormatter->ShowLineNumber   (IsSet(shownItems, mast::LoggerShownItems::LineNumber));

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

    auto loggerKind = m_configuration->LoggerKind();

    if (IsSet(loggerKind, mast::LoggerKind::File))
    {
      // ---------------- Sink for logging to file
      //
      auto customSink = std::make_unique<g3::CustomFileSink>(m_configuration->LoggerFilePath(),
                                                             g3::CustomFileSink::FlushMode::AutoBackground,
                                                             *logFormatter);

      customSink->Clear();
      Startup::sm_logger->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);
    }

    if (IsSet(loggerKind, mast::LoggerKind::Cout))
    {
      auto coutSink = std::make_unique<g3::CoutLoggerSink>(*logFormatter);
      Startup::sm_logger->addSink(std::move(coutSink), &g3::CoutLoggerSink::ReceiveLogMessage);
    }

    // ---------------- Disable, "Cerr" logger if not requested
    //
    if (!IsSet(loggerKind, mast::LoggerKind::OnlyErrorsOnCerr))
    {
      std::future<bool> received = Startup::sm_cerrSinkHandle->call(&g3::LoggerSink::Enabled, false);
    }
  }
}
//
//  End of: MastEnvironment_impl::ConfigureLogger
//---------------------------------------------------------------------------



//! Creates PDL algorithm with their associated nodes
//!
void MastEnvironment_impl::CreateApplications ()
{
  CHECK_VALUE_NOT_NULL(Startup::sm_manager, "Manager must be created before PDL algorithm creation");

  const auto& algosRepository = PDL_AlgorithmsRepository::Instance();

  LOG(INFO) << "Creating PDL algorithm associated to nodes (from SIT description)";
  for (const auto& association : m_algoNamesAssociatedToNodes)
  {
    CHECK_VALUE_NOT_EMPTY (association.appName, "PDL algorithm name cannot be empty");
    CHECK_VALUE_NOT_NULL  (association.node,    "Cannot create PDL algorithm " + Utility::IfNotEmpty_SingleQuoteAndSuffixWithSpace(association.appName));

    auto algo = algosRepository.GetAlgorithm(association.appName);

    LOG(DEBUG) << "Try to create PDL algorithm \"" << association.appName      << "\""
               << " associated to node \""         << association.node->Name() << "\"";

    Startup::sm_manager->CreateApplicationThread(association.node, algo, association.appName);

    LOG(INFO) << "PDL algorithm \""                              << association.appName
              << "\" has been created and associated to node \"" << association.node->Name() << "\"";
  }
}
//
//  End of: MastEnvironment_impl::CreateApplications
//---------------------------------------------------------------------------


//! Creates system model manager
//!
void MastEnvironment_impl::CreateManager ()
{
  LOG(INFO) << "Creating system model manager";

  // ---------------- Configuration algorithm
  //
  auto configAlgoName    = m_configuration->ConfigurationAlgorithm();
  auto configAlgoFactory = ConfigureAlgorithmFactory::Instance();
  auto configAlgo        = configAlgoFactory.Create(configAlgoName);

  LOG(INFO) << "Using configuration algorithm: \"" << configAlgoName << "\"";

  // ---------------- Manager monitor
  //
  //! @todo [JFC]-[June/16/2017]:
  //!
  LOG(WARNING) << "Export of gml/txt of system model during execution is Not Yet Implemented";
  auto managerMonitor = shared_ptr<SystemModelManagerMonitor>();
  if (m_configuration->ReportManagerActivity())
  {
    auto options = m_configuration->ManagerActivityOptions();

    if (   IsSet(m_configuration->GmlReportMoments(),         ReportMoments::BeforeConfiguration)
        || IsSet(m_configuration->PrettyPrintReportMoments(), ReportMoments::BeforeConfiguration)
       )
    {
      options |= ManagerMonitorOptions::BeforeConfiguration;
    }

    if (   IsSet(m_configuration->GmlReportMoments(),         ReportMoments::AfterConfiguration)
        || IsSet(m_configuration->PrettyPrintReportMoments(), ReportMoments::AfterConfiguration)
       )
    {
      options |= ManagerMonitorOptions::AfterConfiguration;
    }


    managerMonitor = make_shared<SystemModelManagerMonitor>(options);
    managerMonitor->ExportBasePath(m_configuration->ManagerActivityFileBasePath());
  }

  // ---------------- System Model Manager
  //
  Startup::sm_manager = make_shared<SystemModelManager>(*Startup::sm_systemModel, std::move(configAlgo), managerMonitor);

  LOG(DEBUG) << "Created system model manager";
}
//
//  End of: MastEnvironment_impl::CreateManager
//---------------------------------------------------------------------------



//! Creates system model using parsed options and loaded plugins
//!
void MastEnvironment_impl::CreateSystemModel ()
{
  const auto& algos = PDL_AlgorithmsRepository::Instance();

  CHECK_VALUE_NOT_ZERO(algos.RegisteredAlgorithmCount(), "A plugin must be loaded with at least one registered PDL algorithm function");

  auto sitFile = m_configuration->SitFilePath();
  CHECK_VALUE_NOT_EMPTY(sitFile, "A valid (non empty) SIT file path must be provided");

  sitFile = GetActualSitFilePath(sitFile);
  CHECK_FILE_EXISTS_EX(sitFile, "SIT file: ");
  LOG(INFO) << "Using SIT file: " << sitFile;

  LOG(INFO) << "Creating System Model";
  auto systemModel = make_shared<SystemModel>();

  auto reader = SIT::SIT_Reader(systemModel);

  reader.parse(sitFile);

  auto topNode = dynamic_pointer_cast<ParentNode>(reader.ParsedSystemModel());

  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: "s + sitFile);
  LOG(INFO) << "SIT has been parsed successfully";

  systemModel->ReplaceRoot(topNode, false);
  Startup::sm_systemModel = systemModel;

  m_algoNamesAssociatedToNodes = reader.PDLAlgorithmNameToNodeAssociation();

  if (!m_configuration->AccessInterfaceProtocol().empty())
  {
    ChangeAccessInterfaceProtocol(m_configuration->AccessInterfaceProtocol());
  }

  // ---------------- Checks
  //
  if (m_configuration->ModelChecking())
  {
    CheckModel();
  }

  //! @todo [JFC]-[June/16/2017]:
  //!
  LOG(WARNING) << "Export of gml/txt of system model is Not Yet Implemented";


  LOG(INFO) << "Created System Model";
}
//
//  End of: MastEnvironment_impl::CreateSystemModel
//---------------------------------------------------------------------------



//! Tries to find actual SIT file path from its name
//!
//! @param sitFile  A SIT file name (or path)
//!
//! @return sitFile if it represents a file that can be open from current working directory, first found actual path otherwise
string MastEnvironment_impl::GetActualSitFilePath (const string& sitFile) const
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
//  End of: MastEnvironment_impl::GetActualSitFilePath
//---------------------------------------------------------------------------


//! Initializes logger
//!
void MastEnvironment_impl::InitializeLogger ()
{
  auto logger       = g3::LogWorker::createLogWorker();
  auto logFormatter = make_shared<g3::LogFormatter>();

  logFormatter->ShowDate         (false);
  logFormatter->ShowTime         (true);
  logFormatter->ShowFileName     (false);
  logFormatter->ShowFunctionName (true);
  logFormatter->ShowLineNumber   (false);
  logFormatter->ShowLevel        (true);
  logFormatter->ShowThreadId     (false);

  // ---------------- Sink for logging errors to std::cerr
  //
  auto cerrSink = std::make_unique<g3::ErrorsOnCerrLoggerSink>(*logFormatter);
  Startup::sm_cerrSinkHandle = logger->addSink(std::move(cerrSink), &g3::ErrorsOnCerrLoggerSink::ReceiveLogMessage);

  g3::initializeLogging(logger.get());

  Startup::sm_logFormatter = logFormatter;
  Startup::sm_logger       = std::move(logger);

  g3::logEnabled(true);
  LOG(DEBUG) << "Mast started";
}
//
//  End of MastEnvironment_impl::InitializeLogger
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
void MastEnvironment_impl::LoadPlugins ()
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
//  End of: MastEnvironment_impl::LoadPlugins
//---------------------------------------------------------------------------


//! Builds a vector of options from command line arguments
//!
//! @param argc   Number of arguments (first one is application name)
//! @param argv   Array of arguments
//!
vector<string> MastEnvironment_impl::MakeArgumentsVector (int argc, const char* argv[])
{
  vector<string> arguments;

  for (int ii = 0 ; ii < argc ; ++ii)
  {
    arguments.emplace_back(argv[ii]);
  }
  return arguments;
}
//
//  End of: MastEnvironment_impl::MakeArgumentsVector
//---------------------------------------------------------------------------



//! Parses options - from list of command line arguments
//!
//! @param arguments  Command line arguments (first one is application name)
//!
void MastEnvironment_impl::ParseOptions (vector<string> arguments)
{
  LOG(INFO) << "Parsing MAST options";
  if (!m_configuration)
  {
    m_configuration = make_shared<MastConfiguration>();
  }

  m_configuration->AutomaticExit(false);  // To get an opportunity to log errors befores closing "properly"
  m_configuration->Update(arguments);

  if (m_configuration->ShouldExit())
  {
    throw std::runtime_error("Failed to parse user options");
  }

  if (!m_unitTestsContext)
  {
    ConfigureLogger();
    LOG(DEBUG) << "Logger (re)configured";
  }
  LOG(INFO) << "MAST options parsed";
}
//
//  End of: MastEnvironment_impl::ParseOptions
//---------------------------------------------------------------------------



//! Starts system model manager AND wait till it ends
//!
//! @note This is a blocking call, till the end of all the PDL algorithms!
//!
void MastEnvironment_impl::Start ()
{
  LOG(INFO) << "Starting MAST scheduler and PDL algorithm(s)";
  Startup::sm_manager->Start();
  Startup::sm_manager->StartCreatedApplicationThreads();
  Startup::sm_manager->WaitForApplicationsEnd();
  Startup::sm_manager->Stop();
}
//
//  End of: MastEnvironment_impl::Start
//---------------------------------------------------------------------------


//! Starts MAST with options from command line arguments
//!
//! @param arguments  Command line arguments (first one is application name)
//!
void MastEnvironment_impl::Start (std::vector<std::string> arguments)
{
  ParseOptions(arguments);
  LoadPlugins();
  CreateSystemModel();
  CreateManager();
  CreateApplications();
  Start();
}
//
//  End of: MastEnvironment_impl::Start
//---------------------------------------------------------------------------

//===========================================================================
// End of MastEnvironment_impl.cpp
//===========================================================================
