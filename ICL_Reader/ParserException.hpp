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

namespace ICL
{
  //! Exception thrown internally by ICL_Parser
  //!
  class ParserException : public std::runtime_error
  {
    unsigned int line        = 0;
    unsigned int beginColumn = 0;
    unsigned int endColumn   = 0;

    public:
    ParserException(const std::string& what_arg)
     : std::runtime_error(what_arg)
    {
    }

    ParserException(const char* what_arg)
     : std::runtime_error(what_arg)
    { }

    ParserException(unsigned int line, unsigned int beginCol, unsigned int endCol, std::experimental::string_view errorMessage)
     : std::runtime_error (MakeMessage(line, beginCol, endCol, errorMessage))
     , line               (line)
     , beginColumn        (beginCol)
     , endColumn          (endCol)
    { }

    private:
    static std::string MakeMessage(unsigned int line, unsigned int beginCol, unsigned int endCol, std::experimental::string_view errorMessage)
    {
      std::ostringstream os;
      os << "ICL error: " << "Line "  << line << ":" << beginCol << "-" << endCol << ": " << errorMessage;

      return os.str();
    }
  };
}

#endif  // not defined PARSEREXCEPTION_H__80BC7A3F_EE0B_4DE3_6A98_95C43E1D45FF__INCLUDED_

//===========================================================================
// End of ParserException.hpp
//===========================================================================
