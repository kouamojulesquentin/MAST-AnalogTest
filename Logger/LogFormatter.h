//===========================================================================
//                           LogFormatter.h
//===========================================================================
//
//! @file LogFormatter.h
//!
//! Declares class LogFormatter
//===========================================================================

#ifndef LOGFORMATTER_H__A59884F6_F5AA_430F_B992_37EA6E48C4__INCLUDED_
  #define LOGFORMATTER_H__A59884F6_F5AA_430F_B992_37EA6E48C4__INCLUDED_


#include "g3log/g3log_Export.hpp"
#include "g3log/logmessage.hpp"
#include <string>
#include <tuple>

namespace g3
{
  //! Formats messages from g3log library
  //!
  class G3LOG_DLL_EXPORT LogFormatter final
  {
    bool m_showDate         = true;
    bool m_showFileName     = true;
    bool m_showFunctionName = true;
    bool m_showLevel        = true;
    bool m_showLineNumber   = true;
    bool m_showMicroseconds = true;
    bool m_showThreadId     = true;
    bool m_showTime         = true;
    int  m_indentationWidth = 4;      // When != 0, specify depth indentation width

    public:
    ~LogFormatter() = default;
    LogFormatter()  = default;
    LogFormatter(const LogFormatter&)            = default;
    LogFormatter& operator=(const LogFormatter&) = default;


    void ShowDate            (bool showDate)         { m_showDate         = showDate;         };
    void ShowFileName        (bool showFileName)     { m_showFileName     = showFileName;     };
    void ShowFunctionName    (bool showFunctionName) { m_showFunctionName = showFunctionName; };
    void ShowLevel           (bool showLevel)        { m_showLevel        = showLevel;        };
    void ShowLineNumber      (bool showLineNumber)   { m_showLineNumber   = showLineNumber;   };
    void ShowMicroseconds    (bool showMicroseconds) { m_showMicroseconds = showMicroseconds; };
    void ShowThreadId        (bool showThreadId)     { m_showThreadId     = showThreadId;     };
    void ShowTime            (bool showTime)         { m_showTime         = showTime;         };
    void SetIndentationWidth (int  indentationWidth) { m_indentationWidth = indentationWidth; };

    std::string        FormatAsStdString (g3::LogMessageMover&& logMessage);

    // ---------------- For unit tests purpose
    //
//+    std::tuple<QRegularExpression, int> GetLogCheckRegex  (const LEVELS& level, const char* filePath, const char* functionName);
  };
  //
  //  End of LogFormatter class declaration
  //---------------------------------------------------------------------------
} // End of namespace g3


#endif  // not defined LOGFORMATTER_H__A59884F6_F5AA_430F_B992_37EA6E48C4__INCLUDED_

//===========================================================================
// End of LogFormatter.h
//===========================================================================
