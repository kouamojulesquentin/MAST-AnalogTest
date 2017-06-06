//===========================================================================
//                           StreamOutput.h
//===========================================================================
//
// Project : Mast
//
//! @file StreamOutput.h
//!
//! Implements class StreamOutput
//!
//===========================================================================

#ifndef STREAMOUTPUT_H__E7FB818A_2EDA_4D0A_6C99_F3009B882544__INCLUDED_
  #define STREAMOUTPUT_H__E7FB818A_2EDA_4D0A_6C99_F3009B882544__INCLUDED_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <ostream>

#include <tclap/CmdLineInterface.h>
#include <tclap/CmdLineOutput.h>
#include <tclap/XorHandler.h>
#include <tclap/Arg.h>

namespace TCLAP
{

//! Stream output for the CmdLine into an output stream
//!
//! @note This is somewhat a copy from StdOutput
//! @note This is useful for unit tests using string stream
//!
class StreamOutput : public CmdLineOutput
{
  std::ostream& m_stdStream; //!< Reference to output stream to which standard messages will be sent to
  std::ostream& m_errStream; //!< Reference to output stream to which error messages will be sent to
  size_t        m_maxWidth  = 95u;
  size_t        m_stdIndent = 2u;

  //! @todo [JFC]-[May/31/2017]: Factor this and StdOutput into a single one requesting stream for std and error messages
  //!

  public:
    virtual ~StreamOutput() = default;
    StreamOutput() = delete;

    //! Constructs a StreamOutput with output streams for
    //! standard and error messages
    //!
    //! @note Same stream can be used for standard and error messages
    //!
    //! @param stdStream   Output stream for standard messages
    //! @param errStream   Output stream for error messages
    //!
    StreamOutput(std::ostream& stdStream, std::ostream& errStream)
      : m_stdStream (stdStream)
      , m_errStream (errStream)
    {
    }

    /**
     * Prints the usage to stdout.  Can be overridden to
     * produce alternative behavior.
     * \param c - The CmdLine object the output is generated for.
     */
    virtual void usage(CmdLineInterface& c);

    /**
     * Prints the version to stdout. Can be overridden
     * to produce alternative behavior.
     * \param c - The CmdLine object the output is generated for.
     */
    virtual void version(CmdLineInterface& c);

    /**
     * Prints (to stderr) an error message, short usage
     * Can be overridden to produce alternative behavior.
     * \param c - The CmdLine object the output is generated for.
     * \param e - The ArgException that caused the failure.
     */
    virtual void failure(CmdLineInterface& c, ArgException& e);

  protected:

    /**
    * Writes a brief usage message with short args.
    * \param c - The CmdLine object the output is generated for.
    * \param os - The stream to write the message to.
    */
    void _shortUsage(CmdLineInterface& c, std::ostream& os) const;

    /**
    * Writes a longer usage message with long and short args,
    * provides descriptions and prints message.
    * \param c - The CmdLine object the output is generated for.
    * \param os - The stream to write the message to.
    */
    void _longUsage(CmdLineInterface& c, std::ostream& os) const;

