//===========================================================================
//                           SystemModelManager_impl.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManager_impl.cpp
//!
//! Implements class SystemModelManager_impl
//!
//===========================================================================

#include "SystemModelManager_impl.hpp"
#include "SystemModel.hpp"
#include "AccessInterface.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "AccessInterfaceRawProtocol.hpp"
#include "Chain.hpp"
#include "Utility.hpp"
#include "SystemModelManagerMonitor.hpp"
#include "MismatchesCollector.hpp"
#include "SystemModelReseter.hpp"
#include "g3log/g3log.hpp"
#include "DataCycleVisitor.hpp"
#include "AccessInterfaceTranslator.hpp"

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

#define MONITOR(fct)                                 if (m_monitor) m_monitor->fct
#define MONITOR_DEBUG_MANAGER(msg)                   if (m_monitor) m_monitor->LogDebug(msg)
#define MONITOR_DEBUG_APP_LIFE(msg, node, debugName) if (m_monitor) m_monitor->LogDebug(msg, node, debugName)
#define MONITOR_DEBUG_APP(msg, appData)              if (m_monitor) m_monitor->LogDebug(msg, *appData->pathResolver.ReferenceNode(), appData->debugName)

#define MONITOR_PDL(msg, appData)                        if (m_monitor) m_monitor->PDLCommand(msg, "",          *appData->pathResolver.ReferenceNode(), appData->debugName)
#define MONITOR_PDL_EX(msg, path, appData)               if (m_monitor) m_monitor->PDLCommand(msg, path,        *appData->pathResolver.ReferenceNode(), appData->debugName)
#define MONITOR_PDL_AND_VALUE(msg, path, value, appData) if (m_monitor) m_monitor->PDLCommand(msg, path, value, *appData->pathResolver.ReferenceNode(), appData->debugName)


#define PATH_RESOLVER(msg)                                        PathResolver(__FILE__, __func__, __LINE__, msg)
#define MUTABLE_PATH_RESOLVER(msg)  const_cast<NodePathResolver&>(PathResolver(__FILE__, __func__, __LINE__, msg))

//! Joins application threads
//!
SystemModelManager_impl::~SystemModelManager_impl ()
{
  MONITOR_DEBUG_MANAGER("Destructing SystemModelManager (begin)");
  Stop();                         // Stopping data cycle loop also release thread blocked in iApply
  WaitForApplicationsEnd();
  MONITOR_DEBUG_MANAGER("Destructing SystemModelManager (end)");
}
//
//  End of: SystemModelManager_impl::~SystemModelManager_impl
//---------------------------------------------------------------------------


//! Associates a SystemModel to fresh SystemModelManager_impl
//!
SystemModelManager_impl::SystemModelManager_impl(SystemModel&                          sm,
                                                 shared_ptr<ConfigurationAlgorithm>    configurationAlgorithm,
                                                 shared_ptr<SystemModelManagerMonitor> monitor)
  : m_sm                             (sm)
  , m_firstAccessNode           ()
  , m_configurator                   (configurationAlgorithm)
  , m_propagator                     ()
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

  MONITOR_DEBUG_MANAGER("Constructed SystemModelManager");
}

//! Returns application data associated with a thread
//!
//! @param threadId   Identifier of the thread
//!
shared_ptr<SystemModelManager_impl::ApplicationData> SystemModelManager_impl::ApplicationDataForThreadId (std::thread::id threadId) const
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
//  End of: SystemModelManager_impl::ApplicationDataForThreadId
//---------------------------------------------------------------------------


