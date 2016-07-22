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

#include <string>
#include <memory>
#include <set>
#include <functional>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>

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

  using string_view = std::experimental::string_view;

  //! Associates a SystemModel to fresh SystemModelManager
  //!
  SystemModelManager(SystemModel& sm,
                     std::shared_ptr<ConfigurationAlgorithm>    configurationAlgorithm = std::make_shared<ConfigureAlgorithm_LastOrDefault>(),
                     std::shared_ptr<SystemModelManagerMonitor> monitor                = std::make_shared<SystemModelManagerMonitor>()
//+                     std::shared_ptr<SystemModelManagerMonitor> monitor                = nullptr
                    );

  //! Does a complete data cycles for SystemModel as long as there are pending nodes
  //!
  //! @note It encompasses configuration, bitstream for SUT collection, bitstream fro SUT
  //!       retrieval and SystemModel updating
  //!
  void DoDataCycles();

  using Application_t = std::function<void()>;

  //! Creates an application thread
  //!
  void CreateApplicationThread(std::shared_ptr<ParentNode> applicationTopNode, Application_t functor, string_view debugName = "");

  //! Waits for all application thread to terminate
  //!
  void WaitForApplicationsEnd ();

  //! Starts all created application threads
  //!
  void StartCreatedApplicationThreads ();

  //! Starts periodical (or on iApply) loop of complete data cycles on a new thread
  //!
  void Start ();

  //! Stops data cycle loop
  //!
  void Stop ();

  //! Executes queued operations
  //!
  void iApply();

  //! Returns current path prefix for current thread
  //!
  std::string iPrefix() const;

  //! Changes path prefix for calling thread
  //!
  void  iPrefix (std::string prefix);

  //! Returns last Register value read from SUT
  //!
  void         iGet (string_view registerPath, uint8_t&      readData);
  void         iGet (string_view registerPath, uint16_t&     readData);
  void         iGet (string_view registerPath, uint32_t&     readData);
  void         iGet (string_view registerPath, uint64_t&     readData);
  void         iGet (string_view registerPath, int8_t&       readData);
  void         iGet (string_view registerPath, int16_t&      readData);
  void         iGet (string_view registerPath, int32_t&      readData);
  void         iGet (string_view registerPath, int64_t&      readData);
  void         iGet (string_view registerPath, BinaryVector& readData);
  BinaryVector iGet (string_view registerPath)
  {
    BinaryVector readData;
    iGet(registerPath, readData);
    return std::move(readData);
  }

  //! Queues data to be read from SUT
  //!
  void iRead (string_view registerPath, BinaryVector expectedValue);

  //! Sets next Register value to sent to SUT
  //!
  void iWrite (string_view registerPath, BinaryVector value);
  void iWrite (string_view registerPath, uint8_t      value);
  void iWrite (string_view registerPath, uint16_t     value);
  void iWrite (string_view registerPath, uint32_t     value);
  void iWrite (string_view registerPath, uint64_t     value);
  void iWrite (string_view registerPath, int8_t       value);
  void iWrite (string_view registerPath, int16_t      value);
  void iWrite (string_view registerPath, int32_t      value);
  void iWrite (string_view registerPath, int64_t      value);

  //! Returns current maximum time between an iApply and the next data cycle
  //!
  std::chrono::milliseconds DataCycleLoopTimeout() const { return m_dataCycleLoopTimeout; }

  //! Sets maximum time between an iApply and the next data cycle
  //!
  void DataCycleLoopTimeout (std::chrono::milliseconds dataCycleLoopTimeout) { m_dataCycleLoopTimeout = dataCycleLoopTimeout; }

  //! Returns current sleep time between two SystemModel configurations (to let application threads to continue their job)
  //!
  std::chrono::milliseconds SleepTimeBetweenConfigurations() const { return m_sleepTimeBetweenConfigurations; }

  //! Sets sleep time between two SystemModel configurations (to let application threads to continue their job)
  //!
  void SleepTimeBetweenConfigurations (std::chrono::milliseconds sleepTimeBetweenConfigurations) { m_sleepTimeBetweenConfigurations = sleepTimeBetweenConfigurations; }


  // ---------------- Protected Methods
  //
  protected:

  static std::shared_ptr<AccessInterface> GetFirstAccessInterface(const SystemModel& sm);

  // ---------------- Private  Methods
  //
  private:

  using NodeIdentifier = SystemModelNode::NodeIdentifier;

  template<typename T> void iGet_impl   (string_view registerPath, T& readData);
  template<typename T> void iWrite_impl (string_view registerPath, T  value);

  struct ApplicationData
  {
    ApplicationData(std::thread p_appThread, NodePathResolver p_pathResolver, string_view p_debugName)
      : appThread    (std::move(p_appThread))
      , canProceed   (false)
      , pathResolver (p_pathResolver)
      , debugName    (p_debugName.to_string())
    {
    }

    std::thread              appThread;           //!< Used to join application thread
    std::mutex               releaseMutex;        //!< Associated with condition variable to block/release pending threads (in iApply)
    std::condition_variable  releaseCv;           //!< Wait mecanism (it is specific to application thread to avoid missing notification)
    std::atomic_bool         canProceed;          //!< When true, application thread can return from iApply
    NodePathResolver         pathResolver;        //!< One per application thread to point to different node, have different prefix and cache
    std::set<NodeIdentifier> pendingRegistersIds; //!< Pending registers for application thread
    std::string              debugName;           //!< Name associated to application thread to ease identification in debug
  };

  using ThreadToAppDataMapper_t = std::map<std::thread::id,     std::shared_ptr<ApplicationData>>;
  using RegIdToAppDataMapper_t  = std::multimap<NodeIdentifier, std::shared_ptr<ApplicationData>>;

  std::shared_ptr<ApplicationData> ApplicationDataForThreadId (std::thread::id threadId) const;
  std::shared_ptr<ApplicationData> ThreadApplicationData() const { return ApplicationDataForThreadId(std::this_thread::get_id()); }


  const NodePathResolver&          PathResolver(const char* file, const char* fct, uint32_t line, std::experimental::string_view msg) const;

  void LoopOnDataCycle ();
  void DoDataCycles_Impl ();
  void RegisterPendingThread (std::shared_ptr<Register> reg);
  void ReleaseServedThreads ();
  void ReportServedRegisters (const std::vector<NodeIdentifier>& activeRegisters);
  void WakeupDataCycles ();

  // ---------------- Private  Fields
  //
  private:
  // Data cycle support
  SystemModel&                               m_sm;                   //!< The system model to manage
  std::shared_ptr<AccessInterface>           m_firstAccessInterface; //!< The first AccessInterface of the system
  ConfigureVisitor                           m_configurator;         //!< In charge of configuration
  PropagatePendingVisitor                    m_propagator;           //!< In charge of propagating pending status bottom up
  ToSutVisitor                               m_toSutVisitor;         //!< In charge of collecting bitstream to SUT
  FromSutUpdater                             m_fromSutUpdater;       //!< In charge of updating SystemModel from bitstream from SUT
  NodePathResolver                           m_pathResolver;         //!< Node path resolver for SystemModelManager thread
  std::shared_ptr<SystemModelManagerMonitor> m_monitor;              //!< Provides monitoring point




  // Multithreading support
  std::thread                     m_managerThread;                  //!< Background thread for data cycle loop
  std::thread::id                 m_managerThreadId;                //!< Manager thread identifier (when constructed or when running data cycle loop in a background thread)
  const std::thread::id           m_constructionThreadId;           //!< Thread identifier when constructed
  std::atomic_bool                m_threadStarted;                  //!< To wait for application thread effectively started before returning to caller
  std::mutex                      m_appStartMutex;                  //!< Associated to condition variable for common start of application threads
  std::condition_variable         m_appStartConditionVar;           //!< Variable to manage common start of application threads
  std::atomic_bool                m_appStarted;                     //!< True when application threads are requested to start effectively
  std::recursive_mutex            m_dataMutex;                      //!< Protects access to SystemModel and common data used to manage application threads
  RegIdToAppDataMapper_t          m_regIdToAppData;                 //!< Associates a register id with application data for threadS that are pending on that register
  std::set<std::thread::id>       m_pendingThreads;                 //!< Identifies threads that must be paused in iApply
  std::atomic_bool                m_loopStarted;                    //!< True when data cycle loop thread has been started effectively (waiting for iApply)
  std::mutex                      m_loopMutex;                      //!< Associated to condition variable to manage restart of data cycle loop
  std::condition_variable         m_loopCV;                         //!< Variable to manage restart of data cycle loop
  bool                            m_runLoop = false;                //!< True when data cycle loop is active
  std::chrono::milliseconds       m_dataCycleLoopTimeout;           //!< Approximate max time before an iApply is seen by data cycle loop
  std::chrono::milliseconds       m_sleepTimeBetweenConfigurations; //!< Sleep duration between two configurations to let application thread to access registers
  mutable std::shared_timed_mutex m_appDataMutex;                   //!< Protects access to applications data (mutable to be used within const methods)
  ThreadToAppDataMapper_t         m_threadToAppData;                //!< Associates a thread id with application data for that thread
};
//
//  End of SystemModelManager class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_

//===========================================================================
// End of SystemModelManager.hpp
//===========================================================================
