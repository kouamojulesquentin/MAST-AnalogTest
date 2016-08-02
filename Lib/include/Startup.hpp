//===========================================================================
//                           Startup.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Startup.hpp
//!
//! Declares Startup class
//!
//===========================================================================


#ifndef STARTUP_H__F34EFB54_17C0_4861_41A4_5FD5CC09AA7F__INCLUDED_
  #define STARTUP_H__F34EFB54_17C0_4861_41A4_5FD5CC09AA7F__INCLUDED_

#include "Platform.hpp"
#include <memory>

namespace mast
{
class SystemModel;
class SystemModelManager;

//! Provides startup facilities to build a system model and run the manager
//!
//!
class DLL_EXPORT Startup final
{

  // ---------------- Public  Methods
  //
  public:
  ~Startup() = delete;
  Startup()  = delete;

  static std::shared_ptr<SystemModel>        GetSystemModel();
  static std::shared_ptr<SystemModelManager> GetManager();

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  static std::shared_ptr<SystemModel>        sm_systemModel;
  static std::shared_ptr<SystemModelManager> sm_manager;
};
//
//  End of Startup class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined STARTUP_H__F34EFB54_17C0_4861_41A4_5FD5CC09AA7F__INCLUDED_

//===========================================================================
// End of Startup.hpp
//===========================================================================
