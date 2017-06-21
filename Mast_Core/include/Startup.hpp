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

#include "LoggerSinks.h"

#include "Mast_Core_export.hpp"
#include <memory>

namespace g3
{
  class LogWorker;
}

namespace mast
{
class SystemModel;
class SystemModelManager;

//! Provides startup facilities to build a system model and run the manager
//!
//!
class MAST_CORE_EXPORT Startup final
{
  // ---------------- Public  Methods
  //
  public:
  ~Startup() = delete;
  Startup()  = delete;

  static std::shared_ptr<SystemModel>        GetSystemModel_NoCreate() { return sm_systemModel; }
  static std::shared_ptr<SystemModel>        GetSystemModel(); //!< Returns shared SystemModel
  static std::shared_ptr<SystemModelManager> GetManager_NoCreate()     { return sm_manager;     }
  static std::shared_ptr<SystemModelManager> GetManager();     //!< Returns shared SystemModelManager
  static std::shared_ptr<g3::LogWorker>      GetLogger();      //!< Returns shared logger
  static void                                StartLogger();    //!< Initializes logger facility
  static void                                StopLogger();     //!< Stop logger facility

  static void ForgetSystemModel(); //!< Gets rid of common SystemModel singleton
  static void ForgetManager();     //!< Gets rid of common SystemModelManager singleton

  // ---------------- Private  Fields
  //
  private:
  friend class MastEnvironment_impl;

  static void InitializeLogger ();

  using CerrSinkHandle_t = g3::SinkHandle<g3::ErrorsOnCerrLoggerSink>;

  static std::shared_ptr<g3::LogWorker>      sm_logger;
  static std::shared_ptr<g3::LogFormatter>   sm_logFormatter;   //!< Logger message formatter
  static std::unique_ptr<CerrSinkHandle_t>   sm_cerrSinkHandle; //!< Initial logger sink that is disabled once user requested sink are connected

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
