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

namespace mast
{
class Register;

//! Abstract base class for implementation of path selector that are based upon a LUTs
//! for selection and deselection values
//!
//! @note Path identifier are defined in range [1..nb_path]
//!
class DLL_EXPORT DefaultTableBasedPathSelector  : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultTableBasedPathSelector() = default;
  DefaultTableBasedPathSelector()  = delete;

  using TablesType = std::vector<BinaryVector> ; //!< Selection/deselection LUT types

  //! Constructs selection and deselection tables with pathCount entries
  //!
  DefaultTableBasedPathSelector(std::shared_ptr<Register> associatedRegister,
                                uint32_t                  pathsCount,
                                TablesType                selectTable,
                                TablesType                deselectTable,
                                bool                      canSelectNone = false);

  virtual bool IsActive   (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already selected
  virtual bool IsSelected (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already selected
  virtual void Select     (uint32_t pathIdentifier) override;       //!< Request activation of the specified path
  virtual void Deselect   (uint32_t pathIdentifier) override;       //!< Request deactivation of the specified path

  virtual uint32_t SelectablePaths() const override { return m_pathsCount; };   //!< Returns the maximum number of selectable paths (max value for IsActive, Select and Deselect)
  virtual bool     CanSelectNone()   const override { return m_canSelectNone; }   //!< Returns true if selector can select nothing (passthrough mode), false otherwise

  uint32_t ActiveCount() const;    //!< Returns the number of paths that are currently active

  virtual void Accept   (SystemModelVisitor& visitor) override;   //!< Forwards call to any embedded Register

  static TablesType& InvertTable (TablesType& table);  //!< Inverts table bits
  static TablesType& FixTable    (TablesType& table);  //!< Makes table entry fix (cannot modify size)

  // ---------------- Protected Methods
  //
  protected:

  void CheckPathIdentifier (uint32_t pathIdentifier) const;



  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:

  uint32_t                  m_pathsCount;             //!< Number of managed paths
  std::shared_ptr<Register> m_muxRegister;            //!< Register that drives the paths multiplexer
  const TablesType          m_selectTable;            //!< Selection LUT
  const TablesType          m_deselectTable;          //!< Deselection LUT
  const bool                m_canSelectNone = false;  //!< When true zero is reserved to select 'no path' otherwise 0 is used to select first path
};
//
//  End of DefaultTableBasedPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined DEFAULTTABLEBASEDPATHSELECTOR_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

//===========================================================================
// End of DefaultTableBasedPathSelector.hpp
//===========================================================================
