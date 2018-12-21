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
#include "SystemModelManager_impl.hpp"

using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;

using namespace mast;

//! Associates a SystemModel to fresh SystemModelManager
//!
SystemModelManager::SystemModelManager(SystemModel&                          sm,
                                       shared_ptr<ConfigurationAlgorithm>    configurationAlgorithm,
                                       shared_ptr<SystemModelManagerMonitor> monitor)
  : m_impl(make_shared<SystemModelManager_impl>(sm, configurationAlgorithm, monitor))
{
}

//! Does a complete data cycles for SystemModel as long as there are pending nodes
//!
//! @note It encompasses configuration, bitstream for SUT collection, bitstream fro SUT
//!       retrieval and SystemModel updating
//!
void SystemModelManager::DoDataCycles() { m_impl->DoDataCycles(); }

//! Exceptions thrown by PDL applications (on their own thread)
//!
const std::vector<std::exception_ptr>& SystemModelManager::ApplicationsExceptions() const { return m_impl->ApplicationsExceptions(); }

//! Creates an application thread
//!
void SystemModelManager::CreateApplicationThread(shared_ptr<ParentNode> applicationTopNode, Application_t functor, string debugName)
{ m_impl->CreateApplicationThread(applicationTopNode, functor, debugName); }

//! Waits for all application thread to terminate
//!
void SystemModelManager::WaitForApplicationsEnd() { m_impl->WaitForApplicationsEnd(); }

//! Starts all created application threads
//!
void SystemModelManager::StartCreatedApplicationThreads() { m_impl->StartCreatedApplicationThreads(); }

//! Waits for all threads to be pendingbefore  triggering a DataCycle (default is false)
//!
void SystemModelManager::setwaitFullPending (bool waitFullPending) { m_impl->setwaitFullPending(waitFullPending);}
  
//! Starts periodical (or on iApply) loop of complete data cycles on a new thread
//!
void SystemModelManager::Start() { m_impl->Start(); }

//! Stops data cycle loop
//!
void SystemModelManager::Stop() { m_impl->Stop(); }

//! Executes queued operations
//!
void SystemModelManager::iApply() { m_impl->iApply(); }

//! Returns current path prefix for current thread
//!
std::string SystemModelManager::iPrefix() { return m_impl->iPrefix(); }

//! Changes path prefix for calling thread
//!
void  SystemModelManager::iPrefix(std::string prefix){ m_impl->iPrefix(prefix); }

//! Returns last Register value read from SUT
//!
void         SystemModelManager::iGet (string_view registerPath, uint8_t&      readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, uint16_t&     readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, uint32_t&     readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, uint64_t&     readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, int8_t&       readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, int16_t&      readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, int32_t&      readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, int64_t&      readData) { m_impl->iGet(registerPath, readData); }
void         SystemModelManager::iGet (string_view registerPath, BinaryVector& readData) { m_impl->iGet(registerPath, readData); }

BinaryVector SystemModelManager::iGet (string_view registerPath) { return m_impl->iGet(registerPath); }

//! Returns XOR of the last value read from SUT and the expected value
//!
BinaryVector SystemModelManager::iGetMiscompares (string_view registerPath) { return m_impl->iGetMiscompares(registerPath); }

//! Queues data to be read from SUT
//!
void SystemModelManager::iRead (string_view registerPath, BinaryVector expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, uint8_t      expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, uint16_t     expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, uint32_t     expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, uint64_t     expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, int8_t       expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, int16_t      expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, int32_t      expectedValue) { m_impl->iRead(registerPath, expectedValue); }
void SystemModelManager::iRead (string_view registerPath, int64_t      expectedValue) { m_impl->iRead(registerPath, expectedValue); }


