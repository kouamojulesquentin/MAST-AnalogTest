//===========================================================================
//                           SystemModelManager.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManager.cpp
//!
//! Implements class SystemModelManager
//!
//===========================================================================


#include "SystemModelManager.hpp"
#include "SystemModel.hpp"
#include "AccessInterface.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "Chain.hpp"
#include "Utility.hpp"
#include "SystemModelManagerMonitor.hpp"
#include "g3log/g3log.hpp"

#include <utility>
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::vector;
using std::string;
using std::experimental::string_view;
using std::mutex;
using std::recursive_mutex;
using std::shared_timed_mutex;
using std::lock_guard;
using std::unique_lock;
using std::shared_lock;

using namespace mast;
using namespace std::chrono_literals;

#define MONITOR(fct)                            if (m_monitor) m_monitor->fct;
#define MONITOR_MESSAGE(msg)                    if (m_monitor) m_monitor->LogUncondionally(msg);
#define MONITOR_WITH_NODE(msg, node, debugName) if (m_monitor) m_monitor->LogUncondionally(msg, node, debugName);

#define PATH_RESOLVER(msg)                                        PathResolver(__FILE__, __func__, __LINE__, msg)
#define MUTABLE_PATH_RESOLVER(msg)  const_cast<NodePathResolver&>(PathResolver(__FILE__, __func__, __LINE__, msg))

//! Joins application threads
//!
SystemModelManager::~SystemModelManager ()
{
  MONITOR_MESSAGE("Destructing SystemModelManager (begin)");
  Stop();                         // Stopping data cycle loop also release thread blocked in iApply
  WaitForApplicationsEnd();
  MONITOR_MESSAGE("Destructing SystemModelManager (end)");
}
//
//  End of: SystemModelManager::~SystemModelManager
//---------------------------------------------------------------------------


//! Associates a SystemModel to fresh SystemModelManager
//!
SystemModelManager::SystemModelManager(SystemModel&                          sm,
                                       shared_ptr<ConfigurationAlgorithm>    configurationAlgorithm,
                                       shared_ptr<SystemModelManagerMonitor> monitor)
  : m_sm                             (sm)
  , m_firstAccessInterface           (GetFirstAccessInterface(sm))
  , m_configurator                   (configurationAlgorithm)
  , m_propagator                     ()
  , m_toSutVisitor                   ()
  , m_fromSutUpdater                 (sm)
  , m_pathResolver                   (sm.Root())
  , m_monitor                        (monitor)
  , m_managerThreadId                (std::this_thread::get_id())
  , m_constructionThreadId           (std::this_thread::get_id())
  , m_threadStarted                  (false)
  , m_appStarted                     (false)
  , m_loopStarted                    (false)
  , m_dataCycleLoopTimeout           (1s)
  , m_sleepTimeBetweenConfigurations (0ms)
{
  auto appState       = make_shared<ApplicationData::State>(ApplicationData::State::ApplicationThreadStarted);
  auto pathResolver   = NodePathResolver(sm.Root());
  m_mainThreadAppData = make_shared<ApplicationData>(std::thread(), appState, pathResolver, "Manager");

  MONITOR_MESSAGE("Constructed SystemModelManager");
}

//! Returns application data associated with a thread
//!
//! @param threadId   Identifier of the thread
//!
shared_ptr<SystemModelManager::ApplicationData> SystemModelManager::ApplicationDataForThreadId (std::thread::id threadId) const
{
  shared_lock<shared_timed_mutex> lock(m_appDataMutex); // Shared lock is enough for read concurrency

  auto pos = m_threadToAppData.find(threadId);
  if (pos == m_threadToAppData.cend())
  {
    if (threadId != m_managerThreadId)
    {
      THROW_LOGIC_ERROR("Thread is not managed by SystemModelManager");
    }


  }

  auto   data = pos->second;
  return data;
}
//
//  End of: SystemModelManager::ApplicationDataForThreadId
//---------------------------------------------------------------------------