//! Creates an application thread
//!
//! @param applicationTopNode Top most node associated with the application
//! @param functor            Function to call at thread creation
//!
void SystemModelManager_impl::CreateApplicationThread (shared_ptr<ParentNode> applicationTopNode, Application_t functor, string debugName)
{
  CHECK_PARAMETER_NOT_NULL(applicationTopNode, "Cannot create application thread with nullptr reference node (you may have to check node path)");

  MONITOR(CreateApplication(*applicationTopNode, debugName));

  auto wrapper = [this, applicationTopNode, functor](std::shared_ptr<ApplicationData::State> applicationState, string_view debugName)
  {
    // ---------------- Report that the thread has effectively been started
    //
    m_threadStarted = true;
    MONITOR_DEBUG_APP_LIFE("Application thread waiting for start signal", *applicationTopNode, debugName);

    // ---------------- Wait for start "signal"
    //
    auto predicate = [this]{ return m_appStarted.load(); };

    std::unique_lock<std::mutex> lock(m_appStartMutex);
    m_appStartConditionVar.wait(lock, predicate);
    lock.unlock();

    *applicationState = ApplicationData::State::ApplicationThreadStarted;

    // ---------------- To actual application job
    //
    MONITOR_DEBUG_APP_LIFE("Application start", *applicationTopNode, debugName);
    try
    {
      functor();
      *applicationState = ApplicationData::State::Terminated;
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      *applicationState = ApplicationData::State::TerminatedWithException;
      MONITOR_DEBUG_APP_LIFE("Uncaught exception '"s + exc.what() + "' from application", *applicationTopNode, debugName);
    }
    catch (...)
    {
      *applicationState = ApplicationData::State::TerminatedWithException;
      MONITOR_DEBUG_APP_LIFE("Uncaught unknown exception from application", *applicationTopNode, debugName);
    }
    MONITOR_DEBUG_APP_LIFE("Application ends", *applicationTopNode, debugName);
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
  MONITOR_DEBUG_APP("Application thread have reported to be running", data);

  m_threadStarted = false;

  unique_lock<shared_timed_mutex> lock(m_appDataMutex);
  m_threadToAppData[appThreadId] = data;
}
//
//  End of: SystemModelManager_impl::CreateApplicationThread
//---------------------------------------------------------------------------

//! Does a data cycles for a hierarchical subset of the SystemModel
//!
//! @note It does the data cycle only on the target AccessInterface, fromSUTVector and toSUTVector are locally defined
//!
void SystemModelManager_impl::DoHierarchicalDataCycle (AccessInterface* currentAccessInterface)
{
  CHECK_PARAMETER_NOT_NULL(currentAccessInterface, "Houps can only operate on valid AccessInterface");

    auto protocol = currentAccessInterface->Protocol();
  if (currentAccessInterface->IsPending())
  {
    auto protocol = currentAccessInterface->Protocol();
    CHECK_VALUE_NOT_NULL(protocol, "All AccessInterface must be associated with a valid protocol");

    uint32_t endpointId = 1u;
    auto nextEndPoint = currentAccessInterface->FirstChild();

    while (nextEndPoint)
    {
      if (nextEndPoint->IsPending())
      {
        ToSutVisitor local_toSutVisitor;

        nextEndPoint->Accept(local_toSutVisitor);

        const auto& toSutVector = local_toSutVisitor.ToSutVector();

        if (!toSutVector.IsEmpty()) // This can be empty when actual SUT state prevent from serving pending Registers
        {
          const auto& activeRegs = local_toSutVisitor.ActiveRegistersIdentifiers();

          BinaryVector fromSutVector;
          
          fromSutVector = protocol->DoCallback(endpointId, nextEndPoint->ApplicationData(), toSutVector);

          m_fromSutUpdater.UpdateRegisters(activeRegs, fromSutVector);
          ReportServedRegisters(activeRegs);
          ReleaseServedThreads();
        }
      }
      nextEndPoint = nextEndPoint->NextSibling();
      ++endpointId;
    }
  }
   auto raw_protocol =  std::dynamic_pointer_cast<AccessInterfaceRawProtocol>(protocol);
   if (raw_protocol)
    { //We need to release DataCycleVisitor::VisitAccessInterfaceTranslator
    CallbackRequest request(NO_MORE_PENDING);
    raw_protocol->PushRequest(request);
    }
}
//
//  End of: SystemModelManager_impl::DoHierarchicalDataCycle
//---------------------------------------------------------------------------


//! Does a complete data cycles for SystemModel as long as there are pending nodes
//!
//! @note It does the data cycle over each AccessInterface in the SystemModel
//!
void SystemModelManager_impl::DoDataCycles ()
{
  if (std::this_thread::get_id() != m_managerThreadId)
  {
    THROW_RUNTIME_ERROR("DoDataCycles shall be called only on SystemModelManager thread");
  }

  if (!m_firstAccessNode)
  {
    m_firstAccessNode = GetFirstAccessNode(m_sm);
  }

  DoDataCycles_Impl();
}


//! Does a complete data cycles for SystemModel as long as there are pending nodes
//!
//! @note It does the data cycle over each AccessInterface in the SystemModel
//!
void SystemModelManager_impl::DoDataCycles_Impl ()
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
      DataCycleVisitor dataCycleVisitor(this);
      root->Accept(dataCycleVisitor);

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
//  End of: SystemModelManager_impl::DoDataCycles
//---------------------------------------------------------------------------


//! Retrieves first AccessInterface from a SystemModel
//! @note SystemModel root node must be the only AccessInterface or a chain for which all children
//!       are AccessInterface
//!
shared_ptr<ParentNode> SystemModelManager_impl::GetFirstAccessNode (const SystemModel& sm)
{
  auto root = sm.Root();

  CHECK_VALUE_NOT_NULL(root, "Cannot manage an empty system model");

  auto accessInterface = dynamic_pointer_cast<AccessInterface>(root);
  if (!accessInterface)
  {
  auto accessInterfaceTranslator = dynamic_pointer_cast<AccessInterfaceTranslator>(root);
  if (!accessInterfaceTranslator)
   {
    auto asChain = dynamic_pointer_cast<Chain>(root);
    CHECK_VALUE_NOT_NULL(asChain, "SystemModel root must be an AccessInterfaceTranslator, AccessInterface or a Chain (for which chidren are AccessInterface or AccessInterfaceTranslator)");

    accessInterface = dynamic_pointer_cast<AccessInterface>(asChain->FirstChild());
    if (!accessInterface)
     {
     accessInterfaceTranslator = dynamic_pointer_cast<AccessInterfaceTranslator>(asChain->FirstChild());
     CHECK_VALUE_NOT_NULL(accessInterfaceTranslator, "Root (a Chain) must have only AccessInterface or AccessInterfaceTranslator children");
     }
    }
  }

  return root;
}
//
//  End of: SystemModelManager_impl::GetFirstAccessNode
//---------------------------------------------------------------------------




//! Executes queued operations
//!
//! @note It triggers a data cycles and wait for all its pending registers being served
//!
void SystemModelManager_impl::iApply ()
{
  auto appData           = ThreadApplicationData();
  *appData->currentState = ApplicationData::State::InApply;

  MONITOR_PDL("iApply - Processing queued requests", appData);

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

    MONITOR_DEBUG_APP("iApply - May be blocked (if data cycle update application data)", appData);

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
      MONITOR_DEBUG_APP("iApply - Application thread has been released because data cycle loop is not/no more running", appData);
    }
    else
    {
      MONITOR_DEBUG_APP("iApply - Released", appData);
    }
  }

  *appData->currentState = ApplicationData::State::Running;
  MONITOR_DEBUG_APP("iApply - Leaving", appData);
}
//
//  End of: SystemModelManager_impl::iApply
//---------------------------------------------------------------------------



