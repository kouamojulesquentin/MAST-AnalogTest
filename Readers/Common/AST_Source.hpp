//===========================================================================
//                           AST_Source.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Source.hpp
//!
//! Declares AST_Source class
//!
//===========================================================================


#ifndef AST_SOURCE_H__98B7A0F4_334_4BA7_FEA3_76B2A0D8944B__INCLUDED_
  #define AST_SOURCE_H__98B7A0F4_334_4BA7_FEA3_76B2A0D8944B__INCLUDED_

#include "AST_Node.hpp"
#include <vector>
#include <string>

namespace Parsers
{
class AST_Signal;

//! Represents some source as single or multible signal(s)
//!
class AST_Source final : public AST_Node
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Source() = default;
  AST_Source()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Returns source signals
  //!
  const std::vector<AST_Signal*>& Signals() const { return m_signals; }

  //! Text representation of source
  //!
  std::string AsText() const;

  // ---------------- Private Methods
  //
  private:

  friend class AST;                                                   // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Source)(Parsers::Kind& kind, Parsers::AST_Signal*&);
  MAKE_UNIQUE_AS_FRIEND(AST_Source)(Parsers::Kind& kind, std::vector<Parsers::AST_Signal*>&&);

  //! Constructs from a single signal
  //!
  AST_Source(Kind kind, AST_Signal* signal)
    : AST_Node  (kind)
    , m_signals ()
  {
    m_signals.push_back(signal);
  }

  //! Constructs from a bunch of ordered (concatenated) signals
  //!
  AST_Source(Kind kind, std::vector<AST_Signal*>&& signals)
    : AST_Node  (kind)
    , m_signals (std::move(signals))
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::vector<AST_Signal*> m_signals; //!< Signal(s) that compose(s) the source
};
//
//  End of AST_Source class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_SOURCE_H__98B7A0F4_334_4BA7_FEA3_76B2A0D8944B__INCLUDED_

//===========================================================================
// End of AST_Source.hpp
//===========================================================================
