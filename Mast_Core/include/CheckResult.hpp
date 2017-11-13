//===========================================================================
//                           CheckResult.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file CheckResult.hpp
//!
//! Declares CheckResult class
//!
//===========================================================================

#ifndef CHECKRESULT_H__D455A6A2_7741_4A12_A385_9BADE5D4E30__INCLUDED_
  #define CHECKRESULT_H__D455A6A2_7741_4A12_A385_9BADE5D4E30__INCLUDED_
#include "Mast_Core_export.hpp"

#include <string>
#include <sstream>
#include <cstdint>


namespace mast
{
//! Collects warning and error while SystemModel is being checked
//!
class MAST_CORE_EXPORT CheckResult final
{
  public:
  ~CheckResult() = default;
  CheckResult()  = default;

  //! Merges other results into this
  //!
  void Merge(const CheckResult& other);

  //! Returns true when it equals to another CheckResult
  //!
  bool operator==(const CheckResult& rhs) const
  {
    return    (infosCount    == rhs.infosCount)
           && (warningsCount == rhs.warningsCount)
           && (errorsCount   == rhs.errorsCount)
           && (infos         == rhs.infos)
           && (warnings      == rhs.warnings)
           && (errors        == rhs.errors);
  }

  //! Returns true when it differs from another CheckResult
  //!
  bool operator!=(const CheckResult& rhs) const
  {
    return !operator==(rhs);
  }

  bool HasIssues()   const { return (warningsCount != 0)  || (errorsCount != 0); }
  bool HasWarnings() const { return (warningsCount != 0); }
  bool HasErrors()   const { return (errorsCount   != 0); }

  std::string MakeReport()             const; //!< Returns a string representing the check result
  std::string MakeCompactErrorReport() const; //!< Returns a string representing only error that stands on a single line when there is just one error
  std::string IssuesReport()           const; //!< Returns a string representing the check result only when there are issues
  std::string InformativeReport()      const; //!< Returns a string representing the check result only when there is something to report

  static const CheckResult None; //!< Instance to compare for no error (very useful in unit tests along with traits to show the issues)

  uint32_t    infosCount    = 0; //!< Total number of collected info messages
  uint32_t    warningsCount = 0; //!< Total number of collected warnings messages
  uint32_t    errorsCount   = 0; //!< Total number of collected errors messages
  std::string infos;             //!< Collected infos
  std::string warnings;          //!< Collected warnings
  std::string errors;            //!< Collected errors
};
//
//  End of CheckResult class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined CHECKRESULT_H__D455A6A2_7741_4A12_A385_9BADE5D4E30__INCLUDED_

//===========================================================================
// End of CheckResult.hpp
//===========================================================================
