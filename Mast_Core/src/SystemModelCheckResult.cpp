//===========================================================================
//                           SystemModelCheckResult.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckResult.cpp
//!
//! Implements class SystemModelCheckResult
//!
//===========================================================================

#include "SystemModelCheckResult.hpp"
#include <experimental/string_view>

using std::string;
using std::experimental::string_view;
using std::ostringstream;
using namespace mast;

const SystemModelCheckResult SystemModelCheckResult::None;  // This is to compare with something that represent no errors


//! Returns a string representing the check result only when there is something to report
//!
//! @note This is useful for unit test for comparing with empty string
string SystemModelCheckResult::InformativeReport () const
{
  if (   (infosCount    != 0)
      || (warningsCount != 0)
      || (errorsCount   != 0)
     )
  {
    return MakeReport();
  }

  return string();
}
//
//  End of: SystemModelCheckResult::InformativeReport
//---------------------------------------------------------------------------



//! Returns a string representing the check result only when there are issues
//!
//! @note This is useful for unit test for comparing with empty string
string SystemModelCheckResult::IssuesReport () const
{
  if (HasIssues())
  {
    return MakeReport();
  }

  return string();
}
//
//  End of: SystemModelCheckResult::IssuesReport
//---------------------------------------------------------------------------

//! Returns a string representing only error that stands on a single line when there is just one error
//!
string SystemModelCheckResult::MakeCompactErrorReport() const
{
  ostringstream os;

  if (errorsCount == 0)
  {
    os << "No errors";
  }
  else if (errorsCount == 1)
  {
    os << errors;
  }
  else
  {
    os << "Errors(" << errorsCount << "): " << errors;
  }

  return os.str();
}


//! Returns a string representing the check result
//!
string SystemModelCheckResult::MakeReport() const
{
  ostringstream os;

  auto reportCollected = [&os](string_view header, uint32_t count, string_view collected)
  {
    os << header << count << "):\n";
    if (!collected.empty())
    {
      os << collected << "\n";
    }
  };

  reportCollected("Errors   (", errorsCount,   errors);
  reportCollected("Warnings (", warningsCount, warnings);
  reportCollected("Infos    (", infosCount,    infos);

  return os.str();
}




//! Merges other results into this
//!
void SystemModelCheckResult::Merge (const SystemModelCheckResult& other)
{
  infosCount    += other.infosCount;
  warningsCount += other.warningsCount;
  errorsCount   += other.errorsCount;

  if (!other.infos.empty())    infos.append    ("\n").append(other.infos);
  if (!other.warnings.empty()) warnings.append ("\n").append(other.warnings);
  if (!other.errors.empty())   errors.append   ("\n").append(other.errors);
}
//
//  End of: SystemModelCheckResult::Merge
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelCheckResult.cpp
//===========================================================================
