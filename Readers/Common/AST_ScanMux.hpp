//===========================================================================
//                           AST_ScanMux.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanMux.hpp
//!
//! Declares AST_ScanMux class
//!
//===========================================================================

#ifndef AST_SCANMUX_H__2D48EB30_FF6B_481A_4F95_463CCA5B9FCB__INCLUDED_
  #define AST_SCANMUX_H__2D48EB30_FF6B_481A_4F95_463CCA5B9FCB__INCLUDED_

#include "AST_ParentNode.hpp"
#include "AST_VectorIdentifier.hpp"

#include <memory>
#include <vector>
#include <string>
#include <experimental/string_view>

namespace Parsers
{
class AST_Signal;
class AST_ScanMuxSelection;

//! Represents a scan chain multiplexer
//!
class AST_ScanMux final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ScanMux() = default;
  AST_ScanMux()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! ScanMux identifier
  //!
  const AST_VectorIdentifier* Identifier() const { return m_identifier; }

  //! Returns ScanMux name
  //!
  std::string Name() const override { return m_identifier->AsText(); }

  //! ScanMux base name (without indices)
  //!
  const std::string& BaseName() const { return m_identifier->BaseName(); }

  //! Returns ScanMux selector signals
  //!
  const std::vector<Parsers::AST_Signal*>& Selectors() const { return m_selectors; }

  //! Returns ScanMux selection info
  //!
  const std::vector<Parsers::AST_ScanMuxSelection*>& Selections() const { return m_selections; }


  // ---------------- Private Methods
  //
  private:

  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_ScanMux)(Parsers::AST_VectorIdentifier*&,
                                     std::vector<Parsers::AST_Signal*>&&,
                                     std::vector<Parsers::AST_ScanMuxSelection*>&&);

  //! Initializes AST_ScanMux
  //!
  //! @param identifier           ScanMux identifier
  //! @param selectors            Selection signals that are used to drive the ScanMux
  //! @param scanMuxSelections    Selections definition i.e. which value(s) select which signal(s)
  //!
  AST_ScanMux(AST_VectorIdentifier*                identifier,
              std::vector<Parsers::AST_Signal*>&&  selectors,
              std::vector<AST_ScanMuxSelection*>&& scanMuxSelections)
    : AST_ParentNode (Kind::ScanMux)
    , m_identifier   (identifier)
    , m_selectors    (std::move(selectors))
    , m_selections   (std::move(scanMuxSelections))
  {
    DispatchChildren();
  }


  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  const AST_VectorIdentifier*                 m_identifier = nullptr; //!< ScanMux identifier
  std::vector<Parsers::AST_Signal*>           m_selectors;            //!< Selection signals that are used to drive the ScanMux
  std::vector<Parsers::AST_ScanMuxSelection*> m_selections;           //!< Selections definition i.e. which value(s) select which signal(s)
};
//
//  End of AST_ScanMux class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_SCANMUX_H__2D48EB30_FF6B_481A_4F95_463CCA5B9FCB__INCLUDED_
//===========================================================================
// End of AST_ScanMux.hpp
//===========================================================================
