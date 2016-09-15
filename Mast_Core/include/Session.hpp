//===========================================================================
//                           Session.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Session.hpp
//!
//! Declares Session class
//!
//===========================================================================


#ifndef SESSION_H__F9BF7523_CC0A_4C47_6489_D8B23FC91559__INCLUDED_
  #define SESSION_H__F9BF7523_CC0A_4C47_6489_D8B23FC91559__INCLUDED_

#include "Platform.hpp"
#include <memory>

namespace g3
{
  class LogWorker;
}

namespace mast
{
class SystemModelManagerMonitor;
class SystemModel;
class SystemModelManager;

//! Defines a "MAST session" with initialization at construction and cleanup at destruction
//!
//! @note One can extend its behaviour to fit special needs
//!
class DLL_EXPORT Session
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Session();

  Session();
  Session(std::shared_ptr<SystemModelManagerMonitor> monitor);

  // ---------------- Public  Fields
  //
  public:
  std::shared_ptr<g3::LogWorker>      logger;
  std::shared_ptr<SystemModel>        sm;
  std::shared_ptr<SystemModelManager> manager;
};
//
//  End of Session class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SESSION_H__F9BF7523_CC0A_4C47_6489_D8B23FC91559__INCLUDED_

//===========================================================================
// End of Session.hpp
//===========================================================================
