//===========================================================================
//                           CustomFileSink.cpp
//===========================================================================
//
//! @file CustomFileSink.cpp
//!
//! Implements class that saves log messages into a file
//===========================================================================


#include "CustomFileSink.h"
#include "g3log/g3log_debug.hpp"

using g3::LogFormatter;
using g3::CustomFileSink;
using std::make_unique;
using namespace std::literals::chrono_literals;


//! Stops flush thread (if active) and flush unflushed data
//!
CustomFileSink::~CustomFileSink ()
{
  COUT_LOG_TIMESTAMP <<  ">>> " << __func__;
  StopFlush_Thread();
  Flush();
  COUT_LOG_TIMESTAMP <<  "<<< " << __func__;
}
//
//  End of fct_CustomFileSink::~CustomFileSink
//---------------------------------------------------------------------------



//! Constructor with specified file path
//!
CustomFileSink::CustomFileSink (const std::string& path, FlushMode flushMode, const LogFormatter& logFormatter)
  : m_logFormatter  (logFormatter)
  , m_filePath      (path)
  , m_automaticEol  (true)
  , m_flushMode     (FlushMode::Manual)
  , m_runFlushTread (false)
  , m_mayNeedFlush  (false)
  , m_fileMutex     ()
  , m_flushThread   ()
  , m_flushPeriod   (500) // ms
{
  COUT_LOG_TIMESTAMP <<  ">>> " << __func__;
  OpenFile_AppendMode();
  SetFlushMode(flushMode);
  COUT_LOG_TIMESTAMP <<  "<<< " << __func__;
}
//
//  End of fct_CustomFileSink::CustomFileSink
//---------------------------------------------------------------------------

//! Constructs with implicit default log formatter
//!
CustomFileSink::CustomFileSink (const std::string& path, FlushMode flushMode)
  : CustomFileSink(path, flushMode, LogFormatter())
{
}
//
//  End of fct_CustomFileSink::CustomFileSink
//---------------------------------------------------------------------------


//! Default constructor
//!
CustomFileSink::CustomFileSink ()
  : CustomFileSink("")
{
}
//
//  End of fct_CustomFileSink::CustomFileSink
//---------------------------------------------------------------------------




//! Clears current log file (truncates it)
//!
void CustomFileSink::Clear ()
{
  CloseFile();
  OpenFile(std::ios_base::trunc);
}
//
//  End of fct_CustomFileSink::Clear
//---------------------------------------------------------------------------



//! Closes current file (if any is open)
//!
void CustomFileSink::CloseFile ()
{
  std::lock_guard<std::mutex> lock(m_fileMutex);
  if (m_fileStream)
  {
    auto& stream = *m_fileStream.get();
    stream << std::flush;
    stream.close();
    m_fileStream.reset();
  }
}
//
//  End of fct_CustomFileSink::CloseFile
//---------------------------------------------------------------------------



//! Flushes to file any not flushed yet log messages
//!
void CustomFileSink::Flush ()
{
  if (m_mayNeedFlush)
  {
    std::lock_guard<std::mutex> lock(m_fileMutex);
    if (m_fileStream)
    {
      m_mayNeedFlush = false;
      COUT_LOG_TIMESTAMP << "Flushing !!!";
      m_fileStream->flush();
    }
  }
}
//
//  End of fct_CustomFileSink::Flush
//---------------------------------------------------------------------------


//! Periodically flushes any unflushed message to file
//!
void CustomFileSink::Flush_Periodically ()
{
  COUT_LOG_TIMESTAMP << ">>> " << __func__;
  try
  {
    while (m_runFlushTread)
    {
      Flush();
      std::this_thread::sleep_for(m_flushPeriod);
    }
  }
//+  catch(std::thread_interrupted&)
//+  {
//+    COUT_LOG_TIMESTAMP << "Flush thread_interrupted";
//+    Flush();
//+  }
  catch(std::exception& exc)
  {
    std::cerr << "CustomFileSink::Flush_Periodically got std::exception " << exc.what();
  }
  catch (...)     // Do not propagate any exception
  {
    std::cerr << "CustomFileSink::Flush_Periodically has catched unknown exception";
  }
  COUT_LOG_TIMESTAMP << "<<< " << __func__;
}
//
//  End of fct_CustomFileSink::Flush_Periodically
//---------------------------------------------------------------------------



