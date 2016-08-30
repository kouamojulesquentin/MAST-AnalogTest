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
