//===========================================================================
//                           Checker.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Checker.cpp
//!
//! Implements class Checker
//!
//===========================================================================

#include "Checker.hpp"
#include "SystemModelNode.hpp"

using namespace mast;
using std::string;
using std::to_string;
using std::experimental::string_view;
using std::shared_ptr;
using std::dynamic_pointer_cast;
//+using std::vector;
//+using std::set;
using std::ostringstream;

//! Builds up a SystemModelCheckResult from currently selected issues
//!
SystemModelCheckResult Checker::MakeCheckResult ()
{
  SystemModelCheckResult result;

  result.errors        = m_errors.str();
  result.warnings      = m_warnings.str();
  result.infos         = m_infos.str();

  result.errorsCount   = m_errorsCount;
  result.warningsCount = m_warningsCount;
  result.infosCount    = m_infosCount;

  return result;
}
//
//  End of: Checker::MakeCheckResult
//---------------------------------------------------------------------------


//! Reports an error, warning or info
//!
void Checker::Report (string_view message, uint32_t& counter, ostringstream& os)
{
  if (counter != 0)
  {
    os << std::endl;
  }

  os << "  - " << message;
  ++counter;
}
//
//  End of: Checker::ReportError
//---------------------------------------------------------------------------


//! Reports an error for a node
//!
void Checker::ReportError (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportError(os.str());
}
//
//  End of: Checker::ReportError
//---------------------------------------------------------------------------


//! Reports an info for a node
//!
void Checker::ReportInfo (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportInfo(os.str());
}
//
//  End of: Checker::ReportError
//---------------------------------------------------------------------------


//! Reports a warning for a node
//!
void Checker::ReportWarning (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportWarning(os.str());
}
//
//  End of: Checker::ReportWarning
//---------------------------------------------------------------------------



//! Helper that streams into os, an header, node name and identifier
//!
ostringstream& Checker::Stream (ostringstream& os, string_view header, const SystemModelNode& node)
{
  if (!header.empty())
  {
    os << header << " ";
  }

  os << node.TypeName() << " '" << node.Name() << "' (id: " << node.Identifier() << ")";
  return os;
}
//
//  End of: Checker::Stream
//---------------------------------------------------------------------------



//===========================================================================
// End of Checker.cpp
//===========================================================================
