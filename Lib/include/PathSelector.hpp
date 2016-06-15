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
class SystemModelVisitor;
class Register;

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

  //! Request activation of the specified path
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

  virtual std::shared_ptr<const Register> AssociatedRegister() const = 0;  //!< Returns associated Register or nullptr when there is none

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