//! Returns last read value from specified register
//!
//! @note This read value is not dependent on an expected value (@see iRead operation)
template<typename T>
void SystemModelManager_impl::iGet_impl (string_view registerPath, T& readData)
{
  auto& pathResolver = PATH_RESOLVER("iGet: ");
  auto reg           = pathResolver.ResolveAsRegister(registerPath);

  {
    unique_lock<recursive_mutex> lock(m_dataMutex); // We must protect for the register been updated just when we read it

    reg->LastFromSut(readData);
  }

  MONITOR_PDL_EX("iGet", registerPath, ThreadApplicationData());
}
//
//  End of: SystemModelManager_impl::iGet_impl
//---------------------------------------------------------------------------

void SystemModelManager_impl::iGet (string_view registerPath, BinaryVector& readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, uint8_t&      readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, uint16_t&     readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, uint32_t&     readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, uint64_t&     readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, int8_t&       readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, int16_t&      readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, int32_t&      readData) { iGet_impl(registerPath, readData); }
void SystemModelManager_impl::iGet (string_view registerPath, int64_t&      readData) { iGet_impl(registerPath, readData); }



//! Returns XOR of the last value read from SUT and the expected value
//!
//! @note May contain x-values (for don't care)
//!
BinaryVector SystemModelManager_impl::iGetMiscompares (string_view registerPath)
{
  auto& pathResolver = PATH_RESOLVER("iGetMiscompares: ");
  auto reg           = pathResolver.ResolveAsRegister(registerPath);

  MONITOR_PDL_EX("iGetMiscompares", registerPath, ThreadApplicationData());

  unique_lock<recursive_mutex> lock(m_dataMutex); // We must protect for the register been updated just when we read it

  return reg->LastCompareResult();
}
//
//  End of: SystemModelManager_impl::iGetMiscompares
//---------------------------------------------------------------------------


