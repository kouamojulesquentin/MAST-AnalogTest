//===========================================================================
//                           SystemModelAdapter_CPP.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter_CPP.cpp
//!
//! Implements 'CPP' language API to system model
//===========================================================================

#include "SystemModelAdapter_CPP.hpp"
#include "AppFunctionAndNodePath_CPP.hpp"
#include "AppFunctionAndName_CPP.hpp"
#include "AppFunctionAndNode.hpp"
#include "AppFunctionNameAndNode.hpp"
#include "SystemModelManager.hpp"
//+#include "Commons_C.hpp"
#include "Startup.hpp"
#include "Utility.hpp"
#include "Session.hpp"
#include "SIT_reader.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <memory>
#include <fstream>
#include <algorithm>

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::string;
using std::experimental::string_view;
using std::dynamic_pointer_cast;
using namespace std::string_literals;
using namespace mast;


namespace CPP_API_IMPL
{
  vector<AppFunctionNameAndNode>        LoadSystemModel      (string_view                           filePath);
  vector<AppFunctionAndNode>            GetFunctionsAndNodes (const vector<AppFunctionAndNodePath>& functionsAndPaths);
  void                                  CreateApplications   (const vector<AppFunctionAndNode>&     functionsAndNodes);
  void                                  RunMast              (const vector<AppFunctionAndNode>&     functionsAndNodes, RunMastOptions options);
  vector<AppFunctionAndNode>            GetFunctionsAndNodes (const vector<AppFunctionAndName>&     appFunctionsAndNames,
                                                              const vector<AppFunctionNameAndNode>& appFunctionNameAndNodes);

  inline auto GetAndCheckManager()
  {
    auto manager = Startup::GetManager();
    CHECK_VALUE_NOT_NULL(manager, "Mast library has not been properly initialized");\
    return manager;
  }
} // End of CPP_API_IMPL namespace

//! Registers applications functions with their associated node to the System Model Manager
//!
//! @param functionsAndNodes  List of application function with their associated node
//!
void CPP_API_IMPL::CreateApplications (const vector<AppFunctionAndNode>& functionsAndNodes)
{
  auto sm      = mast::Startup::GetSystemModel();
  auto manager = mast::Startup::GetManager();

  for (const auto& functionAndNode : functionsAndNodes)
  {
    CHECK_VALUE_NOT_NULL(functionAndNode.node, "Cannot create application " + Utility::IfNotEmpty_SingleQuoteAndSuffixWithSpace(functionAndNode.appName));

    manager->CreateApplicationThread(functionAndNode.node, functionAndNode.appFunction, functionAndNode.appName);
  }
}
//
//  End of: mast::CreateApplications
//---------------------------------------------------------------------------


//! Registers applications functions with their associated node to the System Model Manager
//!
//! @param functionsAndPaths  Associate some application function with a node in the system model
//!
vector<AppFunctionAndNode> CPP_API_IMPL::GetFunctionsAndNodes (const vector<AppFunctionAndNodePath>& functionsAndPaths)
{
  auto functionAndNodes = vector<AppFunctionAndNode>();
  auto rootNode         = mast::Startup::GetSystemModel()->Root();

  CHECK_VALUE_NOT_NULL(rootNode, "Internal error: SystemModel must be constructed before calling GetFunctionsAndNodes");

  for (const auto& nextAssociation : functionsAndPaths)
  {
    CHECK_PARAMETER_NOT_EMPTY(nextAssociation.topNodePath, "An empty node path is not valid to associate an application");

    auto foundNode = rootNode->FindNode(nextAssociation.topNodePath);
    auto topNode   = std::dynamic_pointer_cast<ParentNode>(foundNode);

    CHECK_VALUE_NOT_NULL(topNode,   "Cannot create application "                     + Utility::IfNotEmpty_SingleQuoteAndSuffixWithSpace(nextAssociation.appName)
                                  + "because cannot find an actual node with path: " + Utility::SingleQuote(nextAssociation.topNodePath));

    functionAndNodes.emplace_back(nextAssociation.appFunction, topNode, nextAssociation.appName);
  }
  return functionAndNodes;
}
//
//  End of: GetFunctionsAndNodes
//---------------------------------------------------------------------------


