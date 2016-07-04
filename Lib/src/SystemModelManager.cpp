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

#define MONITOR(fct)                     if (m_monitor) m_monitor->fct;
#define MONITOR_MESSAGE(msg)             if (m_monitor) m_monitor->LogUncondionally(msg);
#define MONITOR_WITH_NODE(msg, node)     if (m_monitor) m_monitor->LogUncondionally(msg, node);

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
    functor();
    MONITOR_WITH_NODE("Application end with node: ", *applicationTopNode);
  };

  auto appThread    = std::thread(wrapper);
  auto appThreadId  = appThread.get_id();
  auto pathResolver = NodePathResolver(applicationTopNode);
  auto data         = make_shared<ApplicationData>(std::move(appThread), pathResolver);

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


//! Returns current path prefix for current thread
//!
string SystemModelManager::iPrefix () const
{
  auto threadId = std::this_thread::get_id();

  if (threadId == m_managerThreadId)
  {
    return m_pathResolver.Prefix();
  }

  RETHROW_LOGIC_ERROR(auto data = ApplicationDataForCurrentThread();
                      return data->m_pathResolver.Prefix(), "iPrefix: Calling thread is not managed by SystemModelManager");
}
//
//  End of: SystemModelManager::iPrefix
//---------------------------------------------------------------------------


//! Changes path prefix for calling thread
//!
void SystemModelManager::iPrefix (std::string prefix)
{
  auto threadId = std::this_thread::get_id();
  if (threadId == m_managerThreadId)
  {
    return m_pathResolver.SetPrefix(std::move(prefix));
  }

  RETHROW_LOGIC_ERROR(auto data = ApplicationDataForCurrentThread();
                      data->m_pathResolver.SetPrefix(std::move(prefix)), "iPrefix: Calling thread is not managed by SystemModelManager");
}
//
//  End of: SystemModelManager::iPrefix
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