//! Queues data to be read from SUT without checking the value
//!
template<typename T>
void SystemModelManager_impl::iGetRefresh_impl (string_view registerPath, T& readData)
{
  MONITOR_PDL_EX("iGetRefresh", registerPath, ThreadApplicationData());
  iRefresh(registerPath);
  iApply();
  iGet_impl(registerPath, readData);
}
//
//  End of: SystemModelManager_impl::iGetRefresh
//---------------------------------------------------------------------------


void SystemModelManager_impl::iGetRefresh (string_view registerPath, BinaryVector& readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, uint8_t&      readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, uint16_t&     readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, uint32_t&     readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, uint64_t&     readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, int8_t&       readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, int16_t&      readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, int32_t&      readData) { iGetRefresh_impl(registerPath, readData); }
void SystemModelManager_impl::iGetRefresh (string_view registerPath, int64_t&      readData) { iGetRefresh_impl(registerPath, readData); }


//! Returns the number of expected read failure for SystemModel sub-tree
//!
//! @param clearCounter When true, the mismatch counter is reset
//!
//! @return The total count of mismatches within the system model
//!
uint32_t SystemModelManager_impl::iGetStatus (shared_ptr<SystemModelNode> node, bool clearCounter)
{
  CHECK_PARAMETER_NOT_NULL(node, "Cannot get status from undefined node");

  auto mismatchesCollector = MismatchesCollector(clearCounter);
  {
    unique_lock<recursive_mutex> lock(m_dataMutex); // We must protect for the register been updated just when we read it
    node->Accept(mismatchesCollector);
  }

  return mismatchesCollector.MismatchesCount();
}
//
//  End of: SystemModelManager_impl::iGetStatus
//---------------------------------------------------------------------------


//! Returns the number of expected read failures from SystemModel sub-tree
//!
//! @note sub-tree can be a single Register or complete SystemModel (node path = ".")
//!
//! @param nodePath     Path to top node of sub-tree
//! @param clearCounter When true, the mismatch counters are reset
//!
//! @return The total count of mismatches for the sub-tree
//!
uint32_t SystemModelManager_impl::iGetStatus (string_view nodePath, bool clearCounter)
{
  auto& pathResolver = PATH_RESOLVER("iGetStatus: ");
  auto  node         = pathResolver.Resolve(nodePath);

  MONITOR_PDL_EX("iGetStatus", nodePath, ThreadApplicationData());

  return iGetStatus(node, clearCounter);
}
//
//  End of: SystemModelManager_impl::iGetStatus
//---------------------------------------------------------------------------


//! Returns the number of expected read failure for the complete system model
//!
//! @param clearCounter When true, the mismatch counter is reset
//!
//! @return The total count of mismatches within the system model
//!
uint32_t SystemModelManager_impl::iGetStatus (bool clearCounter)
{
  MONITOR_PDL_EX("iGetStatus", "SUT Root", ThreadApplicationData());

  return iGetStatus(m_sm.Root(), clearCounter);
}
//
//  End of: SystemModelManager_impl::iGetStatus
//---------------------------------------------------------------------------



