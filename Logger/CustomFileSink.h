//===========================================================================
//                           CustomFileSink.h
//===========================================================================
//
//! @file CustomFileSink.h
//!
//! Declares class CustomFileSink
//===========================================================================


#ifndef CUSTOMFILESINK_H__F1EC3049_DF54_410F_31A5_7FB619CDB364__INCLUDED_
  #define CUSTOMFILESINK_H__F1EC3049_DF54_410F_31A5_7FB619CDB364__INCLUDED_

#include <memory>
#include <string>
#include <fstream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>

#include "g3log_Export.h"
#include "g3log/logmessage.hpp"
#include "g3log/sinkhandle.hpp"
#include "LogFormatter.h"

using namespace std::literals::chrono_literals;


namespace g3
{
  //! Saves pre-formatted log messages
  //!
  //! @note Use std thread (and chrono) for interruption points support
  //!
  class G3LOG_DLL_EXPORT CustomFileSink final
  {
    // ---------------- Public  Methods
    //
    public:

    //! Defines when received log messages are effectively flushed to file
    //!
    enum class FlushMode
    {
      Manual,         //!< Some external code has to call Flush()
      EachMessage,    //!< There is a flush after each received message
      AutoBackground  //!< A background thread flushes every period of time (the default period can be changed)
    };

    using TSinkHandle = std::shared_ptr<g3::SinkHandle<CustomFileSink>>;

    ~CustomFileSink();
    CustomFileSink ();
    CustomFileSink (const std::string& path, FlushMode flushMode = FlushMode::Manual);
    CustomFileSink (const std::string& path, FlushMode flushMode, const LogFormatter& logFormatter);

    CustomFileSink (const CustomFileSink&) = delete;
    CustomFileSink& operator=(const CustomFileSink&) = delete;

    //! Returns whether an EOL is append automatically after each received log message
    //!
    bool  GetAutomaticEol() const { return m_automaticEol; };

    //! Sets whether an EOL is append automatically after  each received log message
    //!
    void  SetAutomaticEol (bool automaticEol) { m_automaticEol = automaticEol; };

    //! Clears current log file (truncates it)
    //!
    void Clear();

    //! Closes current file (to be used when log is disabled to allow the file to be easily edited)
    //!
    void CloseFile();

    //! Flushes to file any not flushed yet log messages
    //!
    void Flush();

    //! Returns current flush mode
    //!
    FlushMode GetFlushMode() const { return m_flushMode; };

    //! Opens associated log file, in APPEND or TRUNC mode
    //!
    bool OpenFile(std::ios_base::openmode truncateOrAppendMode);

    //! Opens associated log file, appending to existing file
    //!
    bool OpenFile_AppendMode();

    //! Sets new flush mode
    //!
    void SetFlushMode (FlushMode flushMode);

    //! Changes file path, truncating specified file if exists.
    //!
    bool SetFilePath(const std::string& path);

    //! Changes log message formatter
    //!
    void SetFormatter(const LogFormatter& logFormatter);

    //! Dumps to file unformatted yet log message
    //!
    void ReceiveLogUnformattedMessage(g3::LogMessageMover&& unformattedMessage);

    //! Dumps to file already formatted log message
    //!
    void ReceiveLogMessage(const std::string& formattedMessage);

    //! Changes flush period (only valid for AutoBackground flush mode)
    //!
    void SetFlushPeriod(const std::chrono::milliseconds& period) { m_flushPeriod = std::chrono::milliseconds(period.count()); }

    //! (Re)Starts saving to file
    //!
    void Start();

    //! Stops saving to file
    //!
    void Stop();

    // ---------------- Private  Methods
    //
    private:

    void Flush_Periodically();
    void StartFlush_Thread();
    void StopFlush_Thread();

    // ---------------- Private  Fields
    //
    private:
    LogFormatter                   m_logFormatter;
    std::string                    m_filePath;
    bool                           m_automaticEol;  //!< If true, a endl is stream after each received log message
    std::unique_ptr<std::ofstream> m_fileStream;
    FlushMode                      m_flushMode;
    std::atomic_bool               m_runFlushTread; //!< Becomes true after each call to ReceiveLogMessage and false just before any flush
    std::atomic_bool               m_mayNeedFlush;  //!< Becomes true after each call to ReceiveLogMessage and false just before any flush
    std::mutex                     m_fileMutex;     //!< Required to serialize message streaming and flush
    std::thread                    m_flushThread;   //!< Only active when flush mode is AutoBackground
    std::chrono::milliseconds      m_flushPeriod;   //!< "Maximum" time between two flushes
  };
  //
  //  End of CustomFileSink class declaration
  //---------------------------------------------------------------------------
} // End of namespace g3

#endif  // not defined CUSTOMFILESINK_H__F1EC3049_DF54_410F_31A5_7FB619CDB364__INCLUDED_

//===========================================================================
// End of CustomFileSink.h
//===========================================================================
