//===========================================================================
//                           SystemModelManagerMonitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManagerMonitor.cpp
//!
//! Implements class SystemModelManagerMonitor
//!
//===========================================================================

#include "SystemModelManagerMonitor.hpp"
#include "ParentNode.hpp"
#include "GmlPrinter.hpp"
#include "PrettyPrinter.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <sstream>
#include <fstream>

using std::ofstream;
using std::ostringstream;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace mast;

//! Monitor state of SystemModel (from parentNode) after configuration
//!
//! @param root SystemModel root node
//!
void SystemModelManagerMonitor::AfterConfiguration (ParentNode& root)
{
  if (IsSet(m_options, ManagerMonitorOptions::AfterConfiguration))
  {
    LOG(INFO) << "Configuration: End";
    ExportGml("After", root);
  }
}
//
//  End of: SystemModelManagerMonitor::AfterConfiguration
//---------------------------------------------------------------------------


//! Monitor state of SystemModel (from parentNode) before configuration
//!
//! @param root SystemModel root node
//!
void SystemModelManagerMonitor::BeforeConfiguration (ParentNode& root)
{
  if (IsSet(m_options, ManagerMonitorOptions::BeforeConfiguration))
  {
    LOG(INFO) << "Configuration: Begin";
    ExportGml("Before", root);
  }
}
//
//  End of: SystemModelManagerMonitor::BeforeConfiguration
//---------------------------------------------------------------------------


//! Monitors creation of application thread
//!
//! @param topNode Top most node associated with the application thread
//!
void SystemModelManagerMonitor::CreateApplication (const ParentNode& topNode, string_view debugName)
{
  if (IsSet(m_options, ManagerMonitorOptions::AppThreadCreation))
  {
    LOG(INFO) << WrapDebugName(debugName) << "Creating application thread" << NodeInfos(topNode);
  }
}
//
//  End of: SystemModelManagerMonitor::BeforeConfiguration
//---------------------------------------------------------------------------


//! Creates a gml view of the SystemModel root node
//!
void SystemModelManagerMonitor::ExportGml (string_view step, ParentNode& root)
{
  if (  !m_exportBasePath.empty()
      && IsSet(m_options, ManagerMonitorOptions::ExportGml)
     )
  {
    // ---------------- Make graph
    //
    auto graph = GmlPrinter::Graph(root, "Mast");

    // ---------------- Save graph to file
    //
    SaveToFile(graph, m_exportBasePath, "gml", step);
  }
}
//
//  End of: SystemModelManagerMonitor::ExportGml
//---------------------------------------------------------------------------


//! Creates a textual representation of SystemModel, starting from root node
//!
void SystemModelManagerMonitor::ExportPrettyPrint (string_view step, ParentNode& root)
{
  if (  !m_exportBasePath.empty()
      && IsSet(m_options, ManagerMonitorOptions::ExportPrettyPrint)
     )
  {
    // ---------------- Make graph
    //
    auto prettyPrint = PrettyPrinter::PrettyPrint(root);

    // ---------------- Save graph to file
    //
    SaveToFile(prettyPrint, m_exportBasePath, "txt", step);
  }
}
//
//  End of: SystemModelManagerMonitor::ExportGml
//---------------------------------------------------------------------------


//! Always log message
//!
//! @param message  A message to log
//!
void SystemModelManagerMonitor::LogUncondionally (string_view message)
{
  if (!message.empty())
  {
    LOG(INFO) << message;
  }
}
//
//  End of: SystemModelManagerMonitor::LogUncondionally
//---------------------------------------------------------------------------


//! Always log message (in relation with a node)
//!
//! @param message  A message to log
//!
void SystemModelManagerMonitor::LogUncondionally (string_view message, const SystemModelNode& node, string_view debugName)
{
  if (!message.empty())
  {
    LOG(INFO) << WrapDebugName(debugName) << message << NodeInfos(node);
  }
}
//
//  End of: SystemModelManagerMonitor::LogUncondionally
//---------------------------------------------------------------------------



//! Builds a path using a base, a step and data cycle counter
//!
string SystemModelManagerMonitor::MakeFilePath (string_view basePath, string_view extension, string_view step) const
{
  if (basePath.empty())
  {
    basePath = "SystemModel";
  }

  ostringstream os;
  os << basePath << "_" << step << "_"<< m_dataCyclesCount << "." << extension;

  return os.str();
}
//
//  End of: SystemModelManagerMonitor::MakeFilePath
//---------------------------------------------------------------------------


//! Returns general information for a node
//!
string SystemModelManagerMonitor::NodeInfos (const SystemModelNode& node)
{
  return " (using top node '"s + node.Name() + "' [id: " + std::to_string(node.Identifier()) + "])";
}
//
//  End of: SystemModelManagerMonitor::NodeInfos
//---------------------------------------------------------------------------




//! Resets data cyles counter
//!
void SystemModelManagerMonitor::Reset ()
{
  m_dataCyclesCount = 0;
  LOG(INFO) << "Reseting data cycles counter";
}
//
//  End of: SystemModelManagerMonitor::Reset
//---------------------------------------------------------------------------



//! Saves some text associated with a step in SystemModelManager
//!
//! @param text       Text to save
//! @param basePath   Destination file base path (include base file name)
//! @param extension  File extension
//! @param step       Represent SystemModelManager step (with only characters authorized for paths)
//!
void SystemModelManagerMonitor::SaveToFile (string_view text, string_view basePath, string_view extension, string_view step) const
{
  auto path = MakeFilePath(basePath, extension, step);

  ofstream os(path);
  os << text;
  os.flush();
}
//
//  End of: SystemModelManagerMonitor::SaveToFile
//---------------------------------------------------------------------------



//! Monitor start of a new data cycle
void SystemModelManagerMonitor::StartDataCycle ()
{
  ++m_dataCyclesCount;
  LOG(INFO) << "Starting data cycle: " << m_dataCyclesCount;
}
//
//  End of: SystemModelManagerMonitor::StartDataCycles
//---------------------------------------------------------------------------

//! Monitor start of a series of new data cycles
void SystemModelManagerMonitor::StartDataCycles ()
{
  LOG(INFO) << "Starting a series of data cycles";
}
//
//  End of: SystemModelManagerMonitor::StartDataCycles
//---------------------------------------------------------------------------





//===========================================================================
// End of SystemModelManagerMonitor.cpp
//===========================================================================
