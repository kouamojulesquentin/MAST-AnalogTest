//===========================================================================
//                           AST_Helper.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Helper.hpp
//!
//! Declares AST_Helper class
//!
//===========================================================================


#ifndef AST_HELPER_H__CFE7DBA6_B4AA_49EF_34A7_7148CD40672F__INCLUDED_
  #define AST_HELPER_H__CFE7DBA6_B4AA_49EF_34A7_7148CD40672F__INCLUDED_

namespace Parsers
{

class AST_Module;
class AST_Identifier;
class AST_Signal;
class AST_Source;
class AST_ScanRegister;
class AST_ScalarIdentifier;
class AST_VectorIdentifier;

//! Provides helpers to deal with AST
//! @note There are helper for:
//!       - Traversing network till a ScanRegister is encountered
//!
class AST_Helper final
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Helper() = delete;
  AST_Helper()  = delete;

  //! Follows a signal until it reaches a ScanRegister
  //!
  static AST_ScanRegister* FollowSignalTilScanRegister (AST_Module* module, const AST_Signal* signal);

  //! Searches connection down to ScanRegister, starting from an instance Port
  //!
  static AST_ScanRegister* ScanRegisterConnectedToInstancePort (const AST_Module*           module,
                                                                const AST_ScalarIdentifier* instanceId,
                                                                const AST_VectorIdentifier* portId);

  static const AST_Signal* SourceSignalOfModulePort (const AST_Module* module, const AST_Identifier* portId);
};
//
//  End of AST_Helper class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_HELPER_H__CFE7DBA6_B4AA_49EF_34A7_7148CD40672F__INCLUDED_

//===========================================================================
// End of AST_Helper.hpp
//===========================================================================