void SystemModelManager::iRead (string_view registerPath, BinaryVector expectedValue, BinaryVector dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, uint8_t      expectedValue, uint8_t      dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, uint16_t     expectedValue, uint16_t     dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, uint32_t     expectedValue, uint32_t     dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, uint64_t     expectedValue, uint64_t     dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, int8_t       expectedValue, int8_t       dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, int16_t      expectedValue, int16_t      dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, int32_t      expectedValue, int32_t      dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }
void SystemModelManager::iRead (string_view registerPath, int64_t      expectedValue, int64_t      dontCareMask) { m_impl->iRead(registerPath, expectedValue, dontCareMask); }

//! Queues data to be read from SUT without checking the value
//!
void SystemModelManager::iRefresh (string_view registerPath) { m_impl->iRefresh(registerPath); }

//! Requests register value to be read from SUT and wait till it can be return
//!
void         SystemModelManager::iGetRefresh (string_view registerPath, uint8_t&      readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, uint16_t&     readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, uint32_t&     readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, uint64_t&     readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, int8_t&       readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, int16_t&      readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, int32_t&      readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, int64_t&      readData) { m_impl->iGetRefresh(registerPath, readData); }
void         SystemModelManager::iGetRefresh (string_view registerPath, BinaryVector& readData) { m_impl->iGetRefresh(registerPath, readData); }

BinaryVector SystemModelManager::iGetRefresh (string_view registerPath) { return m_impl->iGetRefresh(registerPath); }

//! Returns the number of expected read failure for SystemModel sub-tree
//!
uint32_t SystemModelManager::iGetStatus (std::shared_ptr<SystemModelNode> node, bool clearCounter) { return m_impl->iGetStatus(node.get(), clearCounter); }


//! Returns the number of expected read failure for the complete system model
//!
uint32_t SystemModelManager::iGetStatus(bool clearCounter) { return m_impl->iGetStatus(clearCounter); }

//! Returns the number of expected read failures from SystemModel sub-tree
//!
uint32_t SystemModelManager::iGetStatus(string_view nodePath, bool clearCounter) { return m_impl->iGetStatus(nodePath, clearCounter); }

//! Forces the ResetPort to be asserted on the target module and reset SystemModel accordingly
//!
void SystemModelManager::iReset(bool doSynchronousReset) { m_impl->iReset(doSynchronousReset); }


//! Sets next Register value to sent to SUT
//!
void SystemModelManager::iWrite (string_view registerPath, BinaryVector value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, uint8_t      value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, uint16_t     value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, uint32_t     value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, uint64_t     value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int8_t       value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int16_t      value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int32_t      value) { m_impl->iWrite(registerPath, value); }
void SystemModelManager::iWrite (string_view registerPath, int64_t      value) { m_impl->iWrite(registerPath, value); }

//! Returns current maximum time between an iApply and the next data cycle
//!
std::chrono::milliseconds SystemModelManager::DataCycleLoopTimeout() const { return m_impl->DataCycleLoopTimeout(); }

//! Sets maximum time between an iApply and the next data cycle
//!
void SystemModelManager::DataCycleLoopTimeout(std::chrono::milliseconds dataCycleLoopTimeout) { m_impl->DataCycleLoopTimeout(dataCycleLoopTimeout); }

//! Returns current sleep time between two SystemModel configurations (to let application threads to continue their job)
//!
std::chrono::milliseconds SystemModelManager::SleepTimeBetweenConfigurations() const { return m_impl->SleepTimeBetweenConfigurations(); }

//! Sets sleep time between two SystemModel configurations (to let application threads to continue their job)
//!
void SystemModelManager::SleepTimeBetweenConfigurations(std::chrono::milliseconds sleepTimeBetweenConfigurations) { m_impl->SleepTimeBetweenConfigurations(sleepTimeBetweenConfigurations); }

//! Returns instance that monitor manager main actions
//!
std::shared_ptr<SystemModelManagerMonitor> SystemModelManager::Monitor() const { return m_impl->Monitor(); }

//! Sets instance that monitor manager main actions
//!
void SystemModelManager::Monitor(std::shared_ptr<SystemModelManagerMonitor> monitor) { m_impl->Monitor(monitor); }


//===========================================================================
// End of SystemModelManager.cpp
//===========================================================================
