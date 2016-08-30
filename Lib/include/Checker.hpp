//===========================================================================
//                           Checker.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Checker.hpp
//!
//! Declares Checker class
//!
//===========================================================================

#ifndef CHECKER_H__AF5E24EC_CA54_4DB4_54B6_4DAAC27D136B__INCLUDED_
  #define CHECKER_H__AF5E24EC_CA54_4DB4_54B6_4DAAC27D136B__INCLUDED_

#include "SystemModelCheckResult.hpp"

#include <memory>
#include <experimental/string_view>
#include <sstream>

namespace mast
{
class SystemModelNode;

//! Abstract class for checking stuff
//!
class DLL_EXPORT Checker
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Checker() = default;

  //! Runs assigned check
  //!
  virtual SystemModelCheckResult Check() = 0;

  //! Builds up a SystemModelCheckResult from currently detected issues
  //!
  SystemModelCheckResult  MakeCheckResult();

  // ---------------- Protected Methods
  //
  Checker() = default;

  using string_view = std::experimental::string_view;

  void Report        (string_view message, uint32_t& counter, std::ostringstream& os);
  void ReportInfo    (string_view message) { Report(message, m_infosCount,    m_infos);    }
  void ReportWarning (string_view message) { Report(message, m_warningsCount, m_warnings); }
  void ReportError   (string_view message) { Report(message, m_errorsCount,   m_errors);   }

  void ReportInfo    (const SystemModelNode& node, string_view message);
  void ReportWarning (const SystemModelNode& node, string_view message);
  void ReportError   (const SystemModelNode& node, string_view message);

  static std::ostringstream& Stream(std::ostringstream& os, string_view header, const SystemModelNode& node);
  static std::ostringstream& Stream(std::ostringstream& os, const SystemModelNode& node) { return Stream(os, "", node); }

  // ---------------- Private  Fields
  //
  std::ostringstream m_infos;             //!< Collects info messages
  std::ostringstream m_warnings;          //!< Collects warning messages
  std::ostringstream m_errors;            //!< Collects error messages
  uint32_t           m_infosCount    = 0; //!< Total number of collected info messages
  uint32_t           m_warningsCount = 0; //!< Total number of collected warnings messages
  uint32_t           m_errorsCount   = 0; //!< Total number of collected errors messages
};
//
//  End of Checker class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined CHECKER_H__AF5E24EC_CA54_4DB4_54B6_4DAAC27D136B__INCLUDED_
//===========================================================================
// End of Checker.hpp
//===========================================================================
