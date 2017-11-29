//===========================================================================
//                           AST_Builder.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Builder.hpp
//!
//! Declares AST_Builder class
//!
//===========================================================================

#ifndef AST_CLONER_H__87628D5E_5B7_455A_EE81_2F5FC373AAEF__INCLUDED_
  #define AST_CLONER_H__87628D5E_5B7_455A_EE81_2F5FC373AAEF__INCLUDED_

namespace Parsers
{
class AST_Instance;
class AST_Module;
class AST_Network;
class AST_ScalarIdentifier;
class AST_ScanMux;
class AST_ScanRegister;
class AST_ModuleIdentifier;

//! Interfaces for cloning AST nodes
//!
//! @note It is mainly used to avoid cyclic dependency (as a benefit, there is also less compile dependency)
class AST_Builder
{
  // ---------------- Public Methods
  //
  public:
  virtual AST_Instance*     Clone_Instance     (const AST_Instance*     instance) = 0;     //!< Clones an instance
  virtual AST_Module*       Clone_Module       (const AST_Module*       module) = 0;       //!< Clones a module
  virtual AST_ScanMux*      Clone_ScanMux      (const AST_ScanMux*      scanMux) = 0;      //!< Clones a scan multiplexer
  virtual AST_ScanRegister* Clone_ScanRegister (const AST_ScanRegister* scanRegister) = 0; //!< Clones a scan register

  virtual AST_ScalarIdentifier* Create_UniquifiedIdentifier       (const AST_ScalarIdentifier* identifier) = 0; //!< Creates an identifier for a uniquified entity
  virtual AST_ModuleIdentifier* Create_UniquifiedModuleIdentifier (const AST_Module*           module) = 0;     //!< Creates an module identifier for a uniquified module

  virtual AST_Network* Network() = 0;  //!< Returns instruments test network

  // ---------------- Protected Methods
  //
  protected:
  virtual ~AST_Builder() = default;
  AST_Builder()          = default;
};
//
//  End of AST_Builder class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_CLONER_H__87628D5E_5B7_455A_EE81_2F5FC373AAEF__INCLUDED_

//===========================================================================
// End of AST_Builder.hpp
//===========================================================================
