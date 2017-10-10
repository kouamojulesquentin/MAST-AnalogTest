//===========================================================================
//                           ParserException.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ParserException.hpp
//!
//! Declares ParserException class
//!
//===========================================================================


#ifndef PARSEREXCEPTION_H__80BC7A3F_EE0B_4DE3_6A98_95C43E1D45FF__INCLUDED_
  #define PARSEREXCEPTION_H__80BC7A3F_EE0B_4DE3_6A98_95C43E1D45FF__INCLUDED_

#include <sstream>
#include <stdexcept>

namespace mast
{
  //! Exception thrown internally by ICL_Parser
  //!
  class ParserException : public std::runtime_error
  {
    public:
    std::string  parserKind;       //!< Tells what kind of file (language) was parsed
    std::string  baseErrorMessage; //!< Message about the kind of error (without reference to file, line and columns)
    std::string  filePath;         //!< When appropriate, provide which file was parsed when the error is reported
    unsigned int line        = 0;  //!< Line on which parsing error is detected (this may differ from real cause of error)
    unsigned int beginColumn = 0;  //!< First column on which parsing error is detected
    unsigned int endColumn   = 0;  //!< Last column on which parsing error is detected

    public:
    ParserException(const std::string& errorMessage)
      : std::runtime_error (errorMessage)
      , baseErrorMessage   (errorMessage)
    {
    }

    ParserException(const char* errorMessage)
      : std::runtime_error (errorMessage)
      , baseErrorMessage   (errorMessage)
    {
    }

    ParserException(std::experimental::string_view p_parserKind,
                    std::experimental::string_view p_filePath,
                    unsigned int                   p_line,
                    unsigned int                   beginCol,
                    unsigned int                   endCol,
                    std::experimental::string_view errorMessage)
      : std::runtime_error (MakeMessage(p_parserKind, errorMessage, p_filePath, p_line, beginCol, endCol))
      , parserKind         (p_parserKind)
      , baseErrorMessage   (errorMessage)
      , filePath           (p_filePath)
      , line               (p_line)
      , beginColumn        (beginCol)
      , endColumn          (endCol)
    {
    }

    ParserException(unsigned int p_line, unsigned int beginCol, unsigned int endCol, std::experimental::string_view errorMessage)
      : ParserException("", "", p_line, beginCol, endCol, errorMessage)
    {
    }

    //! Builds a message based on saved location of error
    //!
    std::string Message()
    {
      return MakeMessage(parserKind, baseErrorMessage, filePath, line, beginColumn, endColumn);
    }

    private:
    static std::string MakeMessage(std::experimental::string_view parserKind,
                                   std::experimental::string_view errorMessage,
                                   std::experimental::string_view filePath,
                                   unsigned int                   line,
                                   unsigned int                   beginCol,
                                   unsigned int                   endCol)
    {
      std::ostringstream os;
      if (!parserKind.empty())
      {
        os << parserKind << " ";
      }

      os << "Parsing error: ";
      if (!filePath.empty())
      {
        os << "File \"" << filePath << "\" ";
      }
      os << "Line "  << line << ":" << beginCol << "-" << endCol << ": " << errorMessage;

      return os.str();
    }
  };
}

#endif  // not defined PARSEREXCEPTION_H__80BC7A3F_EE0B_4DE3_6A98_95C43E1D45FF__INCLUDED_

//===========================================================================
// End of ParserException.hpp
//===========================================================================
