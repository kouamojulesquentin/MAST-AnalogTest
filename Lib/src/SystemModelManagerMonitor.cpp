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
#include "GmlPrinterVisitor.hpp"
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



//! Creates a gml view of the SystemModel root node
//!
void SystemModelManagerMonitor::ExportGml (string_view step, ParentNode& root)
{
  if (!m_gmlPrinterBasePath.empty())
  {
    // ---------------- Make graph
    //
    GmlPrinterVisitor printer("", GmlPrinterOptions::Std);

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
  LOG(INFO) << "Starting a data cycle: " << m_dataCyclesCount;
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
