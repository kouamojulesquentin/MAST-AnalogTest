//===========================================================================
//                           DefaultNHotPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultNHotPathSelector.hpp
//!
//! Declares class DefaultNHotPathSelector
//!
//===========================================================================

#ifndef DEFAULTNHOTPATHSELECTOR_H__C165E1FC_8827_47C3_86A1_92C4871BDDA0__INCLUDED_
  #define DEFAULTNHOTPATHSELECTOR_H__C165E1FC_8827_47C3_86A1_92C4871BDDA0__INCLUDED_

#include "BinaryVector.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "Utility.hpp"
#include <vector>
#include <memory>

namespace mast
{

class SystemModelNode;
class Register;

//! Defines a default path selector using N hot encoding (multiple path can be selected at the same time)
//!
//! @note Path identifier are one based (in range [1..path_count])
//! @note Internal table contains a reserved slot at index zero
//!
class DLL_EXPORT DefaultNHotPathSelector : public DefaultTableBasedPathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultNHotPathSelector() = default;
  DefaultNHotPathSelector()  = delete;
  DefaultNHotPathSelector(std::shared_ptr<Register> associatedRegister,
                          uint32_t                  pathsCount,
                          SelectorProperty          properties = SelectorProperty::N_Hot_Default);

  virtual bool IsActive            (uint32_t pathId) const override; //!< Returns true when the specified path is already active
  virtual bool IsSelected          (uint32_t pathId) const override; //!< Returns true when the specified path is selected
  virtual bool IsSelectedAndActive (uint32_t pathId) const override; //!< Returns true when the specified path is selected and active
  virtual void Select              (uint32_t pathId) override;       //!< Request activation of the specified path
  virtual void Deselect            (uint32_t pathId) override;       //!< Request deactivation of the specified path

  using TablesType = DefaultTableBasedPathSelector::TablesType;

  //! Returns the associated register expected initial value for a path count and selector properties
  //!
  static BinaryVector AssociatedRegisterInitialValue(uint32_t pathsCount, SelectorProperty properties);

  static TablesType CreateSelectTable   (uint32_t registerLength, uint32_t pathsCount, SelectorProperty properties);
  static TablesType CreateDeselectTable (uint32_t registerLength, uint32_t pathsCount, SelectorProperty properties);

  // ---------------- Protected Methods
  //
  protected:
  static void CheckRegisterLength (uint32_t registerLength, uint32_t pathsCount);

  // ---------------- Private  Fields
  //
  bool m_invertedBits = false;  //!< True when using negative logic (active low hardware)
};
//
//  End of DefaultNHotPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast





#endif  // not defined DEFAULTNHOTPATHSELECTOR_H__C165E1FC_8827_47C3_86A1_92C4871BDDA0__INCLUDED_
//===========================================================================
// End of DefaultNHotPathSelector.hpp
//===========================================================================
