//===========================================================================
//                           AST_ScalarIdentifier.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScalarIdentifier.hpp
//!
//! Declares AST_ScalarIdentifier class
//!
//===========================================================================

#ifndef AST_SCALARIDENTIFIER_H__22E87096_14D1_4D26_65BD_D689E2BB6FA3__INCLUDED_
  #define AST_SCALARIDENTIFIER_H__22E87096_14D1_4D26_65BD_D689E2BB6FA3__INCLUDED_

#include "AST_Identifier.hpp"
#include <string>
#include <experimental/string_view>

namespace Parsers
{
//! Specifies a plain name or a single bit name
//!
class AST_ScalarIdentifier final : public AST_Identifier
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ScalarIdentifier() = default;
  AST_ScalarIdentifier()  = delete;

//+  std::string Name() const { return m_identifier; } //!< Text representation of identifier

  std::string AsText() const override { return m_identifier; }; //!< Text representation of identifier

  // ---------------- Private Methods
  //
  friend class AST;                                                             // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ScalarIdentifier)(std::experimental::string_view&); // AST currently uses make_unit<T>() to create nodes

  AST_ScalarIdentifier(std::experimental::string_view identifier)
    : AST_Identifier (Kind::ScalarIdentifier)
    , m_identifier   (identifier)
  {
  }

  std::string m_identifier; //!< Textual representation of identifier
};
//
//  End of AST_ScalarIdentifier class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_SCALARIDENTIFIER_H__22E87096_14D1_4D26_65BD_D689E2BB6FA3__INCLUDED_

//===========================================================================
// End of AST_ScalarIdentifier.hpp
//===========================================================================
