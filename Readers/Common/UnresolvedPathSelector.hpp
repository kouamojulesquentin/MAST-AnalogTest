//===========================================================================
//                           UnresolvedPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UnresolvedPathSelector.hpp
//!
//! Declares class UnresolvedPathSelector
//!
//===========================================================================

#ifndef UNRESOLVEDPATHSELECTOR_H__2DAFEAC6_102A_4DF1_ED92_8F9736E1F0D__INCLUDED_
  #define UNRESOLVEDPATHSELECTOR_H__2DAFEAC6_102A_4DF1_ED92_8F9736E1F0D__INCLUDED_

#include "PathSelector.hpp"
#include "Utility.hpp"
#include <vector>
#include <memory>

namespace mast
{
class SystemModelNode;
class Register;
class BinaryVector;

//! Defines an Unresolved default path selector to be used during SystemModel construction when associated register(s) are not yet known
//!
//! @note  Should be used only during SystemModel construction.
//!        When the SystemModel is complete no UnresolvedPathSelector should remain.
//!
class UnresolvedPathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~UnresolvedPathSelector() = default;
  UnresolvedPathSelector() = default;

  std::experimental::string_view KindName() const { return "Unresolved"; }; //!< Returns readable type of selector

  std::string DebugSelectorInfo(bool /*onlyProperties*/) const { return "UnresolvedPathSelector"; };

  uint32_t ActiveCount()     const override { CHECK_FAILED(BadUsageMessage); };
  uint32_t SelectablePaths() const override { CHECK_FAILED(BadUsageMessage); };

  bool IsActive            (uint32_t /*pathIdentifier*/) const override { CHECK_FAILED(BadUsageMessage); };
  bool IsSelected          (uint32_t /*pathIdentifier*/) const override { CHECK_FAILED(BadUsageMessage); };
  bool IsSelectedAndActive (uint32_t /*pathIdentifier*/) const override { CHECK_FAILED(BadUsageMessage); };

  void Accept              (SystemModelVisitor& /*visitor*/) override   { /* Does nothing but be usable by PrettyPrinter */ };

  std::shared_ptr<const Register> AssociatedRegister() const override { CHECK_FAILED(BadUsageMessage); };

  const BinaryVector& SelectionValue (uint32_t /*pathIdentifier*/) const  override{ CHECK_FAILED(BadUsageMessage); };

  void Select   (uint32_t /*pathIdentifier*/) override { CHECK_FAILED(BadUsageMessage); };
  void Deselect (uint32_t /*pathIdentifier*/) override { CHECK_FAILED(BadUsageMessage); };

  // ---------------- Private Fields
  //
  private:
  static constexpr auto BadUsageMessage = "UnresolvedPathSelector is only a place holder that must be replaced during SystemModel construction";
};
//
//  End of UnresolvedPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined UNRESOLVEDPATHSELECTOR_H__2DAFEAC6_102A_4DF1_ED92_8F9736E1F0D__INCLUDED_

//===========================================================================
// End of UnresolvedPathSelector.hpp
//===========================================================================
