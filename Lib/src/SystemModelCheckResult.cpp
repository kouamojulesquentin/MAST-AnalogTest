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


//===========================================================================
// End of SystemModelCheckResult.cpp
//===========================================================================
