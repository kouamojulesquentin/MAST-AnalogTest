//===========================================================================
//                           DefaultTableBasedPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultTableBasedPathSelector.hpp
//!
//! Declares class DefaultTableBasedPathSelector
//!
//===========================================================================

#ifndef DEFAULTTABLEBASEDPATHSELECTOR_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_
  #define DEFAULTTABLEBASEDPATHSELECTOR_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

#include "PathSelector.hpp"
#include "BinaryVector.hpp"
#include "Platform.hpp"

#include <vector>
#include <memory>
#include <experimental/string_view>

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
class DLL_EXPORT DefaultTableBasedPathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultTableBasedPathSelector() = default;
  DefaultTableBasedPathSelector()  = delete;

  using TablesType = std::vector<BinaryVector> ; //!< Selection/deselection LUT types

  //! Constructs from selection and deselection tables with pathCount entries
  //!
  DefaultTableBasedPathSelector(std::shared_ptr<Register> associatedRegister,
                                uint32_t                  pathsCount,
                                TablesType                selectTable,
                                TablesType                deselectTable,
                                SelectorProperty          properties = SelectorProperty::None);

  //! Constructs from tables defined as string parameters
  //!
  DefaultTableBasedPathSelector(std::shared_ptr<Register>      associatedRegister,
                                uint32_t                       pathsCount,
                                SelectorProperty               properties,
                                std::experimental::string_view tables);

  virtual bool IsActive            (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already active
  virtual bool IsSelected          (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected
  virtual bool IsSelectedAndActive (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected and active
  virtual void Select              (uint32_t pathIdentifier) override;       //!< Request activation of the specified path
  virtual void Deselect            (uint32_t pathIdentifier) override;       //!< Request deactivation of the specified path

  virtual std::shared_ptr<const Register> AssociatedRegister() const override { return m_muxRegister; }  //!< Returns associated Register

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

  static TablesType& InvertTable (TablesType& table);  //!< Inverts table bits
  static TablesType& FixTable    (TablesType& table);  //!< Makes table entry fix (cannot modify size)

  //! Returns readable type of selector
  //!
  virtual std::experimental::string_view KindName() const override { return "Table_Based"; };

  //! Returns debug information about selector
  //!
  virtual std::string DebugSelectorInfo(bool onlyProperties) const override;

  // ---------------- Protected Methods
  //
  protected:

  void CheckPathIdentifier (uint32_t pathIdentifier) const;

  // ---------------- Protected Fields
  //
  protected:
  uint32_t                  m_pathsCount;    //!< Number of managed paths
  std::shared_ptr<Register> m_muxRegister;   //!< Register that drives the paths multiplexer
  TablesType                m_selectTable;   //!< Selection LUT   (non const because of deferred initialization)
  TablesType                m_deselectTable; //!< Deselection LUT (non const because of deferred initialization)
};
//
//  End of DefaultTableBasedPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined DEFAULTTABLEBASEDPATHSELECTOR_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

//===========================================================================
// End of DefaultTableBasedPathSelector.hpp
//===========================================================================