//! Creates an application thread
//!
//! @param applicationTopNode Top most node associated with the application
//! @param functor            Function to call at thread creation
//!
void SystemModelManager::CreateApplicationThread (shared_ptr<ParentNode> applicationTopNode, Application_t functor, string_view debugName)
{
  CHECK_PARAMETER_NOT_NULL(applicationTopNode, "Cannot create application thread with nullptr top node");

  MONITOR(CreateApplication(*applicationTopNode, debugName));

  auto wrapper = [this, applicationTopNode, functor](std::shared_ptr<ApplicationData::State> applicationState, string_view debugName)
  {
    // ---------------- Report that the thread has effectively been started
    //
    m_threadStarted = true;
    MONITOR_WITH_NODE("Application thread waiting for start signal", *applicationTopNode, debugName);

    // ---------------- Wait for start "signal"
    //
    auto predicate = [this]{ return m_appStarted.load(); };

    std::unique_lock<std::mutex> lock(m_appStartMutex);
    m_appStartConditionVar.wait(lock, predicate);
    lock.unlock();

    *applicationState = ApplicationData::State::ApplicationThreadStarted;

    // ---------------- To actual application job
    //
    MONITOR_WITH_NODE("Application start", *applicationTopNode, debugName);
    try
    {
      functor();
      *applicationState = ApplicationData::State::Terminated;
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      *applicationState = ApplicationData::State::TerminatedWithException;
      MONITOR_WITH_NODE("Uncaught exception '"s + exc.what() + "' from application", *applicationTopNode, debugName);
    }
    catch (...)
    {
      *applicationState = ApplicationData::State::TerminatedWithException;
      MONITOR_WITH_NODE("Uncaught unknown exception from application", *applicationTopNode, debugName);
    }
    MONITOR_WITH_NODE("Application ends", *applicationTopNode, debugName);
  };

  m_threadStarted   = false;    // This is to detect when the thread begins to run (waiting for start signal)
  auto appState     = make_shared<ApplicationData::State>(ApplicationData::State::NotInitialized);
  auto appThread    = std::thread(wrapper, appState, debugName);
  auto appThreadId  = appThread.get_id();
  auto pathResolver = NodePathResolver(applicationTopNode);
  auto data         = make_shared<ApplicationData>(std::move(appThread), appState, pathResolver, debugName);

  *data->currentState = ApplicationData::State::Initialized;
  while (!m_threadStarted)
  {
    std::this_thread::sleep_for(100us);
  }
  *data->currentState = ApplicationData::State::WrapperThreadStarted;
  MONITOR_WITH_NODE("Application thread have reported to be running", *applicationTopNode, debugName);

  m_threadStarted = false;

  unique_lock<shared_timed_mutex> lock(m_appDataMutex);
  m_threadToAppData[appThreadId] = data;
}
//
//  End of: SystemModelManager::CreateApplicationThread
//---------------------------------------------------------------------------


//! Does a complete data cycles for SystemModel as long as there are pending nodes
//!
//! @note It does the data cycle over each AccessInterface in the SystemModel
//!
void SystemModelManager::DoDataCycles ()
{
  if (std::this_thread::get_id() != m_managerThreadId)
  {
    THROW_RUNTIME_ERROR("DoDataCycles shall be called only on SystemModelManager thread");
  }

  DoDataCycles_Impl();
}


//! Does a complete data cycles for SystemModel as long as there are pending nodes
//!
//! @note It does the data cycle over each AccessInterface in the SystemModel
//!
void SystemModelManager::DoDataCycles_Impl ()
{
  MONITOR(StartDataCycles());
  auto root        = m_sm.Root();
  auto doDataCycle = true;

  unique_lock<recursive_mutex> lock(m_dataMutex);
  lock.unlock();
  do
  {
    lock.lock();

    MONITOR(StartDataCycle());
    MONITOR(BeforeConfiguration(*root));
    root->Accept(m_configurator);
    root->Accept(m_propagator);
    MONITOR(AfterConfiguration(*root));

    doDataCycle = root->IsPending();

    if (doDataCycle)
    {
      auto nextAccessInterface = m_firstAccessInterface;

      while (nextAccessInterface)
      {
        if (nextAccessInterface->IsPending())
        {
          auto protocol = nextAccessInterface->Protocol();
          CHECK_VALUE_NOT_NULL(protocol, "All AccessInterface must be associated with a valid protocol");

          uint32_t derivationId   = 1u;
          auto     nextDerivation = nextAccessInterface->FirstChild();

          while (nextDerivation)
          {
            if (nextDerivation->IsPending())
            {
              m_toSutVisitor.Reset();
              nextDerivation->Accept(m_toSutVisitor);

              const auto& toSutVector = m_toSutVisitor.ToSutVector();
              const auto& activeRegs  = m_toSutVisitor.ActiveRegistersIdentifiers();

              auto fromSutVector = protocol->DoAction(derivationId, nextDerivation->ApplicationData(), toSutVector);

              m_fromSutUpdater.UpdateRegisters(activeRegs, fromSutVector);
              ReportServedRegisters(activeRegs);
              ReleaseServedThreads();
            }

            nextDerivation = nextDerivation->NextSibling();
            ++derivationId;
          }
        }

        nextAccessInterface = dynamic_pointer_cast<AccessInterface>(nextAccessInterface->NextSibling());;
      }

      // ---------------- Release mutex and wait awhile for blocked (but not pending) threads can move forward
      //
      lock.unlock();
      std::this_thread::sleep_for(m_sleepTimeBetweenConfigurations);
    } // End of: if (doDataCycle)
  } while (doDataCycle);

  // ---------------- Release threads blocked in iApply without any pending registers
  //
  ReleaseServedThreads();
}
//
//  End of: SystemModelManager::DoDataCycles
//---------------------------------------------------------------------------


