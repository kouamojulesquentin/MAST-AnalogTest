//===========================================================================
//                           SystemModelCheckerBase.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckerBase.hpp
//!
//! Declares SystemModelCheckerBase class
//!
//===========================================================================

#ifndef SYSTEMMODELCHECKERBASE_H__F8958B58_85ED_4CD7_5EB5_76A4CD84F835__INCLUDED_
  #define SYSTEMMODELCHECKERBASE_H__F8958B58_85ED_4CD7_5EB5_76A4CD84F835__INCLUDED_

#include "Checker.hpp"

#include <experimental/string_view>

namespace mast
{
class SystemModelNode;

//! Base helper class for system model checking
//!
class SystemModelCheckerBase : public Checker
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SystemModelCheckerBase() = default;

  // ---------------- Protected Methods
  //
  protected:
  SystemModelCheckerBase() = default;

  using string_view = std::experimental::string_view;

  using Checker::ReportInfo;
  using Checker::ReportWarning;
  using Checker::ReportError;

  void ReportInfo    (const SystemModelNode& node, string_view message);
  void ReportWarning (const SystemModelNode& node, string_view message);
  void ReportError   (const SystemModelNode& node, string_view message);

  static std::ostringstream& Stream(std::ostringstream& os, string_view header, const SystemModelNode& node);
  static std::ostringstream& Stream(std::ostringstream& os, const SystemModelNode& node) { return Stream(os, "", node); }
};
//
//  End of SystemModelCheckerBase class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined SYSTEMMODELCHECKERBASE_H__F8958B58_85ED_4CD7_5EB5_76A4CD84F835__INCLUDED_
//===========================================================================
// End of SystemModelCheckerBase.hpp
//===========================================================================