//! Associates a function and an node from association from function and a name an association from a name to a node
//!
//! @param appFunctionsAndNames     List associations of function and their name
//! @param appFunctionNameAndNodes  List associations of function name and a node
//!
vector<AppFunctionAndNode> CPP_API_IMPL::GetFunctionsAndNodes (const vector<AppFunctionAndName>&     appFunctionsAndNames,
                                                               const vector<AppFunctionNameAndNode>& appFunctionNameAndNodes)
{
  auto functionAndNodes = vector<AppFunctionAndNode>();

  for (const auto& functionNameAndNode : appFunctionNameAndNodes)
  {
    CHECK_PARAMETER_NOT_EMPTY (functionNameAndNode.appName, "Application function name cannot be empty");
    CHECK_PARAMETER_NOT_NULL  (functionNameAndNode.node,    "Application cannot be associated to 'no node'");

    auto it    = std::find_if(appFunctionsAndNames.begin(),
                              appFunctionsAndNames.end(),
                              [=](const AppFunctionAndName& item) { return item.appName == functionNameAndNode.appName; });

    if (it != appFunctionsAndNames.cend())
    {
      functionAndNodes.emplace_back(it->appFunction, functionNameAndNode.node, functionNameAndNode.appName);
    }
    else
    {
      LOG(WARNING) << "Found no registered function for name: '" << functionNameAndNode.appName
                   << "' found in SIT file at line: "            << functionNameAndNode.SIT_line
                   << " (be aware that the association is case sensitive)";
    }
  }

  return functionAndNodes;
}
//
//  End of: GetFunctionsAndNodes
//---------------------------------------------------------------------------


//! Loads model from "SIT" file and set it as root of SystemModel
//!
vector<AppFunctionNameAndNode> CPP_API_IMPL::LoadSystemModel (string_view filePath)
{
  CHECK_PARAMETER_NOT_EMPTY(filePath, "Cannot create system model without a valid path for its description (SIT file)");
  CHECK_FILE_EXISTS(filePath);

  auto sm     = Startup::GetSystemModel();

  auto reader = SIT::SIT_Reader(sm);

  reader.parse(filePath);

  auto topNode = dynamic_pointer_cast<ParentNode>(reader.parsed_sut);
  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: " + filePath);
  sm->ReplaceRoot(topNode, false);

  auto namesAndNodes = reader.namesAndNodes;

  return namesAndNodes;
}
//
//  End of: LoadSystemModel
//---------------------------------------------------------------------------

//! Runs Mast til applications terminates
//!
//! @param functionsAndNodes  Associate some application function with a node in the system model
//! @param options            Options for running MAST
//!
//! @note Mast must have been initialized and system model been created beforehand
//!
void CPP_API_IMPL::RunMast (const vector<AppFunctionAndNode>& functionsAndNodes, RunMastOptions options)
{

  if (IsSet(options, RunMastOptions::CheckModel))
  {
    CheckSystemModel();
  }

  if (IsSet(options, RunMastOptions::PrintModelGraph))
  {
    PrintModelGraph();
  }

  auto manager = mast::Startup::GetManager();

  if (    IsSet(options, RunMastOptions::LogManagerActivity)
      && !manager->Monitor()
     )
  {
    auto monitor = make_shared<SystemModelManagerMonitor>();
    monitor->Options(ManagerMonitorOptions::Std);
    manager->Monitor(monitor);
  }

  CreateApplications(functionsAndNodes);

  manager->Start();
  manager->StartCreatedApplicationThreads();
  manager->WaitForApplicationsEnd();
  manager->Stop();
}
//
//  End of: mast::RunMast
//---------------------------------------------------------------------------


//! Check SystemModel coherency, logging check result and throwing runtime exception in case of error
//!
void mast::CheckSystemModel ()
{
  auto sm          = Startup::GetSystemModel();
  auto checkResult = sm->Check();

  return;
  if (!checkResult.HasIssues())
  {
    if (checkResult.infosCount == 0)
    {
      LOG(INFO) << "System model is correct";
    }
    else
    {
      LOG(INFO) << "System model infos: " << checkResult.infos;
    }
  }
  else
  {
    LOG(WARNING) << checkResult.MakeReport();

    if (checkResult.HasErrors())
    {
      PrintModelGraph();
      THROW_RUNTIME_ERROR("checkResult.MakeReport()");
    }
  }
}
//
//  End of: CheckSystemModel
//---------------------------------------------------------------------------


//!< Cleans up Mast library
//!<
//!< @note  To start using Mast again (e.g. with a new model), one must call InitializeMast again
//!<
void mast::CleanupMast ()
{
  LOG(INFO) << "Cleaning MAST Core library";
  mast::Startup::ForgetManager();
  mast::Startup::ForgetSystemModel();
}
//
//  End of: mast::CleanupMast
//---------------------------------------------------------------------------





//! Registers applications functions with their associated node to the System Model Manager
//!
//! @param appFunctionsAndPaths  Associate some application functions with a node identified by its path in the system model
//!
void mast::CreateApplications (const vector<AppFunctionAndNodePath>& appFunctionsAndPaths)
{
  auto functionsAndNodes = CPP_API_IMPL::GetFunctionsAndNodes(appFunctionsAndPaths);
  CPP_API_IMPL::CreateApplications(functionsAndNodes);
}
//
//  End of: mast::CreateApplications
//---------------------------------------------------------------------------

