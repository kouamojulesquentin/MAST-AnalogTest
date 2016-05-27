//===========================================================================
//                           Conditioners.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Conditioners.hpp
//!
//! Declares Conditioners class
//!
//===========================================================================


#ifndef CONDITIONERS_H__8E5BB768_ACBE_4573_47A8_C035B5B0A749__INCLUDED_
  #define CONDITIONERS_H__8E5BB768_ACBE_4573_47A8_C035B5B0A749__INCLUDED_
namespace mast
{
//! Interface for condition checker used by SystemModelNode
//!
class Conditioners
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Conditioners() = default;
  Conditioners()  = default;

  //! Does required pre-conditioning for specified node.
  //!
  //! @note Some nodes might require special conditioning operations prior to the main data cycle.
  //!       It is the case, for instance, of AI_TRANSLATOR nodes, but it could also be the case for
  //!       legacy nonstandard implementation.
  //!       Preconditioning is done by a depth-first exploration of the active tree.
  //!
  virtual void DoPreConditioning(std::shared_ptr<SystemModelNode> node) = 0;

  //! Does required post-conditioning for specified node.
  //!
  //! @note Some nodes might require special conditioning operations after the main data cycle.
  //!       It is the case, for instance, of AI_TRANSLATOR nodes, but it could also be the case for
  //!       legacy nonstandard implementation.
  //!       Preconditioning is done by a depth-first exploration of the active tree.
  //!
  virtual void DoPostConditioning(std::shared_ptr<SystemModelNode> node) = 0;
};
//
//  End of Conditioners class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined CONDITIONERS_H__8E5BB768_ACBE_4573_47A8_C035B5B0A749__INCLUDED_

//===========================================================================
// End of Conditioners.hpp
//===========================================================================