//! Retrieves first AccessInterface from a SystemModel
//! @note SystemModel root node must be the only AccessInterface or a chain for which all children
//!       are AccessInterface
//!
shared_ptr<AccessInterface> SystemModelManager::GetFirstAccessInterface (const SystemModel& sm)
{
  auto root = sm.Root();

  CHECK_VALUE_NOT_NULL(root, "Cannot manage an empty system model");

  auto accessInterface = dynamic_pointer_cast<AccessInterface>(root);
  if (!accessInterface)
  {
    auto asChain = dynamic_pointer_cast<Chain>(root);
    CHECK_VALUE_NOT_NULL(asChain, "SystemModel root must be an AccessInterface or a Chain (for which chidren are AccessInterface)");

    accessInterface = dynamic_pointer_cast<AccessInterface>(asChain->FirstChild());
    CHECK_VALUE_NOT_NULL(accessInterface, "Root (a Chain) must have only AccessInterface children");
  }

  return accessInterface;
}
//
//  End of: SystemModelManager::GetFirstAccessInterface
//---------------------------------------------------------------------------




//! Executes queued operations
//!
//! @note It triggers a data cycles and wait for all its pending registers being served
//!
void SystemModelManager::iApply ()
{

  auto appData           = ThreadApplicationData();
  *appData->currentState = ApplicationData::State::InApply;

  ProcessQueuedRequests(appData);

  auto threadId = std::this_thread::get_id();
  if (threadId == m_managerThreadId)  // Single thread context?
  {
    DoDataCycles();
  }
  else
  {
    // ---------------- Report that this thread is pending
    //
    {
      unique_lock<recursive_mutex> lock(m_dataMutex);
      appData->canProceed = false;      // Must be set to false before reporting as pending thread !
      m_pendingThreads.insert(threadId);
    }

    WakeupDataCycles();

    MONITOR_WITH_NODE("Will be blocked in iApply", *appData->pathResolver.ReferenceNode(), appData->debugName)

    // ---------------- Block the thread until data cycle loop release it (or is terminated)
    //                  As waking up of data cycle loop may occurs before wait
    //                  we use a timeout to check again if we can proceed (this is to compensate for potential notification loss)
    auto predicate = [appData, this]{ return appData->canProceed.load() || !m_runLoop; };
    auto timeout    = 500us;
    auto maxTimeout = 100ms;

    while (!predicate())
    {
      if (timeout < maxTimeout) { timeout *= 2; }

      {
        std::unique_lock<std::mutex> lock(appData->releaseMutex);
        appData->releaseCv.wait_for(lock, timeout, predicate);
      }
    }

    if (!m_runLoop)
    {
      MONITOR_MESSAGE("iApply - Application thread has been released because data cycle loop is not/no more running ");
    }
    else
    {
      MONITOR_WITH_NODE("iApply - Released ", *appData->pathResolver.ReferenceNode(), appData->debugName)
    }
  }

  *appData->currentState = ApplicationData::State::Running;
  LOG(DEBUG) << "iApply - Leaving";
}
//
//  End of: SystemModelManager::iApply
//---------------------------------------------------------------------------



