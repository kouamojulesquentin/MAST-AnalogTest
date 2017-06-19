//===========================================================================
//                           SystemModelManager_impl.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManager_impl.hpp
//!
//! Declares SystemModelManager_impl class
//!
//===========================================================================


#ifndef SYSTEMMODELMANAGER_IMPL_H__14A9849F_92EC_4138_15AF_79EDDA563071__INCLUDED_
  #define SYSTEMMODELMANAGER_IMPL_H__14A9849F_92EC_4138_15AF_79EDDA563071__INCLUDED_

#include "ConfigureVisitor.hpp"
#include "PropagatePendingVisitor.hpp"
#include "ToSutVisitor.hpp"
#include "FromSutUpdater.hpp"
#include "NodePathResolver.hpp"

#include <string>
#include <experimental/string_view>
#include <memory>
#include <set>
#include <queue>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>

namespace mast
{
class SystemModel;
class SystemModelNode;
class ParentNode;
class SystemModelManagerMonitor;
class SystemModelManager_impl;
class ConfigurationAlgorithm;
class BinaryVector;

//! Implements actual SystemModelManager.
//!
//! @note This is a separate implementation to reduce dependencies for user of the public interface
//!
class SystemModelManager_impl final
{
  // ---------------- Public Methods
  //
  public:
  ~SystemModelManager_impl();
  SystemModelManager_impl() = delete;

  using string_view = std::experimental::string_view;

