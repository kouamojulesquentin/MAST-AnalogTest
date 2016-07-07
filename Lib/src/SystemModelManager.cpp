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


using namespace mast;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::vector;
using std::string;
using std::experimental::string_view;
using std::mutex;
using std::shared_timed_mutex;
using std::lock_guard;
using std::unique_lock;
using std::shared_lock;

#define MONITOR(fct)                     if (m_monitor) m_monitor->fct;
#define MONITOR_MESSAGE(msg)             if (m_monitor) m_monitor->LogUncondionally(msg);
#define MONITOR_WITH_NODE(msg, node)     if (m_monitor) m_monitor->LogUncondionally(msg, node);

#define PATH_RESOLVER(msg)                                        PathResolver(__FILE__, __func__, __LINE__, msg)
#define MUTABLE_PATH_RESOLVER(msg)  const_cast<NodePathResolver&>(PathResolver(__FILE__, __func__, __LINE__, msg))

//! Joins application threads
//!
SystemModelManager::~SystemModelManager ()
{
  JoinAllApplicationThreads();
  Stop();
}
//
//  End of: SystemModelManager::~SystemModelManager
//---------------------------------------------------------------------------


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
    THROW_LOGIC_ERROR("Thread is not managed by SystemModelManager");
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
void SystemModelManager::CreateApplicationThread (shared_ptr<ParentNode> applicationTopNode, Application_t functor)
{
  CHECK_PARAMETER_NOT_NULL(applicationTopNode, "Cannot create application thread with nullptr top node");

  MONITOR(CreateApplication(*applicationTopNode));

  auto wrapper = [this, applicationTopNode, functor]()
  {
    // ---------------- Wait for start "signal"
    //
    std::unique_lock<std::mutex> lock(m_appStartMutex);
    m_appStartConditionVar.wait(lock, [this]{return m_appStarted;});

    // ---------------- To actual application job
    //
    MONITOR_WITH_NODE("Application start with node: ", *applicationTopNode);
    try
    {
      functor();
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      MONITOR_WITH_NODE("Uncaught exception '"s + exc.what() + "' from application with node: ", *applicationTopNode);
    }
    catch (...)
    {
      MONITOR_WITH_NODE("Uncaught unknown exception from application with node: ", *applicationTopNode);
    }
    MONITOR_WITH_NODE("Application end with node: ", *applicationTopNode);
  };

  auto appThread    = std::thread(wrapper);
  auto appThreadId  = appThread.get_id();
  auto pathResolver = NodePathResolver(applicationTopNode);
  auto data         = make_shared<ApplicationData>(std::move(appThread), pathResolver);

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

  do
  {
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

              //+ Release mutex and wait few ms !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }

            nextDerivation = nextDerivation->NextSibling();
            ++derivationId;
          }
        }

        nextAccessInterface = dynamic_pointer_cast<AccessInterface>(nextAccessInterface->NextSibling());;
      }
    }
  } while (doDataCycle);
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


//! Waits for all application thread to terminate
//!
void SystemModelManager::JoinAllApplicationThreads ()
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
      MONITOR_WITH_NODE("Joining application thread associated with node: ", *topNode);
      data->appThread.join();
      MONITOR_WITH_NODE("Joined  application thread associated with node: ", *topNode);
    }
  }
}
//
//  End of: SystemModelManager::JoinAllApplicationThreads
//---------------------------------------------------------------------------


//! Executes queued operations
//!
//! @note If calling application thread has pending registers, it triggers a data cycles and wait for all its
//! pending registers being served
void SystemModelManager::iApply ()
{
  auto threadId = std::this_thread::get_id();

  if (threadId == m_managerThreadId)  // Single thread context?
  {
    DoDataCycles();
  }
  else
  {
    auto appData = ThreadApplicationData();
    {
      unique_lock<shared_timed_mutex> lock(m_pendingThreadMutex);
      m_pendingThreads.insert(threadId);               // Memorize that this is a pending thread

      //! @todo [JFC]-[July/06/2016]: In iApply(): trigger data cycle before blocking
      //!

      appData->canProceed = false;
      WakeupDataCycles();
      if (appData->pendingRegistersIds.size() != 0)
      {
        std::unique_lock<std::mutex> lock(appData->releaseMutex);
        appData->releaseCv.wait(lock, [appData]{ return appData->canProceed; });
      }
    }
  }
}
//
//  End of: SystemModelManager::iApply
//---------------------------------------------------------------------------



//! Returns last read value from specified register
//!
BinaryVector SystemModelManager::iGet (string_view registerPath)
{
  auto& pathResolver = PATH_RESOLVER("iGet: ");
  auto reg           = pathResolver.ResolveAsRegister(registerPath);
  auto readData      = reg->LastFromSut();

  return readData;
}
//
//  End of: SystemModelManager::iGet
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



//! Sets next Register value to sent to SUT
//!
void SystemModelManager::iWrite (string_view registerPath, BinaryVector sequence)
{
  auto& pathResolver = PATH_RESOLVER("iWrite: ");
  auto  reg          = pathResolver.ResolveAsRegister(registerPath);

  reg->SetToSut(std::move(sequence));

  // ---------------- Save the fact that application thread request an operation on that register
  //
  auto threadId = std::this_thread::get_id();
  if (threadId != m_managerThreadId)
  {
    bool isPending = reg->NextToSut() != reg->LastToSut();
    if (isPending)
    {
      // ---------------- Memorize that the register is associated with that thead
      //
      auto  data       = ThreadApplicationData();
      auto& pendingIds = data->pendingRegistersIds;
      auto  regId      = reg->Identifier();

      bool alreadyRegistered = pendingIds.count(regId) == 0;
      if (!alreadyRegistered)
      {
        pendingIds.insert(regId);

        unique_lock<shared_timed_mutex> lock(m_pendingThreadMutex);
        m_regIdToAppData.insert(make_pair(regId, data)); // Memorize that this register is pending for that thread
      }
    }
  }
}
//
//  End of: SystemModelManager::iWrite
//---------------------------------------------------------------------------


