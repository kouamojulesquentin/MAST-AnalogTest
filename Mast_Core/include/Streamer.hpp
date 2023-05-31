//===========================================================================
//                           Streamer.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Streamer.hpp
//!
//! Declares class Streamer
//!
//! A Streamer is a component that is put on a scan chain and modifies the 
//! stream of data coming through it, therefore having an effect on all
//! downstream components
//! A Streamer makes bit-by-bit transformations on incoming and outcoming data
//! which can be seen as a MASK applied on the whole Chain
//!
//===========================================================================

#ifndef STREAMER_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_
  #define STREAMER_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_

#include "ParentNode.hpp"
#include "BinaryVector.hpp"
#include "StreamerProtocol.hpp"

//Only for experiments
#include "TransparentStreamerProtocol.hpp"

using std::make_shared;

namespace mast
{

//! Represents a Stremear node, which is structurally is similar to a chain, but which applies a 
//! transformation on the bitstreams transversing it
//!
class MAST_CORE_EXPORT Streamer : public ParentNode
{
  // ---------------- Public  Methods
  //
  public:
  ~Streamer() = default;
  Streamer()  = delete;
  Streamer(std::experimental::string_view name) : ParentNode(name) {
  m_Protocol = make_shared<TransparentStreamerProtocol>("");
  }
  Streamer(std::experimental::string_view name,std::shared_ptr<StreamerProtocol> protocol) : ParentNode(name) {
  m_Protocol = protocol;
  }

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "Streamer"; } //!< Returns readable type name

  std::shared_ptr<StreamerProtocol> Protocol() {return m_Protocol;};
  
  protected:
  std::shared_ptr<StreamerProtocol> m_Protocol;
  
  };
//
//  End of Streamer class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined STREAMER_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_

//===========================================================================
// End of Streamer.hpp
//===========================================================================
