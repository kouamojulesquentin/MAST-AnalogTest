//===========================================================================
//                           DefaultBinaryPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultBinaryPathSelector.hpp
//!
//! Declares class DefaultBinaryPathSelector
//!
//===========================================================================


#ifndef DEFAULTBINARYPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_
  #define DEFAULTBINARYPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

#include "BinaryVector.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "Utility.hpp"
#include <vector>
#include <memory>

namespace mast
{

class SystemModelNode;
class Register;

//! Defines a default path selector using binary encoding
//!
//! @note Path identifier are one based (in range [1..path_count])
//! @note Internal table contains a reserved slot at index zero
class MAST_CORE_EXPORT DefaultBinaryPathSelector : public DefaultTableBasedPathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultBinaryPathSelector() = default;
  DefaultBinaryPathSelector()  = delete;
  DefaultBinaryPathSelector(std::shared_ptr<Register> associatedRegister,  uint32_t pathsCount, SelectorProperty properties = SelectorProperty::Binary_Default);
  DefaultBinaryPathSelector(const VirtualRegister&    associatedRegisters, uint32_t pathsCount, SelectorProperty properties = SelectorProperty::Binary_Default);


  //! Returns minimal bits count a register should have to drive a mux for number of path
  //!
  static uint32_t RegWidthForPathCount(uint32_t pathCount, bool canSelectNone);

  using TablesType = DefaultTableBasedPathSelector::TablesType;

  //! Returns the associated register expected initial value for a path count and selector properties
  //!
  static BinaryVector AssociatedRegisterInitialValue(uint32_t pathsCount, SelectorProperty properties);


  static TablesType   CreateSelectTable                       (uint32_t   registerLength,  uint32_t pathsCount, SelectorProperty properties);
  static TablesType   CreateDeselectTable                     (uint32_t   registerLength,  uint32_t pathsCount, SelectorProperty properties);

  virtual std::experimental::string_view KindName() const override { return "Binary"; }; //!< Returns readable type of selector

  // ---------------- Protected Methods
  //
  protected:
  static void CheckRegisterLength (uint32_t registerLength, uint32_t pathsCount, bool canSelectNone);
};
//
//  End of DefaultBinaryPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DEFAULTBINARYPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

//===========================================================================
// End of DefaultBinaryPathSelector.hpp
//===========================================================================
