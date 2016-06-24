//===========================================================================
//                           LogFormatter.cpp
//===========================================================================
//
//! @file LogFormatter.cpp
//!
//! Implements class that formatt g3log messages
//===========================================================================


#include "LogFormatter.h"
//+#include <QFileInfo>
#include <iomanip>

using g3::LogFormatter;

//! Formats the log message and return a std::string
//!
std::string LogFormatter::FormatAsStdString(g3::LogMessageMover&& logMessage)
{
  auto msg = logMessage.get();

  std::ostringstream os;


  if (m_showDate)
  {
    os << "[" << msg.timestamp("%Y/%m/%d") << "]";
  }

  if (m_showTime)
  {
    os << "[" << msg.timestamp("%H:%M:%S") << "]";
  }

  if (m_showMicroseconds)
  {
    constexpr auto millisecondsPerMicrosecond = 1000LL;
    constexpr auto secondsPerMicrosecond      = 1000000LL;
    constexpr auto secondsPerMinute           = 60LL;

    auto microseconds          = msg._microseconds;
    auto seconds               = microseconds          / secondsPerMicrosecond;
    auto remainingMicroseconds = microseconds          % secondsPerMicrosecond;
    auto milliseconds          = remainingMicroseconds / millisecondsPerMicrosecond;

    microseconds = remainingMicroseconds % millisecondsPerMicrosecond;

    os << std::setfill('0');
    os << "[" << std::setw(2) << seconds % secondsPerMinute;
    os << ":" << std::setw(3) << milliseconds;
    os << "." << std::setw(3) << microseconds;
    os << " us]";
  }

  if (m_showThreadId)
  {
    os << "[id:" << std::setfill('0') << std::setw(2) << msg.threadID() << "]";
  }

  if (m_showLevel)
  {
    os << "[" << std::left << std::setfill(' ') << std::setw(7) << msg.level() << "]";
  }

  if (m_showFileName)
  {
    os << "[" << std::left << std::setfill(' ') << std::setw(32) << msg.file() << "]";
  }

  if (m_showLineNumber)
  {
    os << "[" << std::setfill('0') << std::right << std::setw(4) << msg.line() << "]";
  }

  if (m_showFunctionName)
  {
    os << "[" << std::left << std::setfill(' ') << std::setw(32) << msg.function() << "]";
  }

  os << " : ";

  if (m_indentationWidth != 0)
  {
    os << std::string(msg.depth() * m_indentationWidth, ' ');
  }

  os << msg.message();

  auto formattedMsg = os.str();
  return formattedMsg;
}

//! Returns a regex to check a log item
//!
//! @note This is primarily intended for unit test (checking logged/formatted) message
//! @note The expression does not test content of the logged message because it could contains special characters for regex
//! @note expected log examples:
//!     "[2015/08/26][20:49:33][18:108.246 us][id:01][   INFO][   UT_G3Log_OOTB.cpp][0102][            test_Log_Info] : Logged test using stream interface !!!"
//!     "[2015/08/26][20:49:33][18:108.246 us][id:01][   INFO][     TemplateApi.cpp][0136][ Template_SearchByKeyword] : -->> Template_SearchByKeyword("with keywordOrName: ", "-") >>---- "
//!     "[2015/08/26][20:49:34][18:108.246 us][id:01][   INFO][   UT_G3Log_OOTB.cpp][0102][ Template_SearchByKeyword] : -->> Tmpl_GetKeywordDelimiters >>---- "
//!     "[2015/08/26][20:49:35][18:108.246 us][id:01][   INFO][   UT_G3Log_OOTB.cpp][0102][ Template_SearchByKeyword] : --<< Tmpl_GetKeywordDelimiters <<---- "
//!
//+std::tuple<QRegularExpression, int> LogFormatter::GetLogCheckRegex(const LEVELS& level, const char* filePath, const char* functionName)
//+{
//+  // ---------------- Trim path to get only file name
//+  //
//+  QFileInfo fileInfo(filePath);
//+  auto fileName = fileInfo.fileName();

//+  QString regex;
//+  int     messageGroupIndex = 3;
//+  if (m_showDate)
//+  {
//+    regex += R"((\[\d{4}/\d{2}/\d{2}\]))";
//+    ++messageGroupIndex;
//+  }

//+  if (m_showTime)
//+  {
//+    regex += R"((\[\d{2}:\d{2}:\d{2}\]))";
//+    ++messageGroupIndex;
//+  }

//+  if (m_showMicroseconds)
//+  {
//+    regex += R"((\[\d{2}:\d{3}.\d{3} us\]))";
//+    ++messageGroupIndex;
//+  }

//+  if (m_showThreadId)
//+  {
//+    regex += R"((\[id:\d+\]))";
//+    ++messageGroupIndex;
//+  }

//+  if (m_showLevel)
//+  {
//+    regex += QString(R"((\[\s*%0\s*\]))").arg(level.text.c_str());
//+    ++messageGroupIndex;
//+  }

//+  if (m_showFileName)
//+  {
//+    regex += QString(R"((\[\s*%0\s*\]))").arg(fileName);
//+    ++messageGroupIndex;
//+  }

//+  if (m_showLineNumber)
//+  {
//+    regex += R"((\[\d+\]))";
//+    ++messageGroupIndex;
//+  }

//+  if (m_showFunctionName)
//+  {
//+    regex += QString(R"((\[\s*%0\s*\]))").arg(functionName);
//+    ++messageGroupIndex;
//+  }

//+  regex += R"( :\s+(--(>>|<<)\s)?(.+)(\s+(>>|<<)----)?$)";

//+  return std::make_tuple(QRegularExpression(regex), messageGroupIndex);
//+}


//===========================================================================
// End of LogFormatter.cpp
//===========================================================================
