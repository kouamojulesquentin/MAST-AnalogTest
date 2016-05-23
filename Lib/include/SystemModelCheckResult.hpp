//===========================================================================
//                           SystemModelCheckResult.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckResult.hpp
//!
//! Declares SystemModelCheckResult class
//!
//===========================================================================


#ifndef SYSTEMMODELCHECKRESULT_H__FFBEA6CE_417E_43A0_32B5_EA2A1A0AF395__INCLUDED_
  #define SYSTEMMODELCHECKRESULT_H__FFBEA6CE_417E_43A0_32B5_EA2A1A0AF395__INCLUDED_

#include <string>
#include <sstream>
#include <cstdint>


namespace mast
{
//! Collects warning and error while SystemModel is being checked
//!
class SystemModelCheckResult final
{
  public:
  ~SystemModelCheckResult() = default;
  SystemModelCheckResult()  = default;

  bool HasIssues() const { return (warningsCount != 0) || (errorsCount != 0); }

  std::string MakeReport()
  {
    std::ostringstream os;

    auto reportCollected = [&os](auto header, auto count, auto& collected)
    {
      os << header << count << "):\n";
      if (!collected.empty())
      {
        os << collected << "\n\n";
      }
    };

    reportCollected("Errors   (", errorsCount,   errors);
    reportCollected("Warnings (", warningsCount, warnings);
    reportCollected("Infos    (", infosCount,    infos);

    return os.str();
  }

  uint32_t    infosCount    = 0; //!< Total number of collected info messages
  uint32_t    warningsCount = 0; //!< Total number of collected warnings messages
  uint32_t    errorsCount   = 0; //!< Total number of collected errors messages
  std::string infos;             //!< Collected infos
  std::string warnings;          //!< Collected warnings
  std::string errors;            //!< Collected errors
};
//
//  End of SystemModelCheckResult class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELCHECKRESULT_H__FFBEA6CE_417E_43A0_32B5_EA2A1A0AF395__INCLUDED_

//===========================================================================
// End of SystemModelCheckResult.hpp
//===========================================================================
