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
#include "FileSystem.hpp"
#include "Plugins.hpp"
#include "MastConfig.hpp"
#include "PDL_AlgorithmsRepository.hpp"
#include "ConfigureAlgorithmFactory.hpp"
#include "AccessInterfaceProtocolFactory.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "SystemModel.hpp"
#include "SystemModelManager.hpp"
#include "SIT_Reader.hpp"
#include "SIT_Printer.hpp"
#include "Startup.hpp"
#include "ModelBuildDriver.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"
#include "AccessInterfaceTranslatorProtocolFactory.hpp"

#include "tclap/DiscardOutput.h"

#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "g3log/loglevels.hpp"

#include <fstream>

using std::vector;
using std::string;
using std::experimental::string_view;
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
  Startup::ForgetSystemModel();
  Startup::ForgetManager();

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
//! @param protocolName
//!
//! @param protocolName Identifier of protocol ; it must be a built-in one or
//!                     have been registered by a plugin
//! @param parameters   Optional parameters to create the protocol
//!
void MastEnvironment_impl::ChangeTopProtocol (const string& protocolName, const string& parameters)
{
  LOG(DEBUG) << "Force protocol \"" << protocolName << "\" to top level access interface";

  auto& factory  = AccessInterfaceProtocolFactory::Instance();
  auto  protocol = factory.Create(protocolName, parameters);

  auto topNode            = Startup::sm_systemModel->Root();
  auto topAccessInterface = dynamic_pointer_cast<AccessInterface>(topNode);
  auto topTranslator = dynamic_pointer_cast<AccessInterfaceTranslator>(topNode);

  if (!topAccessInterface)
   {
   if (!topTranslator)
    {
     auto topChain = dynamic_pointer_cast<Chain>(topNode);
     CHECK_VALUE_NOT_NULL (topChain, "Model top node is not an access interface, a translator nor a chain");
     CHECK_TRUE           (HasOnlyChilrenOfType<AccessInterface>(topChain), "Top chain is not composed of only access interface(s)");

     topAccessInterface = dynamic_pointer_cast<AccessInterface>(topChain->FirstChild());;

     if (topAccessInterface->NextSibling())
     {
      LOG(WARNING) << "Changing protocol for multiple AccessInterface is Not Yet Implemented !!!";
     }
    }
    }

//  CHECK_VALUE_NOT_NULL(topAccessInterface, "Model top node is not an access interface");

  if (topAccessInterface)
   {
   topAccessInterface->SetProtocol(std::move(protocol));
   LOG(INFO) << "Have forced protocol \"" << protocolName << "\" on access interface \"" << topAccessInterface->Name() << "\"";
   }

  if (topTranslator)
   {
  auto& tr_factory  = AccessInterfaceTranslatorProtocolFactory::Instance();
  auto  tr_protocol = tr_factory.Create(protocolName, parameters);
   topTranslator->SetProtocol(std::move(tr_protocol));
   LOG(INFO) << "Have forced protocol \"" << protocolName << "\" on access interface translator \"" << topTranslator->Name() << "\"";
   }
}
//
//  End of: MastEnvironment_impl::ChangeTopProtocol
//---------------------------------------------------------------------------



