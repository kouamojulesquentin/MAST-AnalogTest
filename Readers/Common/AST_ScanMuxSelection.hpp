//===========================================================================
//                           AST_ScanMuxSelection.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanMuxSelection.hpp
//!
//! Declares AST_ScanMuxSelection class
//!
//===========================================================================

#ifndef AST_SCANMUXSELECTION_H__BB26C2E0_5F89_4B6B_42AF_F90168C536D7__INCLUDED_
  #define AST_SCANMUXSELECTION_H__BB26C2E0_5F89_4B6B_42AF_F90168C536D7__INCLUDED_

#include "AST_SimpleNode.hpp"

#include <vector>
#include <string>

namespace Parsers
{
class AST_Signal;

//! Represents a scan chain multiplexer
//!
class AST_ScanMuxSelection final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ScanMuxSelection() = default;
  AST_ScanMuxSelection()  = delete;

  std::string AsText() const override; //!< Text representation of value

  //! Returns set of values for that selection
  //!
  //! @note several different values can select the same set of signals
  //!
  const std::vector<std::string>& SelectionsValues() const { return m_selectionValues; }

  //! Selected signal(s) when multiplexer selector has one of selection values
  //!
  const std::vector<Parsers::AST_Signal*>& SelectedSignals() const { return m_selectedSignals; }

  // ---------------- Private Methods
  //
  private:

  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_ScanMuxSelection)(std::vector<std::string>&& selectionValues, std::vector<Parsers::AST_Signal*>&& selectedSignals);


  //! Initializes AST_ScanMuxSelection with selection values and selected signals
  //!
  AST_ScanMuxSelection(std::vector<std::string>&& selectionValues, std::vector<Parsers::AST_Signal*>&& selectedSignals);

  // ---------------- Private Fields
  //
  private:
  std::vector<std::string>          m_selectionValues; //!< Selection values
  std::vector<Parsers::AST_Signal*> m_selectedSignals; //!< Selected signal when multiplexer selector has one of selection values
};
//
//  End of AST_ScanMuxSelection class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_SCANMUXSELECTION_H__BB26C2E0_5F89_4B6B_42AF_F90168C536D7__INCLUDED_
//===========================================================================
// End of AST_ScanMuxSelection.hpp
//===========================================================================
