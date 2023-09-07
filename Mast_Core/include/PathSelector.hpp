//===========================================================================
//                           PathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PathSelector.hpp
//!
//! Declares PathSelector interface and common implementation
//!
//===========================================================================


#ifndef LINKERINFORMATION_H__9B76DEAF_2DDC_4FFA_9F8A_EC2E62431F68__INCLUDED_
  #define LINKERINFORMATION_H__9B76DEAF_2DDC_4FFA_9F8A_EC2E62431F68__INCLUDED_

#include <cstdint>
#include <memory>
#include <experimental/string_view>

namespace mast
{

enum class SelectorProperty
{
  None             = 0b0000,
  CannotSelectNone = 0b0000, //!< Tells that at least one path must be selected
  CanSelectNone    = 0b0001, //!< Tells whether one can select no path using path id 0 (mainly for binary encoding)
  InvertedBits     = 0b0010, //!< Tells whether a '0' select a endpoint (low level activation in hardware)
  ReverseOrder     = 0b0100, //!< Tells whether bits order is reversed (e.g. in 1_Hot encoding last bit select first endpoint)
  StartAtZero      = 0b1000, //!< Tells whether the first path is selected by a zero value)

  // Helpers for common usage
  Std             = None,                         //!< Default property for any type
  Binary_Default  = None,                         //!< Default property for binary based encoding
  One_Hot_Default = CanSelectNone,                //!< Default property for one hot based encoding
  N_Hot_Default   = CanSelectNone,                //!< Default property for N hot based encoding
  Brocade_Default = CanSelectNone | ReverseOrder, //!< Default property for Brocade compatible encoding
};


class SystemModelVisitor;
class VirtualRegister;
class BinaryVector;

//! Extension point interface used by Linker to manage paths selection
//!
//! @note It is actually an abstract base class that provide SelectorProperty getter
//!
class PathSelector
{
  friend class Linker;

  // ---------------- Public  Methods
  //
  public:

  PathSelector(SelectorProperty properties) : m_properties(properties) {}

  //! Returns true when the specified path is already selected
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual bool IsActive (uint32_t pathIdentifier) const = 0;

  //! Returns the number of paths that are currently active
  //!
  virtual uint32_t ActiveCount() const = 0;

  //! Returns true when the specified path is already selected
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual bool IsSelected (uint32_t pathIdentifier) const = 0;

  //! Returns true when the specified path is already selected and active
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual bool IsSelectedAndActive (uint32_t pathIdentifier) const = 0;

  //! Forwards call to any embedded SystemModelNode (or do nothing)
  //!
  virtual void Accept   (SystemModelVisitor& visitor) = 0;

  //! Returns the maximum number of selectable paths (max value for IsActive, Select and Deselect)
  //!
  virtual uint32_t SelectablePaths() const = 0;

  //! Returns associated VirtualRegister or nullptr when there is none
  //!
  virtual const VirtualRegister* AssociatedRegisters() const = 0;

  //! Returns some value associated with specified path selection
  //!
  //! @note When cannot provide a value, an empty BinaryVector is returned
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual const BinaryVector& SelectionValue (uint32_t pathIdentifier) const = 0;

  //! Returns readable type of selector (binary, One_Hot, N_Hot...)
  //!
  virtual std::experimental::string_view KindName() const = 0;

  //! Returns readable information about selector (ex: select and deselect tables)
  //!
  virtual std::string DebugSelectorInfo(bool onlyProperties) const = 0;

  //! Returns selector main properties
  //!
  SelectorProperty Properties() const { return m_properties; }

  //! Returns true if selector can select nothing (passthrough mode), false otherwise
  //!
  bool CanSelectNone() const
  {
    return (    static_cast<std::underlying_type_t<SelectorProperty>>(m_properties)
              & static_cast<std::underlying_type_t<SelectorProperty>>(SelectorProperty::CanSelectNone)
           ) == static_cast<std::underlying_type_t<SelectorProperty>>(SelectorProperty::CanSelectNone);
  }

  // ---------------- Protected Methods
  //
  virtual ~PathSelector() = default;
  PathSelector()  = default;

  //! Returns associated VirtualRegister or nullptr when there is none
  //!
  virtual VirtualRegister* AssociatedRegisters() = 0;

  //! Requests activation of the specified path
  //!
  //! @note Expected to be called only by Linker
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual void Select   (uint32_t pathIdentifier) = 0;

  //! Request deactivation of the specified path
  //!
  //! @note Expected to be called only by Linker
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual void Deselect (uint32_t pathIdentifier) = 0;


  // ---------------- Private  Fields
  //
  const SelectorProperty m_properties = SelectorProperty::None;       //!< Defines selector properies
};
//
//  End of PathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined LINKERINFORMATION_H__9B76DEAF_2DDC_4FFA_9F8A_EC2E62431F68__INCLUDED_

//===========================================================================
// End of PathSelector.hpp
//===========================================================================