//! Returns current path prefix for current thread
//!
string SystemModelManager_impl::iPrefix ()
{
  auto&  pathResolver = PATH_RESOLVER("iPrefix: ");
  return pathResolver.Prefix();
}
//
//  End of: SystemModelManager_impl::iPrefix
//---------------------------------------------------------------------------


//! Changes path prefix for calling thread
//!
void SystemModelManager_impl::iPrefix (std::string prefix)
{
  MONITOR_PDL_EX("iPrefix ", prefix, ThreadApplicationData());

  auto&  pathResolver = MUTABLE_PATH_RESOLVER("iPrefix: ");
  pathResolver.SetPrefix(std::move(prefix));
}
//
//  End of: SystemModelManager_impl::iPrefix
//---------------------------------------------------------------------------


//! Queues a request to (re-)read register value from SUT giving an expected value.
//!
//! @param registerPath     Register path (relative to the last iPrefix or node associated with application thread)
//! @param expectedValue    Value expected to be read from SUT (may contain don't care bits if specified as string)
//!
template<typename T>
void SystemModelManager_impl::iRead_impl (string_view registerPath, T expectedValue)
{
  auto& pathResolver = PATH_RESOLVER("iRead: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);

  auto expectedAsBV  = BinaryVector(reg->BitsCount(), 0u, SizeProperty::Fixed);
  expectedAsBV.Set(std::move(expectedValue));

  auto appData = ThreadApplicationData();

  MONITOR_PDL_AND_VALUE("iRead - Queuing request", registerPath, expectedAsBV, appData);

  appData->queuedReads.emplace_back(SystemModelManager_impl::QueuedRequest(reg->Identifier(), std::move(expectedAsBV)));

  *appData->currentState = ApplicationData::State::ReadRequest;

  MONITOR_DEBUG_APP("iRead - Leaving", appData);
}
//
//  End of: SystemModelManager_impl::iRead_impl
//---------------------------------------------------------------------------


//! Queues a request to (re-)read register value from SUT giving an expected value.
//!
//! @param registerPath     Register path (relative to the last iPrefix or node associated with application thread)
//! @param expectedValue    Value expected to be read from SUT (may contain don't care bits if specified as string)
//! @param dontCareMask     Mask with 1s for cared-for bits and 0s for don't care bits
//!
template<typename T>
void SystemModelManager_impl::iRead_impl (string_view registerPath, T expectedValue, T dontCareMask)
{
  auto& pathResolver = PATH_RESOLVER("iRead: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);

  auto expectedAsBV  = BinaryVector(reg->BitsCount(), 0u, SizeProperty::Fixed);
  auto dontCareAsBV  = BinaryVector(reg->BitsCount(), 0u, SizeProperty::Fixed);
  expectedAsBV.Set(std::move(expectedValue));
  dontCareAsBV.Set(std::move(dontCareMask));

  auto appData = ThreadApplicationData();

  MONITOR_PDL_AND_VALUE("iRead with don't care - Queuing request", registerPath, expectedAsBV, appData);


  appData->queuedReads.emplace_back(SystemModelManager_impl::QueuedRequest(reg->Identifier(),
                                                                      std::move(expectedAsBV),
                                                                      std::move(dontCareAsBV)));

  *appData->currentState = ApplicationData::State::ReadRequest;

  MONITOR_DEBUG_APP("iRead - Leaving", appData);
}
//
//  End of: SystemModelManager_impl::iRead_impl
//---------------------------------------------------------------------------


void SystemModelManager_impl::iRead (string_view registerPath, BinaryVector expectedValue) { iRead_impl(registerPath, std::move(expectedValue)); }
void SystemModelManager_impl::iRead (string_view registerPath, uint8_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager_impl::iRead (string_view registerPath, uint16_t     expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager_impl::iRead (string_view registerPath, uint32_t     expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager_impl::iRead (string_view registerPath, uint64_t     expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager_impl::iRead (string_view registerPath, int8_t       expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager_impl::iRead (string_view registerPath, int16_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager_impl::iRead (string_view registerPath, int32_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }
void SystemModelManager_impl::iRead (string_view registerPath, int64_t      expectedValue) { iRead_impl(registerPath, expectedValue);            }

void SystemModelManager_impl::iRead (string_view registerPath, BinaryVector expectedValue, BinaryVector dontCareMask) { iRead_impl(registerPath, std::move(expectedValue), std::move(dontCareMask)); }
void SystemModelManager_impl::iRead (string_view registerPath, uint8_t      expectedValue, uint8_t      dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }
void SystemModelManager_impl::iRead (string_view registerPath, uint16_t     expectedValue, uint16_t     dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }
void SystemModelManager_impl::iRead (string_view registerPath, uint32_t     expectedValue, uint32_t     dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }
void SystemModelManager_impl::iRead (string_view registerPath, uint64_t     expectedValue, uint64_t     dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }
void SystemModelManager_impl::iRead (string_view registerPath, int8_t       expectedValue, int8_t       dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }
void SystemModelManager_impl::iRead (string_view registerPath, int16_t      expectedValue, int16_t      dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }
void SystemModelManager_impl::iRead (string_view registerPath, int32_t      expectedValue, int32_t      dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }
void SystemModelManager_impl::iRead (string_view registerPath, int64_t      expectedValue, int64_t      dontCareMask) { iRead_impl(registerPath, expectedValue,            dontCareMask);            }

//! Queues a request to (re-)read register value from SUT
//!
//! @param registerPath     Register path (relative to the last iPrefix or node associated with application thread)
//!
void SystemModelManager_impl::iRefresh (string_view registerPath)
{
  auto& pathResolver = PATH_RESOLVER("iRefresh: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);

  auto appData = ThreadApplicationData();
  MONITOR_PDL_EX("iRefresh - Queuing request", registerPath, appData);

  appData->queuedRefreshes.emplace_back(SystemModelManager_impl::QueuedRequest(reg->Identifier()));

  *appData->currentState = ApplicationData::State::RefreshRequest;

  MONITOR_DEBUG_APP("iRefresh - Leaving", appData);
}
//
//  End of: SystemModelManager_impl::iRefresh
//---------------------------------------------------------------------------


//! Forces the ResetPort to be asserted on the target module and reset SystemModel accordingly
//!
//! @param doSynchronousReset When true, reset will be done by issuing a synchronous reset sequence
//!
void SystemModelManager_impl::iReset (bool doSynchronousReset)
{
  auto rootNode = m_sm.Root();

  if (std::this_thread::get_id() != m_constructionThreadId)
  {
    auto appData = ThreadApplicationData();
    auto refNode = appData->pathResolver.ReferenceNode();
    CHECK_VALUE_NOT_NULL(refNode, "Application thread should always be associated with some 'top' node");

    if (refNode != rootNode)
    {
      LOG(ERROR_LVL) << "iReset cannot be called by application not associated with SystemModel root node";
      return;
    }
  }

  SystemModelReseter reseter(doSynchronousReset);

  rootNode->Accept(reseter);
}
//
//  End of: SystemModelManager_impl::iReset
//---------------------------------------------------------------------------


//! Sets next Register value to sent to SUT
//!
template<typename T>
void SystemModelManager_impl::iWrite_impl (string_view registerPath, T value)
{
  auto& pathResolver  = PATH_RESOLVER("iWrite: ");
  auto  reg           = pathResolver.ResolveAsRegister(registerPath);
  auto asBinaryVector = BinaryVector(reg->BitsCount(), 0u, SizeProperty::Fixed);
  asBinaryVector.Set(std::move(value));

  auto appData = ThreadApplicationData();

  MONITOR_PDL_AND_VALUE("iWrite - Queuing request", registerPath, asBinaryVector, appData);
  appData->queuedWrites.emplace_back(SystemModelManager_impl::QueuedRequest(reg->Identifier(), std::move(asBinaryVector)));

  *appData->currentState = ApplicationData::State::WriteRequest;

  MONITOR_DEBUG_APP("iWrite - Leaving", appData);
}
//
//  End of: SystemModelManager_impl::iWrite_impl
//---------------------------------------------------------------------------


//! Sets next Register value to sent to SUT
//!
void SystemModelManager_impl::iWrite (string_view registerPath, BinaryVector value) { iWrite_impl(registerPath, std::move(value)); }

void SystemModelManager_impl::iWrite (string_view registerPath, uint8_t  value) { iWrite_impl(registerPath, value); }
void SystemModelManager_impl::iWrite (string_view registerPath, uint16_t value) { iWrite_impl(registerPath, value); }
void SystemModelManager_impl::iWrite (string_view registerPath, uint32_t value) { iWrite_impl(registerPath, value); }
void SystemModelManager_impl::iWrite (string_view registerPath, uint64_t value) { iWrite_impl(registerPath, value); }
void SystemModelManager_impl::iWrite (string_view registerPath, int8_t   value) { iWrite_impl(registerPath, value); }
void SystemModelManager_impl::iWrite (string_view registerPath, int16_t  value) { iWrite_impl(registerPath, value); }
void SystemModelManager_impl::iWrite (string_view registerPath, int32_t  value) { iWrite_impl(registerPath, value); }
void SystemModelManager_impl::iWrite (string_view registerPath, int64_t  value) { iWrite_impl(registerPath, value); }

//! Runs data cyles when some application thread(s) are pending (in iApply)
//!
//! @note Returns when Stop is called
void SystemModelManager_impl::LoopOnDataCycle ()
{
    //+ (JFC July/06/2016): Start directly with data cycle to get SUT state ?
  MONITOR_DEBUG_MANAGER("Entering data cycle loop");
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

  MONITOR_DEBUG_MANAGER("Exiting data cycle loop");
}
//
//  End of: SystemModelManager_impl::LoopOnDataCycle
//---------------------------------------------------------------------------



//! Returns path resolver associated with caller thread
//!
const NodePathResolver& SystemModelManager_impl::PathResolver (const char* file, const char* fct, uint32_t line, string_view msg)
{
  auto threadId = std::this_thread::get_id();

  if (threadId == m_managerThreadId)
  {
    if (!m_pathResolver.ReferenceNode())
    {
      m_pathResolver.ReferenceNode(m_sm.Root());
    }
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
//  End of: SystemModelManager_impl::PathResolver
//---------------------------------------------------------------------------



//! Processes queued iWrite, iRead and iRefresh requests
//!
//! @note Must be called only by iApply
//!
void SystemModelManager_impl::ProcessQueuedRequests (shared_ptr<ApplicationData> appData)
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
    reg->SetExpectedFromSut(std::move(request.value), std::move(request.mask));
    reg->SetCheckExpected(true);
    reg->SetPendingForRead(true);

    RegisterPendingThread(reg);
  }
  appData->queuedReads.clear();
}
//
//  End of: SystemModelManager_impl::ProcessQueuedRequests
//---------------------------------------------------------------------------



//! Starts, in a background thread, periodical (or on iApply) loop of complete data cycles
//!
//! @note It returns when the background thread is effectively running
//!
void SystemModelManager_impl::Start ()
{
  if (m_managerThread.joinable())
  {
    THROW_RUNTIME_ERROR("There is already a background thread for data cycle loop");
  }

  if (!m_firstAccessNode)
  {
    m_firstAccessNode = GetFirstAccessNode(m_sm);
  }

  MONITOR_DEBUG_MANAGER("Starting data cycle loop background thread");
  m_runLoop = true;
  auto threadFunctor = [this]()
  {
    MONITOR(StartBackgroundThread());

    try
    {
      LoopOnDataCycle();
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      LOG(ERROR_LVL) << "SystemModelManager background thread caught std::exception: " << exc.what();
      {
        std::lock_guard<std::mutex> lock(m_loopMutex);
        m_runLoop = false;
      }
      m_loopCV.notify_one();
    }
    catch (...)
    {
      LOG(ERROR_LVL) << "Unknown exception caught by SystemModelManager background thread";
      {
        std::lock_guard<std::mutex> lock(m_loopMutex);
        m_runLoop = false;
      }
      m_loopCV.notify_one();
    }
    LOG(DEBUG) << "Exiting SystemModelManager background thread";
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
void SystemModelManager_impl::StartCreatedApplicationThreads ()
{
  {
    std::lock_guard<std::mutex> lock(m_appStartMutex);
    m_appStarted = true;

    MONITOR_DEBUG_MANAGER("Notifying application threads to start");
  }

  m_appStartConditionVar.notify_all();
  MONITOR_DEBUG_MANAGER("Application threads have been notified to start");
}
//
//  End of: SystemModelManager_impl::StartCreatedApplicationThreads
//---------------------------------------------------------------------------



//! Stops data cycle loop
//!
//! @note When returning, the background thread is effectively terminated
//! @note This should be used precautionaly as it may stop cycles before all data are shifted to SUT
//!
//! @see WaitForApplicationsEnd
void SystemModelManager_impl::Stop ()
{
  {
    std::lock_guard<std::mutex> lock(m_loopMutex);
    m_runLoop = false;

    MONITOR_DEBUG_MANAGER("Stopping data cycle loop");
  }
  m_loopCV.notify_one();

  if (m_managerThread.joinable())
  {
    m_managerThread.join();
  }

  m_managerThreadId = m_constructionThreadId;
  MONITOR_DEBUG_MANAGER("Data cycle loop stopped");
}
//
//  End of: SystemModelManager_impl::Stop
//---------------------------------------------------------------------------



//! Releases blocked application thread that have all their pending registers sent to SUT
//!
void SystemModelManager_impl::ReleaseServedThreads ()
{
  // Not compatible with multithread DataCycle
  //unique_lock<recursive_mutex> lock(m_dataMutex);

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

      MONITOR_DEBUG_APP("iApply - Notified from data cycle internal", appData);
      appData->releaseCv.notify_one();
    }
    else
    {
      ++it;
    }
  }
}
//
//  End of: SystemModelManager_impl::ReleaseServedThreads
//---------------------------------------------------------------------------



//! Saves the fact that application thread request an operation on a register
//!
//! @note Only applies in multi-thread context
//!
void SystemModelManager_impl::RegisterPendingThread (shared_ptr<Register> reg)
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
//  End of: SystemModelManager_impl::RegisterPendingThread
//---------------------------------------------------------------------------


//! Clears "Pending registers" for updated (served) registers
//!
//! @param activeRegisters  SUT Registers that have been updated (during last AccessInterface action)
//!
void SystemModelManager_impl::ReportServedRegisters (const vector<NodeIdentifier>& activeRegisters)
{
// Not compatible with multithread DataCycle
//  unique_lock<recursive_mutex> lock(m_dataMutex);

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
//  End of: SystemModelManager_impl::ReportServedRegisters
//---------------------------------------------------------------------------



//! Waits for all application thread to terminate
//!
void SystemModelManager_impl::WaitForApplicationsEnd ()
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
      MONITOR_DEBUG_APP("Joining application thread", data);
      data->appThread.join();
      MONITOR_DEBUG_APP("Joined  application thread", data);
    }
  }
  m_threadToAppData.clear();  // There is no more application threads, so the data are useless
}
//
//  End of: SystemModelManager_impl::WaitForApplicationsEnd
//---------------------------------------------------------------------------


//! Forces a new data cycle (if at least one thread is pending)
//!
void SystemModelManager_impl::WakeupDataCycles ()
{
  std::lock_guard<std::mutex> lock(m_loopMutex);
  m_loopCV.notify_one();
}
//
//  End of: SystemModelManager_impl::WakeupDataCycles
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelManager_impl.cpp
//===========================================================================
