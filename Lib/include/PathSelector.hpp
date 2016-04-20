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

namespace mast
{
//! Extension point interface used by Linker to manage paths selection
//!
class PathSelector
{
  // ---------------- Public  Methods
  //
  public:

  virtual bool IsActive (uint32_t pathIdentifier) const = 0; //!< Returns true when the specified path is already selected
  virtual void Deselect (uint32_t pathIdentifier) = 0;       //!< Request desactivation of the specified path
  virtual void Select   (uint32_t pathIdentifier) = 0;       //!< Request activation of the specified path

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