//! Returns last read value from specified register
//!
template<typename T>
void SystemModelManager::iGet_impl (string_view registerPath, T& readData)
{
  auto& pathResolver = PATH_RESOLVER("iGet: ");
  auto reg           = pathResolver.ResolveAsRegister(registerPath);

  unique_lock<recursive_mutex> lock(m_dataMutex); // We must protect for the register been updated just when we read it

  reg->LastReadFromSut(readData);
}
//
//  End of: SystemModelManager::iGet
//---------------------------------------------------------------------------

void SystemModelManager::iGet (string_view registerPath, BinaryVector& readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, uint8_t&      readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, uint16_t&     readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, uint32_t&     readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, uint64_t&     readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, int8_t&       readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, int16_t&      readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, int32_t&      readData) { iGet_impl(registerPath, readData); }
void SystemModelManager::iGet (string_view registerPath, int64_t&      readData) { iGet_impl(registerPath, readData); }


//! Queues data to be read from SUT without checking the value
//!
template<typename T>
void SystemModelManager::iGetRefresh_impl (string_view registerPath, T& readData)
{
  iRefresh(registerPath);
  iApply();
  iGet_impl(registerPath, readData);
}
//
//  End of: SystemModelManager::iGetRefresh
//---------------------------------------------------------------------------