//! Checks model reporting any issues
//! @note Either save report to user defined report file and log only minimal status
//!       or log complete status
//!       Optional report file MUST be different than log file
void MastEnvironment_impl::CheckModel (shared_ptr<SystemModel> systemModel)
{
  CHECK_PARAMETER_NOT_NULL(systemModel, "Cannot check system model from nullptr");
  auto checkResult = systemModel->Check();

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

  LOG(INFO) << "Creating PDL algorithm associated to nodes (from SIT or ICL description)";
  uint32_t createdAlgo = 0u;
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
    ++createdAlgo;
  }

  if (createdAlgo == 0u)
  {
    LOG(WARNING) << "No PDL algorithm has been created";
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
  CHECK_VALUE_NOT_NULL(Startup::sm_systemModel, "System model must be created before starting the manager");

  // ---------------- Configuration algorithm
  //
  const auto& configAlgoName    = m_configuration->ConfigurationAlgorithm();
  const auto& configAlgoFactory = ConfigureAlgorithmFactory::Instance();
  auto        configAlgo        = configAlgoFactory.Create(configAlgoName);

  LOG(INFO) << "Using configuration algorithm: \"" << configAlgoName << "\"";

  auto managerMonitor = MakeManagerMonitor();


  // ---------------- System Model Manager
  //
  Startup::sm_manager = make_shared<SystemModelManager>(*Startup::sm_systemModel, std::move(configAlgo), managerMonitor);

  Startup::sm_manager->SleepTimeBetweenConfigurations (m_configuration->MinTimeBetweenCycles());
  Startup::sm_manager->DataCycleLoopTimeout           (m_configuration->MaxTimeBetweenCycles());

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

  shared_ptr<SystemModel> systemModel;

  if (!m_configuration->SitFilePath().empty())
  {
    systemModel = CreateSystemModel_FromSitFile(m_configuration->SitFilePath());
  }
  else if (!m_configuration->IclFilePath().empty())
  {
    systemModel = CreateSystemModel_FromIclFile(m_configuration->IclFilePath());
  }
  else if (!m_configuration->IclListFilePath().empty())
  {
    systemModel = CreateSystemModel_FromIclFileList(m_configuration->IclListFilePath());
  }
  else
  {
    CHECK_FAILED("No SIT nor ICL file or list has been provided");
  }

  CHECK_VALUE_NOT_NULL(systemModel, "Faile to create SystemModel");

  // ---------------- Checks
  //
  if (m_configuration->ModelChecking())
  {
    CheckModel(systemModel);
  }
  Startup::sm_systemModel = systemModel;


  if (!m_configuration->AccessInterfaceProtocolName().empty())
  {
    ChangeTopProtocol(m_configuration->AccessInterfaceProtocolName(),
                                  m_configuration->AccessInterfaceProtocolParameters());
  }

  ReportParsedModel();
  LOG(INFO) << "Created System Model";
}
//
//  End of: MastEnvironment_impl::CreateSystemModel
//---------------------------------------------------------------------------


//! Builds SystemModel from "Top" ICL file
//!
//! @param iclFile  ICL file name that represent SUT top
//!
shared_ptr<SystemModel> MastEnvironment_impl::CreateSystemModel_FromIclFile (const string& iclFile)
{
  CHECK_PARAMETER_NOT_EMPTY(iclFile, "A valid (non empty) ICL file path must be provided");

  auto iclFilePath = GetActualProjectFilePath(iclFile, ".icl");
  CHECK_FILE_EXISTS_EX(iclFilePath, "ICL file: ");
  LOG(INFO) << "Using ICL file: " << iclFilePath;
  LOG(INFO) << "Creating System Model";

  ModelBuildDriver buildDriver;
  auto systemModel = buildDriver.CreateModelFromIclFile(iclFilePath);
  auto topNode     = dynamic_pointer_cast<ParentNode>(systemModel->Root());

  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: "s + iclFilePath);

  m_algoNamesAssociatedToNodes = buildDriver.PDLAlgorithmNameToNodeAssociation();

  LOG(INFO) << "ICL file has been parsed successfully";
  return systemModel;
}
//
//  End of: MastEnvironment_impl::CreateSystemModel_FromIclFile
//---------------------------------------------------------------------------


//! Builds SystemModel from list of ICL files
//!
//! @param listFile  File name or path that provides list of ICL files to process to build SUT SystemModel
//!
shared_ptr<SystemModel> MastEnvironment_impl::CreateSystemModel_FromIclFileList (const string& listFile)
{
  CHECK_PARAMETER_NOT_EMPTY(listFile, "A valid (non empty) list file path must be provided");

  auto listFilePath = GetActualProjectFilePath(listFile, ".txt");
  CHECK_FILE_EXISTS_EX(listFilePath, "ICL list file: ");
  LOG(INFO) << "Using ICL list file: " << listFilePath;
  LOG(INFO) << "Creating System Model";

  ModelBuildDriver buildDriver;
  auto systemModel = buildDriver.CreateModelFromIclFileList(listFilePath);
  auto topNode     = dynamic_pointer_cast<ParentNode>(systemModel->Root());

  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: "s + listFilePath);

  m_algoNamesAssociatedToNodes = buildDriver.PDLAlgorithmNameToNodeAssociation();

  LOG(INFO) << "ICL file has been parsed successfully";
  return systemModel;
}
//
//  End of: MastEnvironment_impl::CreateSystemModel_FromIclFile
//---------------------------------------------------------------------------