    /**
     * This function inserts line breaks and indents long strings
     * according the  params input. It will only break lines at spaces,
     * commas and pipes.
     * \param os - The stream to be printed to.
     * \param s - The string to be printed.
     * \param maxWidth - The maxWidth allowed for the output line.
     * \param indentSpaces - The number of spaces to indent the first line.
     * \param secondLineOffset - The number of spaces to indent the second
     * and all subsequent lines in addition to indentSpaces.
     */
    void spacePrint(std::ostream&      os,
                    const std::string& s,
                    size_t             maxWidth,
                    size_t             indentSpaces,
                    size_t             secondLineOffset) const;
};


inline void StreamOutput::version(CmdLineInterface& _cmd)
{
  std::string progName = _cmd.getProgramName();
  std::string version = _cmd.getVersion();

  m_stdStream << std::endl     << progName
              << "  version: " << version << std::endl << std::endl;
}

inline void StreamOutput::usage(CmdLineInterface& _cmd)
{
  m_stdStream << std::endl << "USAGE: " << std::endl << std::endl;

  _shortUsage(_cmd, m_stdStream);

  m_stdStream << std::endl << std::endl << "Where: " << std::endl << std::endl;

  _longUsage(_cmd, m_stdStream);

  m_stdStream << std::endl;
}

inline void StreamOutput::failure(CmdLineInterface& _cmd,
                        ArgException& e)
{
  std::string progName = _cmd.getProgramName();

  m_errStream << "PARSE ERROR: " << e.argId() << std::endl
              << "             " << e.error() << std::endl << std::endl;

  if (_cmd.hasHelpAndVersion())
  {
    m_errStream << "Brief USAGE: " << std::endl;

    _shortUsage(_cmd, m_stdStream);

    m_errStream << std::endl << "For complete USAGE and HELP type: "
                << std::endl << "   " << progName << " --help"
                << std::endl << std::endl;
  }
  else
    usage(_cmd);

}

inline void StreamOutput::_shortUsage(CmdLineInterface& _cmd, std::ostream& os) const
{
  std::list<Arg*> argList                = _cmd.getArgList();
  std::string progName                   = _cmd.getProgramName();
  XorHandler xorHandler                  = _cmd.getXorHandler();
  std::vector<std::vector<Arg*>> xorList = xorHandler.getXorList();

  os << std::string(m_stdIndent, ' ') << progName;

  size_t currentIndent    = 2u; // (after program name)
  size_t secondLineOffset = 5u; // Offset relative to first line

  size_t indent = m_stdIndent + progName.length() + 2u;

  if (indent > (m_maxWidth / 2u)) // Must deal with tool long program name
  {
    indent        = m_maxWidth / 2u;
    currentIndent = indent;
  }

  // first the xor
  std::string s;
  for (int i = 0; static_cast<unsigned int>(i) < xorList.size(); i++)
  {
    s += " {";
    for (ArgVectorIterator it = xorList[i].begin(); it != xorList[i].end(); it++)
      s += (*it)->shortID() + "|";

    s[s.length()-1] = '}';
  }

  if (!s.empty())
  {
    spacePrint(os, s, m_maxWidth, m_stdIndent, secondLineOffset);
    currentIndent = indent;
  }

  // then the rest (normally aligned to the end of application name)
  for (ArgListIterator it = argList.begin(); it != argList.end(); it++)
  {
    if (!xorHandler.contains((*it)))
    {
      spacePrint(os, (*it)->shortID(), m_maxWidth, currentIndent, secondLineOffset);
      currentIndent = indent;
    }
  }
}

inline void StreamOutput::_longUsage(CmdLineInterface& _cmd, std::ostream& os) const
{
  const std::list<Arg*>&                argList    = _cmd.getArgList();
  const XorHandler&                     xorHandler = _cmd.getXorHandler();
  const std::vector<std::vector<Arg*>>& xorList    = xorHandler.getXorList();

  spacePrint(os, _cmd.getMessage(), m_maxWidth, m_stdIndent, 0);
  os << std::endl;

  // first the xor
  for (size_t i = 0; i < xorList.size(); i++)
  {
    for (ArgVectorConstIterator it = xorList[i].cbegin(); it != xorList[i].cend(); it++)
    {
      spacePrint(os, (*it)->longID(),         m_maxWidth, m_stdIndent, 3u);
      spacePrint(os, (*it)->getDescription(), m_maxWidth, m_stdIndent + 2u, 0);

      if (it+1 != xorList[i].end())
        spacePrint(os, "-- OR --", m_maxWidth, 9, 0);
    }
    os << std::endl << std::endl;
  }

  // then the rest
  for (ArgListConstIterator it = argList.cbegin(); it != argList.cend(); it++)
  {
    if (!xorHandler.contains(*it))
    {
      const std::string& longID = (*it)->longID();
      if (longID.length() < (m_maxWidth - m_stdIndent - 3u))
      {
        spacePrint(os, longID, m_maxWidth, m_stdIndent, 3u);
      }
      else
      {
        size_t dashDashPos = longID.find("--");
        if (dashDashPos != std::string::npos)
        {
          spacePrint(os, longID.substr(0, dashDashPos), m_maxWidth, m_stdIndent, 3u);
          spacePrint(os, longID.substr(dashDashPos),    m_maxWidth, m_stdIndent, 3u);
        }
        else
        {
          spacePrint(os, longID, m_maxWidth, m_stdIndent, 3u);
        }
      }
      spacePrint(os, (*it)->getDescription(), m_maxWidth, m_stdIndent + 2u, 0);

      os << std::endl;
    }
  }
}

inline void StreamOutput::spacePrint(std::ostream&      os,
                                     const std::string& s,
                                     size_t             maxWidth,
                                     size_t             indentSpaces,
                                     size_t             secondLineOffset) const
{
  const std::string indent           = std::string(indentSpaces, ' ');
  const std::string secondLineIndent = std::string(indentSpaces  + secondLineOffset, ' ');

  size_t len = s.length();

  if ((len + indentSpaces > maxWidth) && maxWidth > 0)
  {
    size_t allowedLen = maxWidth - indentSpaces;
    size_t start      = 0;

    while (start < len)
    {
      // find the substring length
      size_t stringLen = std::min<size_t>(len - start, allowedLen);

      // trim the length so it doesn't end in middle of a word
      if (stringLen == allowedLen)
      {
        while (stringLen          != 0   &&
               s[stringLen+start] != ' ' &&
               s[stringLen+start] != ',' &&
               s[stringLen+start] != '|')
        {
          stringLen--;
        }
      }

      // ok, the word is longer than the line, so just split
      // wherever the line ends
      if (stringLen <= 0)
        stringLen = allowedLen;

      // check for newlines
      for (size_t i = 0; i < stringLen; i++)
        if (s[start+i] == '\n')
          stringLen = i+1;


      if (start == 0)
      {
        os << indent;

        indentSpaces += secondLineOffset;
        allowedLen   -= secondLineOffset;
      }
      else
      {
        os << secondLineIndent;
      }

      os << s.substr(start, stringLen) << std::endl;

      // so we don't start a line with a space
      while (s[stringLen + start] == ' ' && start < len)
        start++;

      start += stringLen;
    }
  }
  else
  {
    os << indent << s << std::endl;
  }
}
} //namespace TCLAP

#endif  // not defined STREAMOUTPUT_H__E7FB818A_2EDA_4D0A_6C99_F3009B882544__INCLUDED_

//===========================================================================
// End of StreamOutput.h
//===========================================================================
