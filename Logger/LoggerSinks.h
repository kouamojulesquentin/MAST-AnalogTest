//===========================================================================
//                           LoggerSinks.h
//===========================================================================
//
//! @file LoggerSinks.h
//!
//! Declares LoggerSinks classes
//===========================================================================

#ifndef LOGGERSINKS_H__E0FCE333_42B1_4FD3_9EB5_6C18F858F45F__INCLUDED_
  #define LOGGERSINKS_H__E0FCE333_42B1_4FD3_9EB5_6C18F858F45F__INCLUDED_


#include "g3log/g3log_Export.hpp"
#include "g3log/sinkhandle.hpp"
#include "LogFormatter.h"
#include <vector>
#include <string>

namespace g3
{
  //! Common, abstract, implementation of some logger sinks
  //!
  class G3LOG_DLL_EXPORT LoggerSink
  {
    g3::LogFormatter m_formatter;
    bool             m_enabled = true; //!< To enable/disable message for this logger sink only

    // ---------------- Public  Methods
    //
    public:
    virtual ~LoggerSink() = default;
    LoggerSink()          = default;
    LoggerSink(bool enabled)                                           : m_enabled(enabled) {}
    LoggerSink(const g3::LogFormatter& formatter, bool enabled = true) : m_formatter(formatter), m_enabled(enabled) {}

    bool Enabled(bool enabled);
    bool IsEnabled() const { return m_enabled; }

    g3::LogFormatter& Formatter() { return m_formatter; }
    void Formatter(g3::LogFormatter& formatter) { m_formatter = formatter; }

    virtual void ReceiveLogMessage(g3::LogMessageMover&&) = 0;
  };
  //
  //  End of LoggerSinks class declaration
  //---------------------------------------------------------------------------

  //! Saves each log message into a string list (to be easily checked by unit tests dedicated to logs)
  //!
  class G3LOG_DLL_EXPORT UnitTestsLoggerSink final : public LoggerSink
  {
    std::vector<std::string>                                               m_logs;           //!< Saved log message
    static std::weak_ptr<g3::SinkHandle<UnitTestsLoggerSink>> sm_sinkHandle;    //!< Must not keep an handle when the sink is no more used

    public:

    virtual ~UnitTestsLoggerSink() = default;
             UnitTestsLoggerSink()             : UnitTestsLoggerSink(true) {}
             UnitTestsLoggerSink(bool enabled) : LoggerSink(enabled) {}
             UnitTestsLoggerSink(const g3::LogFormatter& formatter, bool enabled = true) : LoggerSink(formatter, enabled) {}

    void ReceiveLogMessage(g3::LogMessageMover&& logMessage) override;

    void                     Clear()    { m_logs.clear(); }
    int                      GetCount() { return m_logs.size(); }
    std::vector<std::string> GetLogs()  { return m_logs;         } // Returns a copy to be used unambiguously with futures

    using TSinkHandle = std::shared_ptr<g3::SinkHandle<UnitTestsLoggerSink>>;

    //! Saves (single) sink handle
    static void        SetSinkHandle(TSinkHandle sinkHandle) { sm_sinkHandle = sinkHandle; }

    //! Provides lastly saved sink handle
    static TSinkHandle LastSinkHandle()                      { return sm_sinkHandle.lock(); }

    //! Waits until there is specified count of collected log item in sink
    static int         WaitTilLogCount(UnitTestsLoggerSink::TSinkHandle sinkHandle, int count);
  };

  //! Streams all log messages to std::cout
  //!
  class G3LOG_DLL_EXPORT CoutLoggerSink final : public LoggerSink
  {
    public:

    virtual ~CoutLoggerSink() = default;
             CoutLoggerSink()             : CoutLoggerSink(true) {}
             CoutLoggerSink(bool enabled) : LoggerSink(enabled) {}
             CoutLoggerSink(const g3::LogFormatter& formatter, bool enabled = true) : LoggerSink(formatter, enabled) {}

    void ReceiveLogMessage(g3::LogMessageMover&& logMessage) override;
  };


  //! Streams ERROR only log messages to std::cerr
  //!
  class G3LOG_DLL_EXPORT ErrorsOnCerrLoggerSink final : public LoggerSink
  {
    public:

    virtual ~ErrorsOnCerrLoggerSink() = default;
             ErrorsOnCerrLoggerSink()             : ErrorsOnCerrLoggerSink(true) {}
             ErrorsOnCerrLoggerSink(bool enabled) : LoggerSink(enabled) {}
             ErrorsOnCerrLoggerSink(const g3::LogFormatter& formatter, bool enabled = true) : LoggerSink(formatter, enabled) {}

    void ReceiveLogMessage(g3::LogMessageMover&& logMessage) override;
  };



  //! Discards (does nothing with) log messages
  //!
  class G3LOG_DLL_EXPORT DiscardLoggerSink final : public LoggerSink
  {
    public:

             DiscardLoggerSink() = default;
    virtual ~DiscardLoggerSink() = default;

    void ReceiveLogMessage(g3::LogMessageMover&&) override {}
  };

} // End of namespace g3

#endif  // not defined LOGGERSINKS_H__E0FCE333_42B1_4FD3_9EB5_6C18F858F45F__INCLUDED_

//===========================================================================
// End of LoggerSinks.h
//===========================================================================