//! Builds SystemModel from "Top" SIT file
//!
//! @param sitFile  SIT file name that represent SUT top
//!
shared_ptr<SystemModel> MastEnvironment_impl::CreateSystemModel_FromSitFile (const string& sitFile)
{
  CHECK_PARAMETER_NOT_EMPTY(sitFile, "A valid (non empty) SIT file path must be provided");

  auto sitFilePath = GetActualProjectFilePath(sitFile, ".sit");
  CHECK_FILE_EXISTS_EX(sitFilePath, "SIT file: ");
  LOG(INFO) << "Using SIT file: " << sitFilePath;
  LOG(INFO) << "Creating System Model";

  ModelBuildDriver buildDriver;
  auto systemModel = buildDriver.CreateModelFromSitFile(sitFilePath);
  auto topNode     = dynamic_pointer_cast<ParentNode>(systemModel->Root());

  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: "s + sitFilePath);

  m_algoNamesAssociatedToNodes = buildDriver.PDLAlgorithmNameToNodeAssociation();

  LOG(INFO) << "SIT file has been parsed successfully";
  return systemModel;
}
//
//  End of: MastEnvironment_impl::CreateSystemModel_FromSitFile
//---------------------------------------------------------------------------



//! Tries to find actual project file path from its name
//!
//! @param projectFile            A project file name (or path)
//! @param defaultFileExtension   Default extension to consider for project file
//!
//! @return projectFile if it represents a file that can be open from current working directory, first found actual path otherwise
//!
string MastEnvironment_impl::GetActualProjectFilePath (const string& projectFile, string_view defaultFileExtension) const
{
  if (Utility::FileExists(projectFile))
  {
    return projectFile;
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
    auto dirPath = hintDir.empty() ? projectFile
                                   : hintDir + DIRECTORY_SEPARATOR + projectFile;
    // Try with hint directory path
    if (Utility::FileExists(dirPath))
    {
      return dirPath;
    }

    // Try with default extension
    dirPath.append(defaultFileExtension.cbegin(), defaultFileExtension.cend());
    if (Utility::FileExists(dirPath))
    {
      return dirPath;
    }
  }

  return projectFile;
}
//
//  End of: MastEnvironment_impl::GetActualProjectFilePath
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



//! Creates a manager monitor according to user options
//!
//! @note Default is no monitoring
//!
shared_ptr<SystemModelManagerMonitor> MastEnvironment_impl::MakeManagerMonitor ()
{
  // ---------------- Manager monitor
  //
  auto managerMonitor = shared_ptr<SystemModelManagerMonitor>();

  if (!m_configuration->ReportManagerActivity())
  {
    LOG(INFO) << "No manager monitoring";
  }
  else
  {
    auto gml                = m_configuration->GmlPrinting();
    auto gmlMoments         = m_configuration->GmlReportMoments();
    auto gmlBefore          = gml & IsSet(gmlMoments, ReportMoments::BeforeConfiguration);
    auto gmlAfter           = gml & IsSet(gmlMoments, ReportMoments::AfterConfiguration);

    auto prettyPrint        = m_configuration->PrettyPrinting();
    auto prettyPrintMoments = m_configuration->PrettyPrintReportMoments();
    auto prettyPrintBefore  = prettyPrint & IsSet(prettyPrintMoments, ReportMoments::BeforeConfiguration);
    auto prettyPrintAfter   = prettyPrint & IsSet(prettyPrintMoments, ReportMoments::AfterConfiguration);

    auto before = gmlBefore | prettyPrintBefore;
    auto after  = gmlAfter  | prettyPrintAfter;

    auto options = m_configuration->ManagerActivityOptions();
    if (before)
    {
      LOG(INFO) << "Will report model view before configurations";
      options |= ManagerMonitorOptions::BeforeConfiguration;
    }

    if (after)
    {
      LOG(INFO) << "Will report model view after configurations";
      options |= ManagerMonitorOptions::AfterConfiguration;
    }


    if (gmlBefore || gmlAfter)
    {
      LOG(INFO) << "Will export GML graph model view for configurations";
      options |= ManagerMonitorOptions::ExportGml;
    }

    if (prettyPrintBefore ||prettyPrintAfter)
    {
      LOG(INFO) << "Will export textual model view for configurations";
      options |= ManagerMonitorOptions::ExportPrettyPrint;
    }

    managerMonitor = make_shared<SystemModelManagerMonitor>(options);

    managerMonitor->ExportBasePath       (m_configuration->ManagerActivityFileBasePath());
    managerMonitor->GmlPrinterOptions    (m_configuration->GmlOptions());
    managerMonitor->PrettyPrinterOptions (m_configuration->PrettyPrintOptions());
  }

  return managerMonitor;
}
//
//  End of: MastEnvironment_impl::MakeManagerMonitor
//---------------------------------------------------------------------------



