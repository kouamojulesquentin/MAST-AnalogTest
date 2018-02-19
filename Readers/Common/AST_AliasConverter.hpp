//===========================================================================
//                           AST_AliasConverter.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_AliasConverter.hpp
//!
//! Declares AST_AliasConverter class
//!
//===========================================================================

#ifndef AST_ALIASCONVERTER_H__1AD6B622_864D_4FF0_F795_5FDC8200AD38__INCLUDED_
  #define AST_ALIASCONVERTER_H__1AD6B622_864D_4FF0_F795_5FDC8200AD38__INCLUDED_

#include "VirtualRegister.hpp"

namespace mast
{
class ParentNode;
}

namespace Parsers
{
class AST_Module;
class AST_Signal;
class AST_ScanRegister;

//! Helper to convert AST aliases to SystemModel aliases
//!
//!
class AST_AliasConverter final
{
  // ---------------- Public Methods
  //
  public:
  ~AST_AliasConverter() = default;
  AST_AliasConverter()  = default;

  //! Converts aliases, starting from given AST module
  //!
  static void ConvertAliases(AST_Module* module, mast::ParentNode* parentNode);

  // ---------------- Private Methods
  //
  private:
  static mast::RegisterSlice MakeRegisterSlice           (AST_ScanRegister* scanRegister, AST_Signal* signal);
  static AST_ScanRegister*   FollowSignalTilScanRegister (AST_Module*       module,       AST_Signal* signal);
};
//
//  End of AST_AliasConverter class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_ALIASCONVERTER_H__1AD6B622_864D_4FF0_F795_5FDC8200AD38__INCLUDED_

//===========================================================================
// End of AST_AliasConverter.hpp
//===========================================================================
