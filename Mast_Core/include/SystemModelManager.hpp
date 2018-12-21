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

#include "Mast_Core_export.hpp"
#include "ConfigureAlgorithm_LastOrDefault.hpp"
#include <memory>
#include <vector>
#include <string>
#include <experimental/string_view>
#include <chrono>

namespace mast
{
class AccessInterface;
class SystemModel;
class SystemModelNode;
class ParentNode;
class SystemModelManagerMonitor;
class SystemModelManager_impl;
class ConfigurationAlgorithm;
class BinaryVector;

//! Coordinates the dynamic aspects of the system model
//!
//! @note The main task is to trigger configuration of the system model,
//!       collect the bitstream to sent to SUT, retrieve the bitstream
//!       from SUT and updating the system model with it
//!
class MAST_CORE_EXPORT SystemModelManager final
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelManager() = default;
  SystemModelManager()  = delete;

  using string_view = std::experimental::string_view;

  //! Associates a SystemModel to fresh SystemModelManager
  //!
  SystemModelManager(SystemModel& sm,
                     std::shared_ptr<ConfigurationAlgorithm>    configurationAlgorithm = std::make_shared<ConfigureAlgorithm_LastOrDefault>(),
                     std::shared_ptr<SystemModelManagerMonitor> monitor                = nullptr
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
  void CreateApplicationThread(std::shared_ptr<ParentNode> applicationTopNode, Application_t functor, std::string debugName = "");

  //! Waits for all application thread to terminate
  //!
  void WaitForApplicationsEnd ();

  //! Starts all created application threads
  //!
  void StartCreatedApplicationThreads ();

  //! Waits for all threads to be pendingbefore  triggering a DataCycle (default is false)
  //!
  void setwaitFullPending (bool waitFullPending) ;

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
  BinaryVector iGet (string_view registerPath);

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
  BinaryVector iGetRefresh (string_view registerPath);

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
  std::chrono::milliseconds DataCycleLoopTimeout() const;

  //! Sets maximum time between an iApply and the next data cycle
  //!
  void DataCycleLoopTimeout (std::chrono::milliseconds dataCycleLoopTimeout);

  //! Returns current sleep time between two SystemModel configurations (to let application threads to continue their job)
  //!
  std::chrono::milliseconds SleepTimeBetweenConfigurations() const;

  //! Sets sleep time between two SystemModel configurations (to let application threads to continue their job)
  //!
  void SleepTimeBetweenConfigurations (std::chrono::milliseconds sleepTimeBetweenConfigurations);

  //! Returns instance that monitor manager main actions
  //!
  std::shared_ptr<SystemModelManagerMonitor>  Monitor() const;

  //! Sets instance that monitor manager main actions
  //!
  void  Monitor (std::shared_ptr<SystemModelManagerMonitor> monitor);

  //! Exceptions thrown by PDL applications (on their own thread)
  //!
  const std::vector<std::exception_ptr>& ApplicationsExceptions() const;

  // ---------------- Private Fields
  //
  private:
  std::shared_ptr<SystemModelManager_impl>  m_impl; //!< The actual SystemModelManager
};
//
//  End of SystemModelManager class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_

//===========================================================================
// End of SystemModelManager.hpp
//===========================================================================
