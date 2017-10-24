//===========================================================================
//                           SIT_Scanner.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Scanner.hpp
//!
//! Declares ICL_Scanner class
//!
//===========================================================================


#ifndef __SITSCANNER_HPP__
#define __SITSCANNER_HPP__ 1

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "SIT_Parser.tab.hh"
#include "location.hh"

namespace SIT
{
//! Implements Cpp interface for SIT lexer
//!
//! @note It avoids having renamed copy of yyFlexLexer
//!
class SIT_Scanner final: public yyFlexLexer
{
  public:

   ~SIT_Scanner()
   {
      delete loc;
  }
  SIT_Scanner(std::istream& stream)
    : yyFlexLexer(&stream)
    , loc(new SIT::SIT_Parser::location_type())
  {
     loc->initialize();
  };

  // Import base class yylex methods (==> get rid of override virtual function warning)
   using FlexLexer::yylex;

   int yylex(SIT_Parser::semantic_type* const lval, SIT_Parser::location_type* location);


private:
   SIT_Parser::semantic_type* yylval = nullptr;
   SIT_Parser::location_type* loc    = nullptr;
};
//
//  End of SIT_Scanner
//---------------------------------------------------------------------------

} /* end namespace SIT */

#endif /* END __SITSCANNER_HPP__ */
