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
}
//
//  End of: SystemModelManager::~SystemModelManager
//---------------------------------------------------------------------------


//! Returns application data associated with caller thread
//!
shared_ptr<SystemModelManager::ApplicationData> SystemModelManager::ApplicationDataForCurrentThread () const
{
   shared_lock<shared_timed_mutex> lock(m_appDataMutex); // Shared lock is enough for read concurrency

  auto threadId = std::this_thread::get_id();
  auto pos      = m_applicationsData.find(threadId);

  if (pos == m_applicationsData.cend())
  {
    THROW_LOGIC_ERROR("Calling thread is not managed by SystemModelManager");
  }

  auto data = pos->second;

  return data;
}
//
//  End of: SystemModelManager::ApplicationDataForCurrentThread
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
    std::unique_lock<std::mutex> lk(m_appStartMutex);
    m_appStartConditionVar.wait(lk, [this]{return m_appStarted;});

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
  m_applicationsData[appThreadId] = data;
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
  for (const auto& item : m_applicationsData)
  {
    auto data    = item.second;
    auto topNode = data->m_pathResolver.ReferenceNode();

    if (data->m_thread.joinable())
    {
      MONITOR_WITH_NODE("Joining application thread associated with node: ", *topNode);
      data->m_thread.join();
      MONITOR_WITH_NODE("Joined  application thread associated with node: ", *topNode);
    }
  }
}
//
//  End of: SystemModelManager::JoinAllApplicationThreads
//---------------------------------------------------------------------------


//! Executes queued operations
//!
void SystemModelManager::iApply ()
{
  auto threadId = std::this_thread::get_id();

  if (threadId == m_managerThreadId)  // Single thread context?
  {
    DoDataCycles();
  }
  else
  {
    auto data = ApplicationDataForCurrentThread();


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
  auto reg           = pathResolver.ResolveAsRegister(registerPath);

  reg->SetToSut(std::move(sequence));
}
//
//  End of: SystemModelManager::iWrite
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

  auto pos = m_applicationsData.find(threadId);
  if (pos == m_applicationsData.cend())
  {
    THROW_IMPL_(file, fct, line, std::logic_error, msg.to_string() +  "Calling thread is not managed by SystemModelManager");
  }

  auto data = pos->second;

  return data->m_pathResolver;
}
//
//  End of: SystemModelManager::PathResolver
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


//===========================================================================
// End of SystemModelManager.cpp
//===========================================================================
