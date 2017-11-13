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

using namespace mast;
using std::string;
using std::to_string;
using std::experimental::string_view;
using std::ostringstream;

//! Builds up a CheckResult from currently selected issues
//!
CheckResult Checker::MakeCheckResult ()
{
  CheckResult result;

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



//===========================================================================
// End of Checker.cpp
//===========================================================================
