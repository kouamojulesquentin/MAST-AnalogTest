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

namespace mast
{

enum class SelectorProperty
{
  None             = 0b0000,
  CannotSelectNone = 0b0000, //!< Tells that at least one path must be selected
  CanSelectNone    = 0b0001, //!< Tells whether one can select no path using path id 0 (mainly for binary encoding)
  InvertedBits     = 0b0010, //!< Tells whether a '0' select a derivation (low level activation in hardware)
  ReverseOrder     = 0b0100, //!< Tells whether bits order is reversed (e.g. in 1_Hot encoding last bit select first derivation)

  Binary_Default  = None,          //!< Default property for binary based encoding
  One_Hot_Default = CanSelectNone, //!< Default property for one hot based encoding
  N_Hot_Default   = CanSelectNone, //!< Default property for N hot based encoding
};


class SystemModelVisitor;
class Register;
class BinaryVector;

//! Extension point interface used by Linker to manage paths selection
//!
class PathSelector
{
  // ---------------- Public  Methods
  //
  public:

  //! Returns true when the specified path is already selected
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual bool IsActive (uint32_t pathIdentifier) const = 0;

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

  //! Requests activation of the specified path
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual void Select   (uint32_t pathIdentifier) = 0;

  //! Request deactivation of the specified path
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual void Deselect (uint32_t pathIdentifier) = 0;


  //! Forwards call to any embedded SystemModelNode (or do nothing)
  //!
  virtual void Accept   (SystemModelVisitor& visitor) = 0;

  //! Returns the maximum number of selectable paths (max value for IsActive, Select and Deselect)
  //!
  virtual uint32_t SelectablePaths() const = 0;

  //! Returns true if selector can select nothing (passthrough mode), false otherwise
  //!
  virtual bool CanSelectNone() const = 0;

  //! Returns associated Register or nullptr when there is none
  //!
  virtual std::shared_ptr<const Register> AssociatedRegister() const = 0;

  //! Returns some value associated with specified path selection
  //!
  //! @note When cannot provide a value, an empty BinaryVector is returned
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual const BinaryVector& SelectionValue (uint32_t pathIdentifier) const = 0;

  // ---------------- Protected Methods
  //
  virtual ~PathSelector() = default;
  PathSelector()  = default;
};
//
//  End of PathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined LINKERINFORMATION_H__9B76DEAF_2DDC_4FFA_9F8A_EC2E62431F68__INCLUDED_

//===========================================================================
// End of PathSelector.hpp
//===========================================================================