  //! Associates a SystemModel to fresh SystemModelManager
  //!
  SystemModelManager_impl(SystemModel&                               sm,
                          std::shared_ptr<ConfigurationAlgorithm>    configurationAlgorithm,
                          std::shared_ptr<SystemModelManagerMonitor> monitor
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
  void CreateApplicationThread(std::shared_ptr<ParentNode> applicationTopNode, Application_t functor, std::string debugName);

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
  std::string iPrefix();

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

  //! Returns XOR of the last value read from SUT and the expected value
  //!
  BinaryVector iGetMiscompares (string_view registerPath);

  //! Queues data to be read from SUT
  //!
  void iRead (string_view registerPath, BinaryVector expectedValue);
  void iRead (string_view registerPath, uint8_t      expectedValue);
  void iRead (string_view registerPath, uint16_t     expectedValue);
  void iRead (string_view registerPath, uint32_t     expectedValue);
  void iRead (string_view registerPath, uint64_t     expectedValue);
  void iRead (string_view registerPath, int8_t       expectedValue);
  void iRead (string_view registerPath, int16_t      expectedValue);
  void iRead (string_view registerPath, int32_t      expectedValue);
  void iRead (string_view registerPath, int64_t      expectedValue);


  void iRead (string_view registerPath, BinaryVector expectedValue, BinaryVector dontCareMask);
  void iRead (string_view registerPath, uint8_t      expectedValue, uint8_t      dontCareMask);
  void iRead (string_view registerPath, uint16_t     expectedValue, uint16_t     dontCareMask);
  void iRead (string_view registerPath, uint32_t     expectedValue, uint32_t     dontCareMask);
  void iRead (string_view registerPath, uint64_t     expectedValue, uint64_t     dontCareMask);
  void iRead (string_view registerPath, int8_t       expectedValue, int8_t       dontCareMask);
  void iRead (string_view registerPath, int16_t      expectedValue, int16_t      dontCareMask);
  void iRead (string_view registerPath, int32_t      expectedValue, int32_t      dontCareMask);
  void iRead (string_view registerPath, int64_t      expectedValue, int64_t      dontCareMask);

  //! Queues data to be read from SUT without checking the value
  //!
  void iRefresh (string_view registerPath);

  //! Requests register value to be read from SUT and wait till it can be return
  //!
  void         iGetRefresh (string_view registerPath, uint8_t&      readData);
  void         iGetRefresh (string_view registerPath, uint16_t&     readData);
  void         iGetRefresh (string_view registerPath, uint32_t&     readData);
  void         iGetRefresh (string_view registerPath, uint64_t&     readData);
  void         iGetRefresh (string_view registerPath, int8_t&       readData);
  void         iGetRefresh (string_view registerPath, int16_t&      readData);
  void         iGetRefresh (string_view registerPath, int32_t&      readData);
  void         iGetRefresh (string_view registerPath, int64_t&      readData);
  void         iGetRefresh (string_view registerPath, BinaryVector& readData);
  BinaryVector iGetRefresh (string_view registerPath)
  {
    BinaryVector readData;
    iGetRefresh(registerPath, readData);
    return std::move(readData);
  }

  //! Returns the number of expected read failure for SystemModel sub-tree
  //!
  uint32_t iGetStatus (std::shared_ptr<SystemModelNode> node, bool clearCounter);


  //! Returns the number of expected read failure for the complete system model
  //!
  uint32_t iGetStatus(bool clearCounter);

  //! Returns the number of expected read failures from SystemModel sub-tree
  //!
  uint32_t iGetStatus(string_view nodePath, bool clearCounter);

  //! Forces the ResetPort to be asserted on the target module and reset SystemModel accordingly
  //!
  void iReset(bool doSynchronousReset = false);

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

  //! Returns instance that monitor manager main actions
  //!
  std::shared_ptr<SystemModelManagerMonitor>  Monitor() const { return m_monitor; }

  //! Sets instance that monitor manager main actions
  //!
  void  Monitor (std::shared_ptr<SystemModelManagerMonitor> monitor) { m_monitor = monitor; }


  // ---------------- Private  Methods
  //
  private:
  static std::shared_ptr<AccessInterface> GetFirstAccessInterface(const SystemModel& sm);

  template<typename T> uint32_t iGetStatus (T you_should_call_iStatus_with_path_and_clear_counter_parameters); //!< Non implemented version to avoid implicit conversion from char* to bool

  using NodeIdentifier = SystemModelNode::NodeIdentifier;

  template<typename T> void iGet_impl        (string_view registerPath, T& readData);
  template<typename T> void iGetRefresh_impl (string_view registerPath, T& readData);
  template<typename T> void iRead_impl       (string_view registerPath, T  expectedValue);
  template<typename T> void iRead_impl       (string_view registerPath, T  expectedValue, T dontCareMask);
  template<typename T> void iWrite_impl      (string_view registerPath, T  value);

  struct QueuedRequest
  {
    QueuedRequest(NodeIdentifier p_id, BinaryVector p_value = BinaryVector(), BinaryVector p_mask = BinaryVector())
      : regId (p_id), value (p_value), mask (p_mask)
    {}

    NodeIdentifier regId = 0; //!< Identifies the register for which there is a queued operation
    BinaryVector   value;     //!< The value associated with the operation
    BinaryVector   mask;      //!< For read operation, defined whitch bits are ignored (zeros) or not (ones)
  };

  struct ApplicationData
  {
    enum class State
    {
      NotInitialized,
      Initialized,
      WrapperThreadStarted,
      ApplicationThreadStarted,
      WriteRequest,
      ReadRequest,
      RefreshRequest,
      InApply,
      Running,
      Terminated,
      TerminatedWithException,
    };

    ApplicationData(std::thread p_appThread, std::shared_ptr<State> p_currentState, NodePathResolver p_pathResolver, string_view p_debugName)
      : appThread    (std::move(p_appThread))
      , currentState (p_currentState)
      , canProceed   (false)
      , pathResolver (p_pathResolver)
      , debugName    (p_debugName.to_string())
    {
    }

    std::thread               appThread;           //!< Used to join application thread
    std::shared_ptr<State>    currentState;        //!< This is for debug purpose only
    std::mutex                releaseMutex;        //!< Associated with condition variable to block/release pending threads (in iApply)
    std::condition_variable   releaseCv;           //!< Wait mecanism (it is specific to application thread to avoid missing notification)
    std::atomic_bool          canProceed;          //!< When true, application thread can return from iApply
    NodePathResolver          pathResolver;        //!< One per application thread to point to different node, have different prefix and cache
    std::set<NodeIdentifier>  pendingRegistersIds; //!< Pending registers for application thread
    std::deque<QueuedRequest> queuedWrites;        //!< Identifiers for registers queued for a write operation
    std::deque<QueuedRequest> queuedReads;         //!< Identifiers for registers queued for a read operation
    std::deque<QueuedRequest> queuedRefreshes;     //!< Identifiers for registers queued for a refresh operation
    std::string               debugName;           //!< Name associated to application thread to ease identification in debug
  };

  using ThreadToAppDataMapper_t = std::map<std::thread::id,     std::shared_ptr<ApplicationData>>;
  using RegIdToAppDataMapper_t  = std::multimap<NodeIdentifier, std::shared_ptr<ApplicationData>>;

  std::shared_ptr<ApplicationData> ApplicationDataForThreadId (std::thread::id threadId) const;
  std::shared_ptr<ApplicationData> ThreadApplicationData() const { return ApplicationDataForThreadId(std::this_thread::get_id()); }


  const NodePathResolver& PathResolver(const char* file, const char* fct, uint32_t line, std::experimental::string_view msg);

  void ProcessQueuedRequests (std::shared_ptr<ApplicationData> appData);
  void LoopOnDataCycle ();
  void DoHierarchicalDataCycle (std::shared_ptr<AccessInterface>  currentAccessInterface, std::shared_ptr<AccessInterface>  InterfaceTranslator);
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
  FromSutUpdater                             m_fromSutUpdater;       //!< In charge of updating SystemModel from bitstream from SUT
  NodePathResolver                           m_pathResolver;         //!< Node path resolver for SystemModelManager thread
  std::shared_ptr<SystemModelManagerMonitor> m_monitor;              //!< Provides monitoring point




  // Multithreading support
  std::thread                      m_managerThread;                  //!< Background thread for data cycle loop
  std::thread::id                  m_managerThreadId;                //!< Manager thread identifier (when constructed or when running data cycle loop in a background thread)
  const std::thread::id            m_constructionThreadId;           //!< Thread identifier when constructed
  std::atomic_bool                 m_threadStarted;                  //!< To wait for application thread effectively started before returning to caller
  std::mutex                       m_appStartMutex;                  //!< Associated to condition variable for common start of application threads
  std::condition_variable          m_appStartConditionVar;           //!< Variable to manage common start of application threads
  std::atomic_bool                 m_appStarted;                     //!< True when application threads are requested to start effectively
  std::recursive_mutex             m_dataMutex;                      //!< Protects access to SystemModel and common data used to manage application threads
  RegIdToAppDataMapper_t           m_regIdToAppData;                 //!< Associates a register id with application data for threadS that are pending on that register
  std::set<std::thread::id>        m_pendingThreads;                 //!< Identifies threads that must be paused in iApply
  std::atomic_bool                 m_loopStarted;                    //!< True when data cycle loop thread has been started effectively (waiting for iApply)
  std::mutex                       m_loopMutex;                      //!< Associated to condition variable to manage restart of data cycle loop
  std::condition_variable          m_loopCV;                         //!< Variable to manage restart of data cycle loop
  bool                             m_runLoop = false;                //!< True when data cycle loop is active
  std::chrono::milliseconds        m_dataCycleLoopTimeout;           //!< Approximate max time before an iApply is seen by data cycle loop
  std::chrono::milliseconds        m_sleepTimeBetweenConfigurations; //!< Sleep duration between two configurations to let application thread to access registers
  mutable std::shared_timed_mutex  m_appDataMutex;                   //!< Protects access to applications data (mutable to be used within const methods)
  ThreadToAppDataMapper_t          m_threadToAppData;                //!< Associates a thread id with application data for that thread
  std::shared_ptr<ApplicationData> m_mainThreadAppData;              //!< For single thread model, this is the associated application data
};
//
//  End of SystemModelManager_impl class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELMANAGER_IMPL_H__14A9849F_92EC_4138_15AF_79EDDA563071__INCLUDED_

//===========================================================================
// End of SystemModelManager_impl.hpp
//===========================================================================
