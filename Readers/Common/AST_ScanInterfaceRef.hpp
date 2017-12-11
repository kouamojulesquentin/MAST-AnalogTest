//===========================================================================
//                           AST_ScanInterfaceRef.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanInterfaceRef.hpp
//!
//! Declares AST_ScanInterfaceRef class
//!
//===========================================================================

#ifndef AST_SCANINTERFACEREF_H__433BFD28_DA7E_46EE_79B_3E501C0EA670__INCLUDED_
  #define AST_SCANINTERFACEREF_H__433BFD28_DA7E_46EE_79B_3E501C0EA670__INCLUDED_

#include "AST_Node.hpp"
#include <vector>
#include <tuple>
#include <string>

namespace Parsers
{
class AST_ScalarIdentifier;

//! Identifies a ScanInterface reference (in AccessLink context)
//!
class AST_ScanInterfaceRef final : public AST_Node
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ScanInterfaceRef() = default;
  AST_ScanInterfaceRef()  = delete;

  //! Returns refered, scoped, ScanInterface names
  //!
  const std::vector<std::tuple<AST_ScalarIdentifier*, std::string>>&
  ScanInterfaceNames() const { return m_scanInterfaceNames; }

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_ScanInterfaceRef)(std::vector<std::tuple<Parsers::AST_ScalarIdentifier*, std::string>>&&);

  //! Initializes AST_ScanInterface with children
  //!
  //! @param identifier   Scan interface name
  //! @param children     Scan interface children nodes
  //!
  AST_ScanInterfaceRef(std::vector<std::tuple<AST_ScalarIdentifier*, std::string>>&& scanInterfaceNames)
    : AST_Node             (Kind::ScanInterfaceRef)
    , m_scanInterfaceNames (std::move(scanInterfaceNames))
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::vector<std::tuple<AST_ScalarIdentifier*, std::string>> m_scanInterfaceNames; // Tuple first item identifies an optional instance, 2nd item identifies a scan interface
};
//
//  End of AST_ScanInterfaceRef class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_SCANINTERFACEREF_H__433BFD28_DA7E_46EE_79B_3E501C0EA670__INCLUDED_

//===========================================================================
// End of AST_ScanInterfaceRef.hpp
//===========================================================================