//! Runs data cyles when some application thread(s) are pending (in iApply)
//!
//! @note Returns when Stop is called
void SystemModelManager::LoopOnDataCycle ()
{
    //+ (JFC July/06/2016): Start directly with data cycle to get SUT state ?
  while (m_runLoop)
  {
    if (m_pendingThreads.size() != 0)
    {
      DoDataCycles_Impl();
    }

    // ---------------- Wait on new iApply or request to stop
    //
    std::unique_lock<std::mutex> lock(m_loopMutex);
    m_loopCV.wait(lock, [this] { return !m_runLoop || (m_pendingThreads.size() != 0); });
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


//! Starts periodical (or on iApply) loop of complete data cycles on calling thread
//!
//! @note It only returns when StopDataCycleLoop is called, so it shall be called
//!       from a thread differing from those used to create SystemModelManager.
//!
//! @see StartInBackground
//!
void SystemModelManager::Start ()
{
  if (std::this_thread::get_id() == m_managerThreadId)
  {
    THROW_RUNTIME_ERROR("StartDataCycleLoop shall be called only from thread different from SystemModelManager thread");
  }

  m_runLoop = true;
  LoopOnDataCycle();
}
//
//  End of Start
//---------------------------------------------------------------------------


//! Starts periodical (or on iApply) loop of complete data cycles on calling thread
//!
//! @note It only returns when StopDataCycleLoop is called, so it shall be called
//!       from a thread differing from those used to create SystemModelManager.
//!
void SystemModelManager::StartInBackground ()
{
  if (m_managerThread.joinable())
  {
    THROW_RUNTIME_ERROR("There is already a background thread for data cycle loop");
  }

  m_runLoop = true;
  auto threadFunctor = [this]()
  {
    LoopOnDataCycle();
    MONITOR_MESSAGE("Exiting data cycle loop");
  };

  m_managerThread   = std::thread(threadFunctor);
  m_managerThreadId = m_managerThread.get_id();
}
//
//  End of StartInBackground
//---------------------------------------------------------------------------


//! Starts all created application threads
//!
//! @note Application thread are configured to wait for a common start
//!       This is done to wait for SystemModel complete creation before
//!       any application try to use it
//!
void SystemModelManager::StartCreatedApplicationThreads ()
{
  {
    std::lock_guard<std::mutex> lock(m_appStartMutex);
    m_appStarted = true;

    MONITOR_MESSAGE("Starting all created application threads");
  }

  m_appStartConditionVar.notify_all();
}
//
//  End of: SystemModelManager::StartCreatedApplicationThreads
//---------------------------------------------------------------------------



//! Stops data cycle loop
//!
void SystemModelManager::Stop ()
{
  {
    std::lock_guard<std::mutex> lock(m_loopMutex);
    m_runLoop = false;

    MONITOR_MESSAGE("Stopping datacycle loop");
  }

  m_loopCV.notify_one();

  if (m_managerThread.joinable())
  {
    m_managerThread.join();
  }
  m_managerThreadId = m_constructionThreadId;
}
//
//  End of: SystemModelManager::Stop
//---------------------------------------------------------------------------



//! Releases blocked application thread that have all their pending registers sent to SUT
//!
void SystemModelManager::ReleaseServedThreads ()
{
  for (const auto& threadId : m_pendingThreads)
  {
    auto appData = ApplicationDataForThreadId(threadId);

    {
      std::lock_guard<std::mutex> lock(appData->releaseMutex);
      appData->canProceed = true;
    }
    appData->releaseCv.notify_one();
  }
}
//
//  End of: SystemModelManager::ReleaseServedThreads
//---------------------------------------------------------------------------



//! Clears "Pending registers" for updated (served) registers
//!
//! @param activeRegisters  SUT Registers that have been updated (during last AccessInterface action)
//!
void SystemModelManager::ReportServedRegisters (const vector<NodeIdentifier>& activeRegisters)
{
//+  unique_lock<shared_timed_mutex> lock(m_pendingThreadMutex);
  //! @todo [JFC]-[July/06/2016]: In ReportServedRegisters(): Ensure exclusive access to m_regIdToAppData
  //!


  for (const auto& regId : activeRegisters)
  {
    // ---------------- Get all threads pending for that registers
    //
    auto range = m_regIdToAppData.equal_range(regId);

    // ---------------- Report that the register has been update (served)
    //
    //                  (Normally, only a single thread should be pending for a register)
    //
    for (auto it = range.first ; it != range.second ; ++it)
    {
      auto threadData = it->second;

      //! @todo [JFC]-[July/06/2016]: In ReportServedRegisters(): Ensure exclusive access to pendingRegistersIds
      //!
      threadData->pendingRegistersIds.erase(regId);
    }

    // ---------------- No more thread(s) pending for that register
    //
    m_regIdToAppData.erase(regId);
  }
}
//
//  End of: SystemModelManager::ReportServedRegisters
//---------------------------------------------------------------------------


//! Forces a new data cycle (if at least one thread is pending)
//!
void SystemModelManager::WakeupDataCycles ()
{
  {
    std::lock_guard<std::mutex> lock(m_loopMutex);
  }

  m_loopCV.notify_one();
}
//
//  End of: SystemModelManager::WakeupDataCycles
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelManager.cpp
//===========================================================================
