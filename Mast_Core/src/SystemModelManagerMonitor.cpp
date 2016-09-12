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
  if (m_monitorAfterConfiguration)
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
  if (m_monitorBeforeConfiguration)
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
  if (m_monitorAppThreadCreation)
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
  if (!m_gmlPrinterBasePath.empty())
  {
    // ---------------- Make graph
    //
    GmlPrinter printer("", GmlPrinterOptions::Std);

    root.Accept(printer);
    auto graph = printer.Graph();

    // ---------------- Save graph to file
    //
    auto path = MakeFilePath(m_gmlPrinterBasePath, step);
    ofstream os(path);
    os << graph;
    os.flush();
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
string SystemModelManagerMonitor::MakeFilePath (string_view basePath, string_view step)
{
  if (basePath.empty())
  {
    basePath = "SystemModel";
  }

  ostringstream os;
  os << basePath << "_" << step << "_"<< m_dataCyclesCount << ".gml";

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
