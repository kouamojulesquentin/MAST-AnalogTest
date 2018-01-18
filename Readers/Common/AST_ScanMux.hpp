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

namespace mast
{
class Linker;
}

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

  //! Tells whether this ScanMux multiplexes more than one bit
  //!
  bool IsBusMux () const;

  std::shared_ptr<mast::Linker> AssociatedLinker() { return m_associatedModelLinker; }  //!< Mast SystemModel associated Linker (when not null)

  //! Sets Mast SystemModel associated Linker (when creating SystemModel)
  //!
  void  AssociatedLinker (std::shared_ptr<mast::Linker> associatedLinker) { m_associatedModelLinker = associatedLinker; };

  //! Tells if ScanMux is already associated with a SystemModel Linker
  //!
  bool HasAssociatedLinker() const { return m_associatedModelLinker ? true : false; }


  // ---------------- Private Methods
  //
  private:

  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_ScanMux)(Parsers::AST_VectorIdentifier*&,
                                     std::vector<Parsers::AST_Signal*>&&,
                                     std::vector<Parsers::AST_ScanMuxSelection*>&&);

  //! Initializes AST_ScanMux
  //!
  AST_ScanMux(AST_VectorIdentifier*                identifier,
              std::vector<Parsers::AST_Signal*>&&  selectors,
              std::vector<AST_ScanMuxSelection*>&& scanMuxSelections);


  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  const AST_VectorIdentifier*                 m_identifier = nullptr;  //!< ScanMux identifier
  std::vector<Parsers::AST_Signal*>           m_selectors;             //!< Selection signals that are used to drive the ScanMux
  std::vector<Parsers::AST_ScanMuxSelection*> m_selections;            //!< Selections definition i.e. which value(s) select which signal(s)
  std::shared_ptr<mast::Linker>               m_associatedModelLinker; //!< Associated Linker once mast model is created (SystemModel creation)
};
//
//  End of AST_ScanMux class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_SCANMUX_H__2D48EB30_FF6B_481A_4F95_463CCA5B9FCB__INCLUDED_
//===========================================================================
// End of AST_ScanMux.hpp
//===========================================================================
