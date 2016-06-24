//===========================================================================
//                           LoggerSinks.cpp
//===========================================================================
//
//! @file LoggerSinks.cpp
//!
//! Implements some LoggerSinks classes
//===========================================================================


#include "LoggerSinks.h"
#include <thread>
#include <chrono>
#include <sstream>

using namespace std::literals::chrono_literals;
using g3::LoggerSink;
using g3::UnitTestsLoggerSink;
using g3::CoutLoggerSink;

std::weak_ptr<g3::SinkHandle<g3::UnitTestsLoggerSink>> g3::UnitTestsLoggerSink::sm_sinkHandle;

//! Enables/Disables collection of log messages
//!
bool LoggerSink::Enabled(bool enabled)
{
  auto oldValue = m_enabled;

  m_enabled = enabled;

  return oldValue;
}


//! Collects (or not) a log message
//!
void UnitTestsLoggerSink::ReceiveLogMessage(g3::LogMessageMover&& logMessage)
{
  if (IsEnabled())
  {
    m_logs.emplace_back(Formatter().FormatAsStdString(std::move(logMessage)));;
  }
}

//! Waits until there is specified count of collected log item
//!
//! @note Cannot be implemented by UnitTestsLoggerSink instance because it would be serialized with log messages (by so always timing out)
//! @note In practice, in single threaded context, as calls to UnitTestsLoggerSink instance are serialized, there is no real need to wait
//!
int UnitTestsLoggerSink::WaitTilLogCount (UnitTestsLoggerSink::TSinkHandle sinkHandle, int count)
{
  auto startTime = std::chrono::high_resolution_clock::now();
  auto endTime   = startTime + std::chrono::milliseconds(50);

  int currentCount = 0;
  while ((currentCount = sinkHandle->call(UnitTestsLoggerSink::GetCount).get()) < static_cast<int>(count))
  {
    auto now = std::chrono::high_resolution_clock::now();
    if (now >= endTime)
    {
      break;
    }
    std::this_thread::sleep_for(5ms);
  }

  return currentCount;
}

//! Collects (or not) a log message
//!
void CoutLoggerSink::ReceiveLogMessage(g3::LogMessageMover&& logMessage)
{
  if (IsEnabled())
  {
    std::cout << Formatter().FormatAsStdString(std::move(logMessage)) << std::endl;
  }
}


//===========================================================================
// End of LoggerSinks.cpp
//===========================================================================
