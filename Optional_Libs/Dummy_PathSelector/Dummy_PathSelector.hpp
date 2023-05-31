//===========================================================================
//                           Dummy_PathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dummy_PathSelector.hpp
//!
//! Declares class Dummy_PathSelector
//!
//===========================================================================

#ifndef Dummy_PathSelector_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_
  #define Dummy_PathSelector_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

#include "PathSelector.hpp"
#include "BinaryVector.hpp"
#include "VirtualRegister.hpp"
#include "Mast_Core_export.hpp"
#include "EnumsUtility.hpp"

#include <vector>
#include <memory>
#include <experimental/string_view>
#include <ostream>

namespace mast
{
class Register;

//! Base class for implementation of path selector that are based upon a LUTs
//! for selection and deselection values
//!
//! @note Do not support independent selection
//!       Only one path can be selected at any given time (unless there are voluntarily coupled)
//! @note Do not support independent deselection
//!
//! @note Path identifier are defined in range [1..nb_path]
//!
class MAST_CORE_EXPORT Dummy_PathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~Dummy_PathSelector() = default;
  Dummy_PathSelector()  = delete;

  using TablesType = std::vector<BinaryVector> ; //!< Selection/deselection LUT types

  //! Example Constructor with pathCount entries
  //! Compatible with FullCustom factory creator
  //!
  Dummy_PathSelector(std::shared_ptr<Register> associatedRegister,
                                uint32_t               pathsCount,
				const std::string& parameters);

  virtual bool IsActive            (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already active
  virtual bool IsSelected          (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected
  virtual bool IsSelectedAndActive (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected and active
  virtual void Select              (uint32_t pathIdentifier) override;       //!< Request activation of the specified path
  virtual void Deselect            (uint32_t pathIdentifier) override;       //!< Request deactivation of the specified path

  virtual const VirtualRegister* AssociatedRegisters() const override { return &m_muxRegisters; }  //!< Returns associated Registers

  virtual uint32_t SelectablePaths() const override { return m_pathsCount; };   //!< Returns the maximum number of selectable paths (max value for IsActive, Select and Deselect)

  virtual uint32_t ActiveCount() const override;    //!< Returns the number of paths that are currently active

  virtual void Accept   (SystemModelVisitor& visitor) override;   //!< Forwards call to any embedded Register

  //! Returns some value associated with specified path selection
  //!
  //! @note When cannot provide a value, an empty BinaryVector is returned
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual const BinaryVector& SelectionValue (uint32_t pathIdentifier) const override;

  //! Returns readable type of selector
  //!
  virtual std::experimental::string_view KindName() const override { return "Dummy"; };

  //! Returns debug information about selector
  //!
  virtual std::string DebugSelectorInfo(bool onlyProperties) const override;

  // ---------------- Protected Methods
  //
  protected:

  virtual VirtualRegister* AssociatedRegisters() override { return &m_muxRegisters; }  //!< Returns associated Registers

  void CheckPathIdentifier (uint32_t pathIdentifier) const;

  // ---------------- Protected Fields
  //
  protected:
  BinaryVector m_DummySelectorInformation;

  // ---------------- Private Fields
  //
  uint32_t        m_pathsCount;    //!< Number of managed paths
  VirtualRegister m_muxRegisters;  //!< Register(s) that drive(s) the paths multiplexer
};
//
//  End of Dummy_PathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined Dummy_PathSelector_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

//===========================================================================
// End of Dummy_PathSelector.hpp
//===========================================================================
