//===========================================================================
//                           BrocadeSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BrocadeSelector.hpp
//!
//! Declares class BrocadeSelector
//!
//===========================================================================

#ifndef BROCADESELECTOR_H__A8AFB3D9_2D52_47D9_FE9C_FF969633F579__INCLUDED_
  #define BROCADESELECTOR_H__A8AFB3D9_2D52_47D9_FE9C_FF969633F579__INCLUDED_

#include "BinaryVector.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include <vector>
#include <memory>

namespace mast
{

class SystemModelNode;
class Register;

//! Defines a path selector for Brocade
//!
//! @note It uses N hot encoding (multiple path can be selected at the same time).
//!       It is a specific class in provision for additional, specific, capabilities;
//!
//! @note Path identifier are one based (in range [1..path_count])
//! @note Internal table contains a reserved slot at index zero
//!
class MAST_CORE_EXPORT BrocadeSelector : public DefaultTableBasedPathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~BrocadeSelector() = default;
  BrocadeSelector()  = delete;
  BrocadeSelector(std::shared_ptr<Register> associatedRegister,
                  uint32_t                  slaveTapsCount,
                  SelectorProperty          properties = SelectorProperty::Brocade_Default);

  virtual bool IsActive            (uint32_t pathId) const override; //!< Returns true when the specified path is already active
  virtual bool IsSelected          (uint32_t pathId) const override; //!< Returns true when the specified path is selected
  virtual bool IsSelectedAndActive (uint32_t pathId) const override; //!< Returns true when the specified path is selected and active
  virtual void Select              (uint32_t pathId) override;       //!< Request activation of the specified path
  virtual void Deselect            (uint32_t pathId) override;       //!< Request deactivation of the specified path

  using TablesType = DefaultTableBasedPathSelector::TablesType;

  static TablesType CreateSelectTable   (uint32_t pathsCount, SelectorProperty properties);  //!< Create N_Hot style table to select a endpoint
  static TablesType CreateDeselectTable (uint32_t pathsCount, SelectorProperty properties);  //!< Create N_Hot style table to unselect a endpoint

  virtual std::experimental::string_view KindName() const override { return "Brocade"; }; //!< Returns readable type of selector

  static const uint32_t REGISTER_BITS_COUNT; //!< Brocade control register width
  static const uint32_t MAX_TAPS_COUNT;      //!< Max number of TAPs managed by Brocade circuit
};
//
//  End of BrocadeSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined BROCADESELECTOR_H__A8AFB3D9_2D52_47D9_FE9C_FF969633F579__INCLUDED_
//===========================================================================
// End of BrocadeSelector.hpp
//===========================================================================