//! Opens associated log file in APPEND or TRUNC mode
//!
bool CustomFileSink::OpenFile (std::ios_base::openmode truncateOrAppendMode)
{
  if (m_filePath.empty())
  {
    return false;
  }
  else
  {
    std::lock_guard<std::mutex> lock(m_fileMutex);

    auto fileStream = make_unique<std::ofstream>();

    fileStream->open(m_filePath, std::ios_base::out | truncateOrAppendMode);

    if (fileStream->is_open())
    {
      m_fileStream.swap(fileStream);
    }
    else
    {
       std::ostringstream ss_error;
       ss_error << "\nFILE ERROR:  could not open log file:[" << m_filePath << "]";
       ss_error << "\n\t\t std::ios_base state = "            << fileStream->rdstate();
       std::cerr << ss_error.str().c_str() << std::endl;
       fileStream->close();
       fileStream.reset();
       return false;
    }

    return true;
  }
}
//
//  End of fct_CustomFileSink::OpenFile
//---------------------------------------------------------------------------


//! Opens associated log file, appending to existing file
//!
bool CustomFileSink::OpenFile_AppendMode ()
{
  return OpenFile(std::ios_base::ate | std::ios_base::app);
}
//
//  End of fct_CustomFileSink::OpenFile
//---------------------------------------------------------------------------



//! Dumps to file unformatted yet log message
//!
void CustomFileSink::ReceiveLogUnformattedMessage (g3::LogMessageMover&& unformattedMessage)
{
  std::string formattedMessage = m_logFormatter.FormatAsStdString(std::move(unformattedMessage));
  ReceiveLogMessage(formattedMessage);
}
//
//  End of fct_CustomFileSink::ReceiveLogUnformattedMessage
//---------------------------------------------------------------------------



//! Dumps to file already formatted log message
//!
//! @note For efficiently purpose, except if required to, does not flush to file
//!       each time
void CustomFileSink::ReceiveLogMessage (const std::string& formattedMessage)
{
  if (!m_fileStream)
  {
    OpenFile_AppendMode();
  }

  if (m_fileStream)
  {
    auto& stream = *m_fileStream.get();

    std::lock_guard<std::mutex> lock(m_fileMutex);
    stream << formattedMessage;

    if (m_automaticEol)
    {
      stream << "\n";
    }

    m_mayNeedFlush = true;

    if (m_flushMode == FlushMode::EachMessage)
    {
      m_mayNeedFlush = false;
      m_fileStream->flush();
    }
  }
}
//
//  End of fct_CustomFileSink::ReceiveLogMessage
//---------------------------------------------------------------------------



//! Changes file path, appending to specified file if it already exists.
//!
bool CustomFileSink::SetFilePath (const std::string& path)
{
  CloseFile();
  {
    std::lock_guard<std::mutex> lock(m_fileMutex);
    m_filePath = path;
  }
  return OpenFile_AppendMode();
}
//
//  End of fct_CustomFileSink::SetFilePath
//---------------------------------------------------------------------------



//! Sets new flush mode
//!
void CustomFileSink::SetFlushMode (FlushMode flushMode)
{
  if (flushMode != m_flushMode)
  {
    if (m_flushMode == FlushMode::AutoBackground)
    {
      StopFlush_Thread();
    }
    else if (flushMode == FlushMode::AutoBackground)
    {
      StartFlush_Thread();
    }
    m_flushMode = flushMode;
  }
}
//
//  End of fct_CustomFileSink::SetFlushMode
//---------------------------------------------------------------------------



//! Changes log message formatter
//!
//! @note Caller must use the proper API to serialize this call with reception of log messages.
//!       This is normally done using SinkHandle that do the job
//!
void CustomFileSink::SetFormatter (const LogFormatter& logFormatter)
{
  m_logFormatter = logFormatter;
}
//
//  End of fct_CustomFileSink::SetFormatter
//---------------------------------------------------------------------------


//! (Re)Starts saving to file
//!
void CustomFileSink::Start ()
{
  OpenFile_AppendMode();
  if (m_flushMode == FlushMode::AutoBackground)
  {
    StartFlush_Thread();
  }
}
//
//  End of fct_CustomFileSink::Start
//---------------------------------------------------------------------------


//! (Re)Stops saving to file
//!
void CustomFileSink::Stop ()
{
  CloseFile();
  if (m_flushMode == FlushMode::AutoBackground)
  {
    StopFlush_Thread();
  }
}
//
//  End of fct_CustomFileSink::Stop
//---------------------------------------------------------------------------


//! Starts flush thread
//!
void CustomFileSink::StartFlush_Thread ()
{
  m_runFlushTread = true;
  m_flushThread   = std::thread([this]() { this->Flush_Periodically(); });
}
//
//  End of fct_CustomFileSink::StartFlush_Thread
//---------------------------------------------------------------------------



//! If active, stops flush thread
//!
void CustomFileSink::StopFlush_Thread ()
{
  m_runFlushTread = false;
  if (m_flushThread.joinable())
  {
//+    m_flushThread.interrupt();
    m_flushThread.join();
  }
}
//
//  End of fct_CustomFileSink::StopFlush_Thread
//---------------------------------------------------------------------------



//===========================================================================
// End of CustomFileSink.cpp
//===========================================================================
