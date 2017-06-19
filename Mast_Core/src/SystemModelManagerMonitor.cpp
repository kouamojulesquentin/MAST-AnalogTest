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
#include "BinaryVector.hpp"
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
    ExportGml         ("After", root);
    ExportPrettyPrint ("After", root);
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
    ExportGml         ("Before", root);
    ExportPrettyPrint ("Before", root);
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
    LOG(INFO) << WrapDebugName(debugName) << "Creating application thread" << NodeInfos(topNode, true);
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
    auto graph = GmlPrinter::Graph(root, "Mast", m_gmlPrinterOptions);

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
    auto prettyPrint = PrettyPrinter::PrettyPrint(root, m_prettyPrinterOptions);

    // ---------------- Save graph to file
    //
    SaveToFile(prettyPrint, m_exportBasePath, "txt", step);
  }
}
//
//  End of: SystemModelManagerMonitor::ExportGml
//---------------------------------------------------------------------------


//! Logs a debug message
//!
//! @param message  A message to log
//!
void SystemModelManagerMonitor::LogDebug (string_view message)
{
  if (!message.empty() && IsSet(m_options, ManagerMonitorOptions::InternalDebug))
  {
    LOG(DEBUG) << message;
  }
}
//
//  End of: SystemModelManagerMonitor::LogDebug
//---------------------------------------------------------------------------


//! Logs a debug message (in relation with a node)
//!
//! @param message    A message to log
//! @param node       The node related to the message
//! @param debugName  Application debug name
//!
void SystemModelManagerMonitor::LogDebug (string_view message, const SystemModelNode& node, string_view debugName)
{
  if (!message.empty() && IsSet(m_options, ManagerMonitorOptions::InternalDebug))
  {
    LOG(DEBUG) << WrapDebugName(debugName) << message << NodeInfos(node);
  }
}
//
//  End of: SystemModelManagerMonitor::LogDebug
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
string SystemModelManagerMonitor::NodeInfos (const SystemModelNode& node, bool ignoreVerboseMode) const
{
  string infos;

  if (ignoreVerboseMode || IsSet(m_options, ManagerMonitorOptions::Verbose))
  {
    infos.append(" (app top node: '").append(node.Name()).append("'");

    if (IsSet(m_options, ManagerMonitorOptions::InternalDebug))
    {
      infos.append(" [id: ").append(std::to_string(node.Identifier())).append("])");
    }

    infos.append(")");
  }

  return infos;
}
//
//  End of: SystemModelManagerMonitor::NodeInfos
//---------------------------------------------------------------------------


//! Returns formatted node path message (when not empty)
//!
string SystemModelManagerMonitor::NodePath (string_view nodePath) const
{
  if (nodePath.empty())
  {
    return "";
  }
  return " on node '" + nodePath + "'";
}
//
//  End of: SystemModelManagerMonitor::NodePath
//---------------------------------------------------------------------------

//! Logs a PDL command
//!
//! @param message    A PDL command message
//! @param nodePath   Path relative to application top node (or prefix) for which the PDL command is issued
//! @param node       Application associated top node
//! @param debugName  Application debug name
//!
void SystemModelManagerMonitor::PDLCommand (string_view message, string_view nodePath, const ParentNode& node, string_view debugName)
{
  if (!message.empty() && IsSet(m_options, ManagerMonitorOptions::PDLCommands))
  {
    LOG(INFO) << WrapDebugName(debugName) << message
              << NodePath(nodePath)
              << NodeInfos(node);
  }
}
//
//  End of: SystemModelManagerMonitor::LogDebug
//---------------------------------------------------------------------------


//! Logs a PDL command with associated BinaryVector value
//!
//! @param message    A PDL command message
//! @param nodePath   Path relative to application top node (or prefix) for which the PDL command is issued
//! @param value      The PDL command value
//! @param node       Application associated top node
//! @param debugName  Application debug name
//!
void SystemModelManagerMonitor::PDLCommand (string_view message, string_view nodePath, const BinaryVector& value, const ParentNode& node, string_view debugName)
{
  if (!message.empty() && IsSet(m_options, ManagerMonitorOptions::PDLCommands))
  {
    LOG(INFO) << WrapDebugName(debugName) << message
              << NodePath(nodePath)
              << " with value: " << value.DataAsMixString()
              << NodeInfos(node);
  }
}
//
//  End of: SystemModelManagerMonitor::LogDebug
//---------------------------------------------------------------------------


//! Resets data cyles counter
//!
void SystemModelManagerMonitor::Reset ()
{
  m_dataCyclesCount = 0;
  if (IsSet(m_options, ManagerMonitorOptions::DataCycles))
  {
    LOG(INFO) << "Reseting data cycles counter";
  }
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


//! Monitor start of SystemModelManager background thread
void SystemModelManagerMonitor::StartBackgroundThread ()
{
  if (IsSet(m_options, ManagerMonitorOptions::DataCycles))
  {
    LOG(INFO) << "Started SystemModelManager data cycles loop background thread";
  }
}
//
//  End of: SystemModelManagerMonitor::StartDataCycles
//---------------------------------------------------------------------------


//! Monitor start of a new data cycle
void SystemModelManagerMonitor::StartDataCycle ()
{
  ++m_dataCyclesCount;
  if (IsSet(m_options, ManagerMonitorOptions::DataCycles))
  {
    LOG(INFO) << "Starting data cycle: " << m_dataCyclesCount;
  }
}
//
//  End of: SystemModelManagerMonitor::StartDataCycles
//---------------------------------------------------------------------------

//! Monitor start of a series of new data cycles
void SystemModelManagerMonitor::StartDataCycles ()
{
  if (IsSet(m_options, ManagerMonitorOptions::DataCycles))
  {
    LOG(INFO) << "Starting a series of data cycles";
  }
}
//
//  End of: SystemModelManagerMonitor::StartDataCycles
//---------------------------------------------------------------------------





//===========================================================================
// End of SystemModelManagerMonitor.cpp
//===========================================================================
