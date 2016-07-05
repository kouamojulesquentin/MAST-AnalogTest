//===========================================================================
//                           SystemModelManager.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManager.hpp
//!
//! Declares SystemModelManager class
//!
//===========================================================================


#ifndef SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_
  #define SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_

#include "Platform.hpp"
#include "ConfigureVisitor.hpp"
#include "PropagatePendingVisitor.hpp"
#include "ToSutVisitor.hpp"
#include "FromSutUpdater.hpp"
#include "NodePathResolver.hpp"
#include "SystemModelManagerMonitor.hpp"

#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace mast
{
class AccessInterface;
class SystemModel;

//! Coordinates the dynamic aspects of the system model
//!
//! @note The main task is to trigger configuration of the system model,
//!       collect the bitstream to sent to SUT, retrieve the bitstream
//!       from SUT and updating the system model with it
//!
class DLL_EXPORT SystemModelManager final
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelManager();
  SystemModelManager() = delete;

  //! Associates a SystemModel to fresh SystemModelManager
  //!
  SystemModelManager(SystemModel& sm,
                     std::shared_ptr<ConfigurationAlgorithm>    configurationAlgorithm = std::make_shared<ConfigureAlgorithm_LastOrDefault>(),
                     std::shared_ptr<SystemModelManagerMonitor> monitor                = std::make_shared<SystemModelManagerMonitor>()
//+                     std::shared_ptr<SystemModelManagerMonitor> monitor                = nullptr
                    )
    : m_sm                   (sm)
    , m_firstAccessInterface (GetFirstAccessInterface(sm))
    , m_configurator         (configurationAlgorithm)
    , m_propagator           ()
    , m_toSutVisitor         ()
    , m_fromSutUpdater       (sm)
    , m_monitor              (monitor)
    , m_managerThreadId      (std::this_thread::get_id())
    , m_pathResolver         (sm.Root())
  {  }

  //! Does a complete data cycles for SystemModel as long as there are pending nodes
  //!
  //! @note It encompasses configuration, bitstream for SUT collection, bitstream fro SUT
  //!       retrieval and SystemModel updating
  //!
  void DoDataCycles();

  using Application_t = std::function<void()>;

  //! Creates an application thread
  //!
  void CreateApplicationThread(std::shared_ptr<ParentNode> applicationTopNode, Application_t functor);

  //! Waits for all application thread to terminate
  //!
  void JoinAllApplicationThreads ();

  //! Starts all created application threads
  //!
  void StartCreatedApplicationThreads ();

  //! Returns current path prefix for current thread
  //!
  std::string iPrefix() const;

  //! Changes path prefix for calling thread
  //!
  void  iPrefix (std::string prefix);

  //! Returns last Register value read from SUT
  //!
  BinaryVector iGet (std::experimental::string_view registerPath);

  // ---------------- Protected Methods
  //
  protected:

  static std::shared_ptr<AccessInterface> GetFirstAccessInterface(const SystemModel& sm);

  // ---------------- Private  Methods
  //
  private:

  struct ApplicationData
  {
    ApplicationData(std::thread appThread, NodePathResolver pathResolver)
      : m_thread       (std::move(appThread))
      , m_pathResolver (pathResolver)
    {
    }

    std::thread      m_thread;
    NodePathResolver m_pathResolver;
  };

  using ApplicationDataMapper_t = std::map<std::thread::id, std::shared_ptr<ApplicationData>>;
//+  using ApplicationDataMapper_t = std::map<std::thread::id, ApplicationData>;

  std::shared_ptr<ApplicationData> ApplicationDataForCurrentThread() const;
  const NodePathResolver&          PathResolver(const char* file, const char* fct, uint32_t line, std::experimental::string_view msg) const;

  // ---------------- Private  Fields
  //
  private:
  SystemModel&                               m_sm;                   //!< The system model to manage
  std::shared_ptr<AccessInterface>           m_firstAccessInterface; //!< The first AccessInterface of the system
  ConfigureVisitor                           m_configurator;         //!< In charge of configuration
  PropagatePendingVisitor                    m_propagator;           //!< In charge of propagating pending status bottom up
  ToSutVisitor                               m_toSutVisitor;         //!< In charge of collecting bitstream to SUT
  FromSutUpdater                             m_fromSutUpdater;       //!< In charge of updating SystemModel from bitstream from SUT
  std::shared_ptr<SystemModelManagerMonitor> m_monitor;              //!< Provides monitoring point
  const std::thread::id                      m_managerThreadId;      //!< Thread that created the manager
  NodePathResolver                           m_pathResolver;         //!< Node path resolver for SystemModelManager thread
  std::mutex                                 m_appStartMutex;        //!< Mutex to manage common start of application threads
  std::condition_variable                    m_appStartConditionVar; //!< Variable to manage common start of application threads
  bool                                       m_appStarted = false;   //!< True when application threads are requested to start effectively
  ApplicationDataMapper_t                    m_applicationsData;     //!< Associates a thread id with application data for that thread
};
//
//  End of SystemModelManager class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_

//===========================================================================
// End of SystemModelManager.hpp
//===========================================================================