//! Starts up mast library, building model using specified file
//!
//! @note This function is not thread safe: It must be call by only one thread prior to any usage of mast library
//!       This function must be called only after main has been call by the runtime system
//!
void mast::InitializeMast (string_view modelFilePath)
{
  // Those 2 lines do the initialization
  /* unused */ Startup::GetSystemModel();
  /* unused */ Startup::GetManager();

  CPP_API_IMPL::LoadSystemModel(modelFilePath);
}
//
//  End of: mast::InitializeMast
//---------------------------------------------------------------------------
//! Starts up mast library, without building the model
//!
//! @note This function is not thread safe: It must be call by only one thread prior to any usage of mast library
//!       This function must be called only after main has been call by the runtime system
//!
void mast::InitializeMast ()
{
  // Those 2 lines do the initialization
  /* unused */ Startup::GetSystemModel();
  /* unused */ Startup::GetManager();

}
//
//  End of: mast::InitializeMast
//---------------------------------------------------------------------------


//! Exports GML formated graph of system model
//!
//! @param filePath   Gml file path
//!
void mast::PrintModelGraph (const string& filePath)
{
  std::ofstream os(filePath);
  auto          sm = Startup::GetSystemModel();

  os << GmlPrinter::Graph(sm->Root(), "", GmlPrinterOptions::Std | GmlPrinterOptions::ShowProtocol);
}
//
//  End of: mast::PrintModelGraph
//---------------------------------------------------------------------------


//! Creates a system model from configuration file, then runs Mast till applications terminates
//!
void mast::RunMast (string_view modelFilePath, const vector<AppFunctionAndNodePath>& appFunctionsAndPaths, RunMastOptions options)
{
  CHECK_PARAMETER_NOT_EMPTY(modelFilePath, "Cannot run Mast without a valid path for system model (SIT file)");
  CHECK_FILE_EXISTS(modelFilePath);

  Session session;
  CPP_API_IMPL::LoadSystemModel(modelFilePath);
  auto functionsAndNodes = CPP_API_IMPL::GetFunctionsAndNodes(appFunctionsAndPaths);
  CPP_API_IMPL::RunMast(functionsAndNodes, options);
}
//
//  End of: mast::RunMast
//---------------------------------------------------------------------------


//! Creates a system model from configuration file, then runs Mast till applications terminates
//! Warning, openOCD may cause a segfault
void mast::RunMast (string_view modelFilePath, const vector<AppFunctionAndName>& appFunctionsAndNames, RunMastOptions options)
{
  CHECK_PARAMETER_NOT_EMPTY(modelFilePath, "Cannot run Mast without a valid path for system model (SIT file)");
  CHECK_FILE_EXISTS(modelFilePath);

  auto initializeLogger = IsSet(options, RunMastOptions::LogManagerActivity);
  Session session(initializeLogger);

  auto namesAndNodes     = CPP_API_IMPL::LoadSystemModel(modelFilePath); 
  auto functionsAndNodes = CPP_API_IMPL::GetFunctionsAndNodes(appFunctionsAndNames, namesAndNodes);

  CPP_API_IMPL::RunMast(functionsAndNodes, options);
}
//
//  End of: mast::RunMast
//---------------------------------------------------------------------------


//! Runs Mast till applications terminates
//!
//! @param appFunctionsAndPaths Associations of application function and node path
//! @param options
//!
//! @note Mast must have been initialized and system model been created beforehand
//!
void mast::RunMast (const vector<AppFunctionAndNodePath>& appFunctionsAndPaths, RunMastOptions options)
{
  CHECK_PARAMETER_NOT_NULL(mast::Startup::GetSystemModel()->Root(), "System model must have been constructed before running Mast");

  auto functionsAndNodes = CPP_API_IMPL::GetFunctionsAndNodes(appFunctionsAndPaths);
  CPP_API_IMPL::RunMast(functionsAndNodes, options);
}
//
//  End of: mast::RunMast
//---------------------------------------------------------------------------


//! Starts System Model Manager
//!
void mast::Start ()
{
  auto manager = CPP_API_IMPL::GetAndCheckManager();
  manager->Start();
}
//
//  End of: mast::Start
//---------------------------------------------------------------------------


//! Stops System Model Manager
//!
void mast::Stop ()
{
  auto manager = CPP_API_IMPL::GetAndCheckManager();
  manager->Stop();
}
//
//  End of: mast::Stop
//---------------------------------------------------------------------------


//! Waits (blocks) until all application threads terminate (on their own or by a mechanism unknown to mast manager)
//!
void mast::WaitForApplicationsEnd ()
{
  auto manager = CPP_API_IMPL::GetAndCheckManager();
  manager->WaitForApplicationsEnd();
}
//
//  End of: mast::WaitForApplicationsEnd
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelAdapter_CPP.cpp
//===========================================================================
