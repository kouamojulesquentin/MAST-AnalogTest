//===========================================================================
//                           Chain.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Chain.hpp
//!
//! Declares class Chain
//!
//===========================================================================

#ifndef CHAIN_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_
  #define CHAIN_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_

#include "ParentNode.hpp"

namespace mast
{

//! Represents a (fix) chain of registers, allowing nodes to be concatenated to replicate a scan chain composed of TDRs.
//! This node represents the concatenation of elements (register, ScanMuxes etc..) in the SUT through the cardinality
//! of children nodes: node(i+1) follows node (i).
//!
//! @note note that a CHAIN node with only one derivation has no functional effect on the execution, but it can be
//! used to package some sub-system and exploit the "append" method to assure correct construction.
//!
class Chain : public ParentNode
{
  // ---------------- Public  Methods
  //
  public:
  ~Chain() = default;
  Chain()  = default;

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual const uint8_t* GetLastSequence() const;  //!< Returns pointer on byte-stream for last sequence shifted from sut
  virtual const uint8_t* GetNextSequence() const;  //!< Returns pointer on byte-stream for next sequence to shift into sut
};
//
//  End of Chain class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CHAIN_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_

//===========================================================================
// End of Chain.hpp
//===========================================================================