void SystemModelManager::iGetRefresh (string_view registerPath, BinaryVector& readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, uint8_t&      readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, uint16_t&     readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, uint32_t&     readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, uint64_t&     readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, int8_t&       readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, int16_t&      readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, int32_t&      readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager::iGetRefresh (string_view registerPath, int64_t&      readData) { iGetRefresh_impl(registerPath, readData); }


//! Returns the number of expected read failure for a single Register
//!
//! @param registerPath Path to the register
//! @param clearCounter When true, the mismatch counter is reset
//!
uint32_t SystemModelManager::iGetStatus (string_view registerPath, bool clearCounter)
{
  auto& pathResolver = PATH_RESOLVER("iGetStatus: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);

  unique_lock<recursive_mutex> lock(m_dataMutex); // We must protect for the register been updated just when we read it

  auto count = reg->Mismatches();
  if (clearCounter)
  {
    reg->ResetMismatches();
  }

  return count;
}
//
//  End of: SystemModelManager::iGetStatus
//---------------------------------------------------------------------------



//! Returns current path prefix for current thread
//!
string SystemModelManager::iPrefix () const
{
  auto&  pathResolver = PATH_RESOLVER("iPrefix: ");
  return pathResolver.Prefix();
}
//
//  End of: SystemModelManager::iPrefix
//---------------------------------------------------------------------------


//! Changes path prefix for calling thread
//!
void SystemModelManager::iPrefix (std::string prefix)
{
  auto&  pathResolver = MUTABLE_PATH_RESOLVER("iPrefix: ");
  pathResolver.SetPrefix(std::move(prefix));
}
//
//  End of: SystemModelManager::iPrefix
//---------------------------------------------------------------------------


//! Queues a request to (re-)read register value from SUT giving an expected value.
//!
//! @param registerPath     Register path (relative to the last iPrefix or node associated with application thread)
//! @param expectedValue    Value expected to be read from SUT (may contain don't care bits)
//!
template<typename T>
void SystemModelManager::iRead_impl (string_view registerPath, T value)
{
  LOG(DEBUG) << "iRead - Entering";

  //! @todo [JFC]-[August/02/2016]: In iRead_impl(): Add support for don't care
  //!
  auto& pathResolver = PATH_RESOLVER("iRead: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);

  auto asBinaryVector = BinaryVector(reg->BitsCount(), 0u, SizeProperty::Fixed);
  asBinaryVector.Set(std::move(value));

  auto appData = ThreadApplicationData();
  appData->queuedReads.emplace_back(SystemModelManager::QueuedRequest(reg->Identifier(), std::move(asBinaryVector)));

  *appData->currentState = ApplicationData::State::ReadRequest;

  LOG(DEBUG) << "iRead - Leaving";
}
//
//  End of: SystemModelManager::iRead_impl
//---------------------------------------------------------------------------


void SystemModelManager::iRead (string_view registerPath, BinaryVector expectedValue) { iRead_impl(registerPath, std::move(expectedValue)); }
void SystemModelManager::iRead (string_view registerPath, uint8_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager::iRead (string_view registerPath, uint16_t     expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager::iRead (string_view registerPath, uint32_t     expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager::iRead (string_view registerPath, uint64_t     expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager::iRead (string_view registerPath, int8_t       expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager::iRead (string_view registerPath, int16_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager::iRead (string_view registerPath, int32_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager::iRead (string_view registerPath, int64_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }


//! Queues a request to (re-)read register value from SUT
//!
//! @param registerPath     Register path (relative to the last iPrefix or node associated with application thread)
//!
void SystemModelManager::iRefresh (string_view registerPath)
{
  LOG(DEBUG) << "iRefresh - Entering";

  auto& pathResolver = PATH_RESOLVER("iRefresh: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);

  auto appData = ThreadApplicationData();
  appData->queuedRefreshes.emplace_back(SystemModelManager::QueuedRequest(reg->Identifier()));

  *appData->currentState = ApplicationData::State::RefreshRequest;

  LOG(DEBUG) << "iRefresh - Leaving";
}
//
//  End of: SystemModelManager::iRefresh
//---------------------------------------------------------------------------


//! Sets next Register value to sent to SUT
//!
template<typename T>
void SystemModelManager::iWrite_impl (string_view registerPath, T value)
{
  LOG(DEBUG) << "iWrite - Entering";

  auto& pathResolver = PATH_RESOLVER("iWrite: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);


  auto asBinaryVector = BinaryVector(reg->BitsCount(), 0u, SizeProperty::Fixed);
  asBinaryVector.Set(std::move(value));

  auto appData = ThreadApplicationData();
  appData->queuedWrites.emplace_back(SystemModelManager::QueuedRequest(reg->Identifier(), std::move(asBinaryVector)));

  *appData->currentState = ApplicationData::State::WriteRequest;

  LOG(DEBUG) << "iWrite - Leaving";
}
//
//  End of: SystemModelManager::iWrite_impl
//---------------------------------------------------------------------------


//! Sets next Register value to sent to SUT
//!
void SystemModelManager::iWrite (string_view registerPath, BinaryVector value) { iWrite_impl(registerPath, std::move(value)); }

void SystemModelManager::iWrite (string_view registerPath, uint8_t  value) { iWrite_impl(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, uint16_t value) { iWrite_impl(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, uint32_t value) { iWrite_impl(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, uint64_t value) { iWrite_impl(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int8_t   value) { iWrite_impl(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int16_t  value) { iWrite_impl(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int32_t  value) { iWrite_impl(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int64_t  value) { iWrite_impl(registerPath, value); }

//! Runs data cyles when some application thread(s) are pending (in iApply)
//!
//! @note Returns when Stop is called
void SystemModelManager::LoopOnDataCycle ()
{
    //+ (JFC July/06/2016): Start directly with data cycle to get SUT state ?
  MONITOR_MESSAGE("Entering data cycle loop");
  m_loopStarted = true; // Report that the thread has effectively been started
  while (m_runLoop)
  {
    auto needDataCycle = false;
    {
      unique_lock<recursive_mutex> lock(m_dataMutex);
      needDataCycle = m_pendingThreads.size() != 0;
    } // Really need to unlock the mutex because DoDataCycles_Impl need to release it to blocked application threads

    if (needDataCycle)
    {
      DoDataCycles_Impl();
    }

    {
      // ---------------- Wait on new iApply or request to stop
      //
      std::unique_lock<std::mutex> lock(m_loopMutex);
      m_loopCV.wait_for(lock, m_dataCycleLoopTimeout, [this] { return !m_runLoop || (m_pendingThreads.size() != 0); });
    }
  }

  MONITOR_MESSAGE("Exiting data cycle loop");
}
//
//  End of: SystemModelManager::LoopOnDataCycle
//---------------------------------------------------------------------------



//! Returns path resolver associated with caller thread
//!
const NodePathResolver& SystemModelManager::PathResolver (const char* file, const char* fct, uint32_t line, string_view msg) const
{
  auto threadId = std::this_thread::get_id();

  if (threadId == m_managerThreadId)
  {
    return m_pathResolver;
  }

  shared_lock<shared_timed_mutex> lock(m_appDataMutex); // Shared lock is enough for read concurrency

  auto pos = m_threadToAppData.find(threadId);
  if (pos == m_threadToAppData.cend())
  {
    THROW_IMPL_(file, fct, line, std::logic_error, msg.to_string() +  "Calling thread is not managed by SystemModelManager");
  }

  auto data = pos->second;

  return data->pathResolver;
}
//
//  End of: SystemModelManager::PathResolver
//---------------------------------------------------------------------------



//! Processes queued iWrite, iRead and iRefresh requests
//!
//! @note Must be called only by iApply
//!
void SystemModelManager::ProcessQueuedRequests (shared_ptr<ApplicationData> appData)
{
  unique_lock<recursive_mutex> lock(m_dataMutex);

  // ---------------- Process queued writes
  //
  for (const auto& request : appData->queuedWrites)
  {
    auto reg = m_sm.RegisterWithId(request.regId);
    reg->SetToSut(std::move(request.value));

    bool pendingWrite = reg->NextToSut() != reg->LastToSut();
    if (pendingWrite)
    {
      RegisterPendingThread(reg);
    }
  }
  appData->queuedWrites.clear();

  // ---------------- Process queued refreshes
  //
  for (const auto& request : appData->queuedRefreshes)
  {
    auto reg = m_sm.RegisterWithId(request.regId);
    reg->SetPendingForRead(true);

    RegisterPendingThread(reg);
  }
  appData->queuedRefreshes.clear();

  // ---------------- Process queued reads
  //
  for (const auto& request : appData->queuedReads)
  {
    auto reg = m_sm.RegisterWithId(request.regId);
    reg->SetExpectedFromSut(std::move(request.value));
    reg->SetCheckExpected(true);
    reg->SetPendingForRead(true);

    RegisterPendingThread(reg);
  }
  appData->queuedReads.clear();
}
//
//  End of: SystemModelManager::ProcessQueuedRequests
//---------------------------------------------------------------------------



//! Starts, in a background thread, periodical (or on iApply) loop of complete data cycles
//!
//! @note It returns when the background thread is effectively running
//!
void SystemModelManager::Start ()
{
  if (m_managerThread.joinable())
  {
    THROW_RUNTIME_ERROR("There is already a background thread for data cycle loop");
  }

  MONITOR_MESSAGE("Starting data cycle loop background thread");
  m_runLoop = true;
  auto threadFunctor = [this]()
  {
    MONITOR_MESSAGE("Starting background thread");
    LoopOnDataCycle();
    MONITOR_MESSAGE("Exiting background thread");
  };

  m_managerThread   = std::thread(threadFunctor);
  m_managerThreadId = m_managerThread.get_id();
  while (!m_loopStarted)
  {
    std::this_thread::sleep_for(100us);
  }
}
//
//  End of Start
//---------------------------------------------------------------------------


//! Starts all created application threads
//!
//! @note Application thread are configured to wait for a common start
//!       This is done to wait for SystemModel complete creation before
//!       any application try to use it
//!
//! @note When returning, some or all application thread may still be not running
//!
void SystemModelManager::StartCreatedApplicationThreads ()
{
  {
    std::lock_guard<std::mutex> lock(m_appStartMutex);
    m_appStarted = true;

    MONITOR_MESSAGE("Notifying application threads to start");
  }

  m_appStartConditionVar.notify_all();
  MONITOR_MESSAGE("Application threads have been notified to start");
}
//
//  End of: SystemModelManager::StartCreatedApplicationThreads
//---------------------------------------------------------------------------



//! Stops data cycle loop
//!
//! @note When returning, the background thread is effectively terminated
//! @note This should be used precautionaly as it may stop cycles before all data are shifted to SUT
//!
//! @see WaitForApplicationsEnd
void SystemModelManager::Stop ()
{
  {
    std::lock_guard<std::mutex> lock(m_loopMutex);
    m_runLoop = false;

    MONITOR_MESSAGE("Stopping data cycle loop");
  }
  m_loopCV.notify_one();

  if (m_managerThread.joinable())
  {
    m_managerThread.join();
  }

  m_managerThreadId = m_constructionThreadId;
  MONITOR_MESSAGE("Data cycle loop stopped");
}
//
//  End of: SystemModelManager::Stop
//---------------------------------------------------------------------------



//! Releases blocked application thread that have all their pending registers sent to SUT
//!
void SystemModelManager::ReleaseServedThreads ()
{
  unique_lock<recursive_mutex> lock(m_dataMutex);

  for (auto it = m_pendingThreads.begin() ; it != m_pendingThreads.end() ; )
  {
    auto threadId = *it;
    auto appData  = ApplicationDataForThreadId(threadId);

    if (appData->pendingRegistersIds.size() == 0) // Only release thread if it has no more pending registers
    {
      {
        std::lock_guard<std::mutex> lock(appData->releaseMutex);
        appData->canProceed = true;
      }

      // ---------------- Remove the thread id from the set (without invalidating the iteration)
      //
      auto toErasePos = it;
      ++it;
      m_pendingThreads.erase(toErasePos);

      MONITOR_WITH_NODE("Will be released from iApply: ", *appData->pathResolver.ReferenceNode(), appData->debugName)
      appData->releaseCv.notify_one();
    }
    else
    {
      ++it;
    }
  }
}
//
//  End of: SystemModelManager::ReleaseServedThreads
//---------------------------------------------------------------------------



//! Saves the fact that application thread request an operation on a register
//!
//! @note Only applies in multi-thread context
//!
void SystemModelManager::RegisterPendingThread (shared_ptr<Register> reg)
{
  auto threadId = std::this_thread::get_id();
  if (threadId != m_managerThreadId)
  {
    auto  appData    = ThreadApplicationData();
    auto& pendingIds = appData->pendingRegistersIds;
    auto  regId      = reg->Identifier();

    appData->canProceed = false;

    bool alreadyRegistered = pendingIds.count(regId) != 0;
    if (!alreadyRegistered)
    {
      pendingIds.insert(regId);
      m_regIdToAppData.insert(make_pair(regId, appData));
    }
  }
}
//
//  End of: SystemModelManager::RegisterPendingThread
//---------------------------------------------------------------------------


//! Clears "Pending registers" for updated (served) registers
//!
//! @param activeRegisters  SUT Registers that have been updated (during last AccessInterface action)
//!
void SystemModelManager::ReportServedRegisters (const vector<NodeIdentifier>& activeRegisters)
{
  unique_lock<recursive_mutex> lock(m_dataMutex);

  for (const auto& regId : activeRegisters)
  {
    // ---------------- Check must be done once after read
    //
    auto reg = m_sm.RegisterWithId(regId);

    // ---------------- Get all threads pending for that registers
    //
    auto range = m_regIdToAppData.equal_range(regId);

    // ---------------- Report that the register has been updated (served)
    //
    //                  (Normally, only a single thread should be pending for a register)
    //
    for (auto it = range.first ; it != range.second ; ++it)
    {
      auto threadData = it->second;

      threadData->pendingRegistersIds.erase(regId);  // Protected by m_dataMutex
    }

    // ---------------- No more thread(s) pending for that register
    //
    m_regIdToAppData.erase(regId);
  }
}
//
//  End of: SystemModelManager::ReportServedRegisters
//---------------------------------------------------------------------------



//! Waits for all application thread to terminate
//!
void SystemModelManager::WaitForApplicationsEnd ()
{
  // ---------------- Make sure all application have been started
  //                  (not waiting for start signal)
  //
  if (!m_appStarted)
  {
    StartCreatedApplicationThreads(); // Make sure none is still waiting to start
  }

  shared_lock<shared_timed_mutex> lock(m_appDataMutex); // Shared lock is enough for read concurrency
  for (const auto& item : m_threadToAppData)
  {
    auto data    = item.second;
    auto topNode = data->pathResolver.ReferenceNode();

    if (data->appThread.joinable())
    {
      MONITOR_WITH_NODE("Joining application thread", *topNode, data->debugName);
      data->appThread.join();
      MONITOR_WITH_NODE("Joined  application thread", *topNode, data->debugName);
    }
  }
  m_threadToAppData.clear();  // There is no more application threads, so the data are useless
}
//
//  End of: SystemModelManager::WaitForApplicationsEnd
//---------------------------------------------------------------------------


//! Forces a new data cycle (if at least one thread is pending)
//!
void SystemModelManager::WakeupDataCycles ()
{
  std::lock_guard<std::mutex> lock(m_loopMutex);
  m_loopCV.notify_one();
}
//
//  End of: SystemModelManager::WakeupDataCycles
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelManager.cpp
//===========================================================================