//! Parses options - from list of command line arguments
//!
//! @param arguments  Command line arguments (first one is application name)
//!
//! @return True when parsing is successful, false when help is requested (and nothing more)
bool MastEnvironment_impl::ParseOptions (vector<string> arguments)
{
  LOG(INFO) << "Parsing MAST options";
  if (!m_configuration)
  {
    m_configuration = make_shared<MastConfiguration>();
    if (m_unitTestsContext)
    {
      m_configuration->CommandLineParserOutput(make_shared<TCLAP::DiscardOutput>());
    }
  }

  m_configuration->AutomaticExit(false);  // To get an opportunity to log errors befores closing "properly"
  m_configuration->Update(arguments);

  if (m_configuration->ShouldExit())
  {
    if (m_configuration->RequestHelp())
    {
      return false;
    }
    throw std::runtime_error("Failed to parse user options");
  }

  if (!m_unitTestsContext)
  {
    ConfigureLogger();
    LOG(DEBUG) << "Logger (re)configured";
  }
  LOG(INFO) << "MAST options parsed";
  return true;
}
//
//  End of: MastEnvironment_impl::ParseOptions
//---------------------------------------------------------------------------



//! Save GML/Text view of parsed SIT model
//!
//! @note Only if options are enabled
void MastEnvironment_impl::ReportParsedModel ()
{

  // ---------------- SIT
  //
  if (m_configuration->SitExport())
  {
    auto sit = SIT_Printer::MakeSIT(Startup::sm_systemModel->Root());

    ofstream os(m_configuration->SitExportFilePath());
    os << sit;
    os.flush();
  }

  // ---------------- GML
  //
  if (    m_configuration->GmlPrinting()
      &&  IsSet(m_configuration->GmlReportMoments(), ReportMoments::AfterModelParsing))
  {
    auto graph = GmlPrinter::Graph(Startup::sm_systemModel->Root(),
                                   m_configuration->GmlGraphName(),
                                   m_configuration->GmlOptions());

    ofstream os(m_configuration->GmlFilePath());
    os << graph;
    os.flush();
  }

  // ---------------- Pretty Print
  //
  if (   m_configuration->PrettyPrinting()
      && IsSet(m_configuration->PrettyPrintReportMoments(), ReportMoments::AfterModelParsing))
  {
    auto prettyPrint = PrettyPrinter::PrettyPrint(Startup::sm_systemModel->Root(),
                                                  m_configuration->PrettyPrintOptions());

    ofstream os(m_configuration->PrettyPrintFilePath());
    os << prettyPrint;
    os.flush();
  }
}
//
//  End of: MastEnvironment_impl::ReportParsedModel
//---------------------------------------------------------------------------



//! Rethrows exception from one PDL application that throws an exception on its own thread
//!
void MastEnvironment_impl::RethrowApplicationsExceptions ()
{
  const auto& applicationExceptions = Startup::sm_manager->ApplicationsExceptions();
  if (!applicationExceptions.empty())
  {
    auto pException = applicationExceptions.front();
    LOG(DEBUG) << "Rethrowing PDL application exception (1st one in order the application where started)";
    std::rethrow_exception(pException);
  }
}
//
//  End of: MastEnvironment_impl::RethrowApplicationsExceptions
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

  RethrowApplicationsExceptions();
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
  if (ParseOptions(arguments))
  {
    LoadPlugins();
    CreateSystemModel();
    CreateManager();
    CreateApplications();
    Start();
  }
}
//
//  End of: MastEnvironment_impl::Start
//---------------------------------------------------------------------------

//===========================================================================
// End of MastEnvironment_impl.cpp
//===========================================================================
